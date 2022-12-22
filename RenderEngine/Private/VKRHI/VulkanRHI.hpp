#pragma once

#include <vulkan/vulkan.hpp>
#include <vma/vk_mem_alloc.h>
#include <memory>
#include <vector>
#include <atomic>

namespace voyage
{
    enum CommandQueueType
    {
        CommandQueueType_Graphics = 0,
        CommandQueueType_Compute = 1,
        CommandQueueType_Transfer = 2,
    };

    class Semaphore
    {
        friend class RHI;
        vk::Semaphore semaphore;
        std::atomic_uint64_t signal;
    };

    class Swapchain
    {
        friend class RHI;
        vk::SurfaceKHR surface;
        vk::SwapchainKHR swapchain;
        vk::Fence nextFrameFence;
		std::vector<vk::Image> backbuffers;
		uint32_t currentFrameIndex;
    };

    class RHI
    {
    public:
        RHI();
        vk::CommandPool AllocateCommandPool(CommandQueueType type, uint64_t categoryHash);
        void AllocateCommandBuffer(CommandQueueType type, vk::CommandPool commandpool, vk::CommandBufferLevel level, uint32_t count, vk::CommandBuffer* pCommandBuffers);
        void FreeCommandPool(CommandQueueType type, vk::CommandPool commandpool, Semaphore* semaphore, uint64_t signal);

        Semaphore* AllocateSemaphore(uint64_t initialvalue);
        void CPUSignal(Semaphore* semaphore, uint64_t signal);
        bool IsSignaled(Semaphore* semaphore, uint64_t signal);
        uint64_t NextSignal(Semaphore* semaphore);
        void FreeSemaphore(Semaphore* semaphore);

        Swapchain* CreateSwapchain(intptr_t hwnd, uint32_t minFrameCount);
        vk::Image GetBackbuffer(Swapchain* swapchain, uint32_t index);
        uint32_t GetCurrentFrameIndex(Swapchain* swapchain) const;
        bool NextFrameReady(Swapchain* swapchain);
        void Present(Swapchain* swapchain, Semaphore* semaphore);
        void DestroySwapchain(Swapchain* swapchain);

        vk::DescriptorPool AllocateDescriptorPool(uint32_t maxSets, uint32_t sizeCount, vk::DescriptorPoolSize* pSizes);
        void AllocateDescriptorSets(vk::DescriptorPool descriptorPool, uint32_t count, vk::DescriptorSetLayout* pLayouts, vk::DescriptorSet* pDescriptorSets);
        void UpdateDescriptorSet(vk::DescriptorSet set, uint32_t countWrite, vk::WriteDescriptorSet* pWrites, uint32_t copyCount, vk::CopyDescriptorSet* pCopies);
        void FreeDescriptorPool(vk::DescriptorPool descriptorPool);

        vk::RenderPass CreateRenderPass();
        void DestroyRenderPass(vk::RenderPass pass);

    private:
        void _CreateInstance();
        void _SelectPhysicalDevice();
        void _CreateDevice();
        vk::Instance _instance;
        vk::PhysicalDevice _physicalDevice;
        vk::Device _device;

        static constexpr uint32_t N = CommandQueueType_Transfer + 1;
        vk::Queue _queues[N];
        uint32_t _queuefamilyIndex[N];
        std::vector<void*> _commandpools[N];
    };
}