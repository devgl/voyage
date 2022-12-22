#include "VulkanRHI.hpp"

namespace voyage
{
	RHI::RHI()
	{
		_CreateInstance();
		_SelectPhysicalDevice();
		_CreateDevice();
	}

	void RHI::_CreateInstance()
	{
		constexpr const char* extensions[] = {
			VK_KHR_SURFACE_EXTENSION_NAME,
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		};

		constexpr const char* layers[] = {
			"VK_LAYER_KHRONOS_validation"
		};

		vk::ApplicationInfo appInfo{};
		appInfo.apiVersion = VK_API_VERSION_1_3;

		vk::InstanceCreateInfo instanceInfo{};
		instanceInfo.setPApplicationInfo(&appInfo);
		instanceInfo.setPEnabledExtensionNames(extensions);
		instanceInfo.setPEnabledLayerNames(layers);
		_instance = vk::createInstance(instanceInfo);
	}

	void RHI::_SelectPhysicalDevice()
	{
		auto physicalDevices = _instance.enumeratePhysicalDevices();
		if (physicalDevices.size() == 1)
		{
			_physicalDevice = physicalDevices[0];
		}
		else
		{
			// select the most powerful one
			_physicalDevice = physicalDevices[0];
		}

		uint32_t queueFamilyIndex = 0;
		auto queueFamilys = _physicalDevice.getQueueFamilyProperties();
		for (const auto& qf : queueFamilys)
		{
			if ((qf.queueFlags & vk::QueueFlagBits::eGraphics) 
				&&(qf.queueFlags & vk::QueueFlagBits::eCompute)
				&&(qf.queueFlags & vk::QueueFlagBits::eTransfer))
			{
				_queuefamilyIndex[CommandQueueType_Graphics] = queueFamilyIndex;
			}
			if (!(qf.queueFlags & vk::QueueFlagBits::eGraphics)
				&& (qf.queueFlags & vk::QueueFlagBits::eCompute)
				&& (qf.queueFlags & vk::QueueFlagBits::eTransfer))
			{
				_queuefamilyIndex[CommandQueueType_Compute] = queueFamilyIndex;
			}
			if (!(qf.queueFlags & vk::QueueFlagBits::eGraphics)
				&& !(qf.queueFlags & vk::QueueFlagBits::eCompute)
				&& (qf.queueFlags & vk::QueueFlagBits::eTransfer))
			{
				_queuefamilyIndex[CommandQueueType_Transfer] = queueFamilyIndex;
			}

			queueFamilyIndex++;
		}
	}

	void RHI::_CreateDevice()
	{
		constexpr const char* extensions[] = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME
		};

		float queuePriority = 1.0f;
		vk::DeviceQueueCreateInfo queueInfo[N];
		queueInfo[CommandQueueType_Graphics].pQueuePriorities = &queuePriority;
		queueInfo[CommandQueueType_Graphics].queueCount = 1;
		queueInfo[CommandQueueType_Graphics].queueFamilyIndex = _queuefamilyIndex[CommandQueueType_Graphics];

		queueInfo[CommandQueueType_Compute].pQueuePriorities = &queuePriority;
		queueInfo[CommandQueueType_Compute].queueCount = 1;
		queueInfo[CommandQueueType_Compute].queueFamilyIndex = _queuefamilyIndex[CommandQueueType_Compute];

		queueInfo[CommandQueueType_Transfer].pQueuePriorities = &queuePriority;
		queueInfo[CommandQueueType_Transfer].queueCount = 1;
		queueInfo[CommandQueueType_Transfer].queueFamilyIndex = _queuefamilyIndex[CommandQueueType_Transfer];

		auto properties = _physicalDevice.enumerateDeviceExtensionProperties();

		vk::PhysicalDeviceVulkan12Features features{};
		features.timelineSemaphore = true;

		vk::DeviceCreateInfo info{};
		info.setPNext(&features);
		info.setPEnabledExtensionNames(extensions);
		info.setQueueCreateInfos(queueInfo);
		_device = _physicalDevice.createDevice(info);

