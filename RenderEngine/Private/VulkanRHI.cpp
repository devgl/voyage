#include "VulkanRHI.hpp"

namespace voyage
{
	bool Semaphore::IsSignaled(uint64_t signal)
	{
		return _device.getSemaphoreCounterValue(_semaphore) >= signal;
	}

	uint64_t Semaphore::NextSignal()
	{
		return ++_signal;
	}

	vk::Image Swapchain::GetBackbuffer(uint32_t index)
	{
		return _backbuffers[index];
	}

	uint32_t Swapchain::GetCurrentFrameIndex() const
	{
		return _currentFrameIndex;
	}

	RHI::RHI()
	{
		_CreateInstance();
		_SelectPhysicalDevice();
		_CreateDevice();
	}

	void RHI::_CreateInstance()
	{
		constexpr const char* extensions[] = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
			VK_KHR_SURFACE_EXTENSION_NAME,
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		};

		constexpr const char* layers[] = {
			"VK_LAYER_KHRONOS_validation"
		};

		vk::InstanceCreateInfo instanceInfo{};
		instanceInfo.setPpEnabledExtensionNames(extensions);
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
			VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,
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

		vk::DeviceCreateInfo info{};
		info.setPpEnabledExtensionNames(extensions);
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
				data->semaphore = semaphore->_semaphore;
				data->signal = signal;
				data->inuse = 0;
				break;
			}
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

	Swapchain* RHI::CreateSwapchain(uint32_t width, uint32_t height, vk::Format format, uint32_t minFrameCount)
	{
		vk::SurfaceKHR surface;

		auto swapchain = new Swapchain();
		swapchain->_currentFrameIndex = -1;

		vk::SwapchainCreateInfoKHR swapchainInfo{};
		swapchainInfo.queueFamilyIndexCount = 1;
		swapchainInfo.pQueueFamilyIndices = &_queuefamilyIndex[CommandQueueType_Graphics];
		swapchainInfo.surface = surface;
		swapchainInfo.minImageCount = minFrameCount;
		swapchainInfo.imageFormat = format;
		swapchainInfo.imageExtent = vk::Extent2D{ width, height };
		swapchainInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
		swapchain->_swapchain = _device.createSwapchainKHR(swapchainInfo);

		vk::FenceCreateInfo fenceInfo{};
		swapchain->_nextFrameFence = _device.createFence(fenceInfo);

		swapchain->_backbuffers = _device.getSwapchainImagesKHR(swapchain->_swapchain);

		return swapchain;
	}

	bool RHI::NextFrameReady(Swapchain* swapchain)
    {
        auto result = _device.acquireNextImageKHR(swapchain->_swapchain, 0, nullptr, swapchain->_nextFrameFence, &swapchain->_currentFrameIndex);
        if (result == vk::Result::eSuccess)
        {
            _device.resetFences(1, &swapchain->_nextFrameFence);
        }
        return result == vk::Result::eSuccess;
    }

	void RHI::Present(Swapchain* swapchain, Semaphore* semaphore)
	{
		auto queue = _queues[CommandQueueType_Graphics];

		vk::PresentInfoKHR info{};
		info.swapchainCount = 1;
		info.pSwapchains = &swapchain->_swapchain;
		info.pImageIndices = &swapchain->_currentFrameIndex;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &semaphore->_semaphore;
		vk::resultCheck(queue.presentKHR(info), "failed to present swapchain");
	}

	void RHI::DestroySwapchain(Swapchain* swapchain)
	{
		_device.destroyFence(swapchain->_nextFrameFence);
		_device.destroySwapchainKHR(swapchain->_swapchain);
		delete swapchain;
	}

}