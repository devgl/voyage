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

    public:
        void CPUSignal(uint64_t signal);
        bool IsSignaled(uint64_t signal);
        uint64_t NextSignal();

    private:
        vk::Device _device;
        vk::Semaphore _semaphore;
        std::atomic_uint64_t _signal;
    };

    class Swapchain
    {
        friend class RHI;

    public:
        vk::Image GetBackbuffer(uint32_t index);
        uint32_t GetCurrentFrameIndex() const;

    private:
        vk::SurfaceKHR _surface;
        vk::SwapchainKHR _swapchain;
        vk::Fence _nextFrameFence;

		std::vector<vk::Image> _backbuffers;
		uint32_t _currentFrameIndex;
    };

    class RHI
    {
    public:
        RHI();
        vk::CommandPool AllocateCommandPool(CommandQueueType type, uint64_t categoryHash);
        void AllocateCommandBuffer(CommandQueueType type, vk::CommandPool commandpool, vk::CommandBufferLevel level, uint32_t count, vk::CommandBuffer* pCommandBuffers);
        void FreeCommandPool(CommandQueueType type, vk::CommandPool commandpool, Semaphore* semaphore, uint64_t signal);

        Semaphore* AllocateSemaphore(uint64_t initialvalue);
        void FreeSemaphore(Semaphore* semaphore);

        Swapchain* CreateSwapchain(intptr_t hwnd, uint32_t minFrameCount);
        bool NextFrameReady(Swapchain* swapchain);
        void Present(Swapchain* swapchain, Semaphore* semaphore);
        void DestroySwapchain(Swapchain* swapchain);

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