		_queues[CommandQueueType_Graphics] = _device.getQueue(_queuefamilyIndex[CommandQueueType_Graphics], 0);
		_queues[CommandQueueType_Compute] = _device.getQueue(_queuefamilyIndex[CommandQueueType_Compute], 0);
		_queues[CommandQueueType_Transfer] = _device.getQueue(_queuefamilyIndex[CommandQueueType_Transfer], 0);
	}

	struct CommandPoolData
	{
		vk::CommandPool pool;
		vk::Semaphore semaphore;
		uint64_t signal;
		uint64_t categoryHash;
		uint32_t inuse : 1;
	};

	bool IsSignaledOrNull(vk::Device device, CommandPoolData* data)
	{
		if (data->semaphore)
		{
			return device.getSemaphoreCounterValue(data->semaphore) >= data->signal;
		}
		else
		{
			return true;
		}
	}

	vk::CommandPool RHI::AllocateCommandPool(CommandQueueType type, uint64_t categoryHash)
	{
		for (auto pPool : _commandpools[type])
		{
			auto data = static_cast<CommandPoolData*>(pPool);
			if (!data->inuse == 0 && data->categoryHash == categoryHash && IsSignaledOrNull(_device, data))
			{
				data->semaphore = nullptr;
				data->signal = 0;
				data->inuse = 1;
				_device.resetCommandPool(data->pool);
				return data->pool;
			}
		}

		vk::CommandPoolCreateInfo info{};
		info.queueFamilyIndex = _queuefamilyIndex[type];
		auto pool = _device.createCommandPool(info);

		auto data = new CommandPoolData;
		data->pool = pool;
		data->semaphore = nullptr;
		data->signal = 0;
		data->categoryHash = categoryHash;
		data->inuse = 1;
		_commandpools[type].push_back(data);

		return pool;
	}

	void RHI::FreeCommandPool(CommandQueueType type, vk::CommandPool commandpool, Semaphore* semaphore, uint64_t signal)
	{
		for (auto pPool : _commandpools[type])
		{
			auto data = static_cast<CommandPoolData*>(pPool);
			if (data->pool == commandpool)
			{
				data->semaphore = semaphore->semaphore;
				data->signal = signal;
				data->inuse = 0;
				break;
			}
		}
	}

	Semaphore* RHI::AllocateSemaphore(uint64_t initialvalue)
	{
		vk::SemaphoreTypeCreateInfo semaphoreType{};
		semaphoreType.initialValue = initialvalue;
		semaphoreType.semaphoreType = vk::SemaphoreType::eTimeline;

		vk::SemaphoreCreateInfo info{};
		info.setPNext(&semaphoreType);

		auto s = new Semaphore();
		s->semaphore = _device.createSemaphore(info);
		s->signal = initialvalue;
		return s;
	}

	void RHI::CPUSignal(Semaphore* semaphore, uint64_t signal)
	{
		vk::SemaphoreSignalInfo info{};
		info.semaphore = semaphore->semaphore;
		info.value = signal;

		_device.signalSemaphore(info);
		semaphore->signal.store(signal);
	}

	bool RHI::IsSignaled(Semaphore* semaphore, uint64_t signal)
	{
		return _device.getSemaphoreCounterValue(semaphore->semaphore) >= signal;
	}

	uint64_t RHI::NextSignal(Semaphore* semaphore)
	{
		return ++semaphore->signal;
	}

	void RHI::FreeSemaphore(Semaphore* semaphore)
	{
		if (semaphore != nullptr)
		{
			_device.destroySemaphore(semaphore->semaphore);
			delete semaphore;
		}
	}

	void RHI::AllocateCommandBuffer(CommandQueueType type, vk::CommandPool commandpool, vk::CommandBufferLevel level, uint32_t count, vk::CommandBuffer* pCommandBuffers)
	{
		vk::CommandBufferAllocateInfo info{};
		info.commandPool = commandpool;
		info.level = level;
		info.commandBufferCount = count;
		vk::resultCheck(_device.allocateCommandBuffers(&info, pCommandBuffers), "failed to allocate command buffers");
	}

	Swapchain* RHI::CreateSwapchain(intptr_t hwnd, uint32_t minFrameCount)
	{
		auto swapchain = new Swapchain();
		swapchain->currentFrameIndex = -1;

		vk::Win32SurfaceCreateInfoKHR surfaceInfo{};
		surfaceInfo.hwnd = (HWND)hwnd;
		surfaceInfo.hinstance = ::GetModuleHandle(NULL);
		swapchain->surface = _instance.createWin32SurfaceKHR(surfaceInfo);

		auto caps = _physicalDevice.getSurfaceCapabilitiesKHR(swapchain->surface);
		auto formats = _physicalDevice.getSurfaceFormatsKHR(swapchain->surface);

		vk::SwapchainCreateInfoKHR swapchainInfo{};
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.pQueueFamilyIndices = &_queuefamilyIndex[CommandQueueType_Graphics];
		swapchainInfo.surface = swapchain->surface;
		swapchainInfo.minImageCount = minFrameCount;
		swapchainInfo.imageFormat = formats[0].format;
		swapchainInfo.imageColorSpace = formats[0].colorSpace;
		swapchainInfo.imageExtent = caps.currentExtent;
		swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
		swapchainInfo.imageArrayLayers = 1;
		swapchain->swapchain = _device.createSwapchainKHR(swapchainInfo);

		vk::FenceCreateInfo fenceInfo{};
		swapchain->nextFrameFence = _device.createFence(fenceInfo);

		swapchain->backbuffers = _device.getSwapchainImagesKHR(swapchain->swapchain);

		return swapchain;
	}

	vk::Image RHI::GetBackbuffer(Swapchain* swapchain, uint32_t index)
	{
		return swapchain->backbuffers[index];
	}

	uint32_t RHI::GetCurrentFrameIndex(Swapchain* swapchain) const
	{
		return swapchain->currentFrameIndex;
	}

	bool RHI::NextFrameReady(Swapchain* swapchain)
    {
        auto result = _device.acquireNextImageKHR(swapchain->swapchain, 0, nullptr, swapchain->nextFrameFence, &swapchain->currentFrameIndex);
        if (result == vk::Result::eSuccess)
        {
			vk::resultCheck(_device.resetFences(1, &swapchain->nextFrameFence), "failed reset fences");
        }
        return result == vk::Result::eSuccess;
    }

	void RHI::Present(Swapchain* swapchain, Semaphore* semaphore)
	{
		auto queue = _queues[CommandQueueType_Graphics];

		vk::PresentInfoKHR info{};
		info.swapchainCount = 1;
		info.pSwapchains = &swapchain->swapchain;
		info.pImageIndices = &swapchain->currentFrameIndex;
		//info.waitSemaphoreCount = 1;
		//info.pWaitSemaphores = &semaphore->_semaphore;
		vk::resultCheck(queue.presentKHR(info), "failed to present swapchain");
	}

	void RHI::DestroySwapchain(Swapchain* swapchain)
	{
		_instance.destroySurfaceKHR(swapchain->surface);
		_device.destroyFence(swapchain->nextFrameFence);
		_device.destroySwapchainKHR(swapchain->swapchain);
		delete swapchain;
	}

	vk::DescriptorPool RHI::AllocateDescriptorPool(uint32_t maxSets, uint32_t sizeCount, vk::DescriptorPoolSize* pSizes)
	{
		vk::DescriptorPoolCreateInfo info{};
		info.maxSets = maxSets;
		info.poolSizeCount = sizeCount;
		info.pPoolSizes = pSizes;

		return _device.createDescriptorPool(info);
	}

	void RHI::AllocateDescriptorSets(vk::DescriptorPool descriptorPool, uint32_t count, vk::DescriptorSetLayout* pLayouts, vk::DescriptorSet* pDescriptorSets)
	{
		vk::DescriptorSetAllocateInfo info{};
		info.descriptorPool = descriptorPool;
		info.descriptorSetCount = count;
		info.pSetLayouts = pLayouts;
		vk::resultCheck(_device.allocateDescriptorSets(&info, pDescriptorSets), "failed to allocate descriptor sets");
	}

	void RHI::UpdateDescriptorSet(vk::DescriptorSet set, uint32_t countWrite, vk::WriteDescriptorSet* pWrites, uint32_t copyCount, vk::CopyDescriptorSet* pCopies)
	{
		_device.updateDescriptorSets(countWrite, pWrites, copyCount, pCopies);
	}

	void RHI::FreeDescriptorPool(vk::DescriptorPool descriptorPool)
	{
		_device.destroyDescriptorPool(descriptorPool);
	}

	vk::RenderPass RHI::CreateRenderPass()
	{
		vk::RenderPassCreateInfo info{};
		return _device.createRenderPass(info);
	}

	void RHI::DestroyRenderPass(vk::RenderPass pass)
	{
		_device.destroyRenderPass(pass);
	}

}