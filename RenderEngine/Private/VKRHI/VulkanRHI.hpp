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
        void SubmitCommandBuffers(CommandQueueType type, uint32_t count, const vk::SubmitInfo* pInfos);
        void FreeCommandPool(CommandQueueType type, vk::CommandPool commandpool, Semaphore* semaphore, uint64_t signal);

        Semaphore* AllocateSemaphore(uint64_t initialvalue);
        void CPUSignal(Semaphore* semaphore, uint64_t signal);
        bool IsSignaled(Semaphore* semaphore, uint64_t signal);
        uint64_t NextSignal(Semaphore* semaphore);
        void FreeSemaphore(Semaphore* semaphore);

        Swapchain* CreateSwapchain(intptr_t hwnd, uint32_t minFrameCount);
        vk::Image GetBackbuffer(Swapchain* swapchain, uint32_t index);
        uint32_t GetCurrentFrameIndex(Swapchain* swapchain) const;
        uint32_t GetFrameCount(Swapchain* swapchain) const;
        bool NextFrameReady(Swapchain* swapchain);
        void Present(Swapchain* swapchain, Semaphore* semaphore);
        void DestroySwapchain(Swapchain* swapchain);

    public:
        vk::Device device;
        vk::PipelineCache pipelineCache;

    private:
        void _CreateInstance();
        void _SelectPhysicalDevice();
        void _CreateDevice();
        vk::Instance _instance;
        vk::PhysicalDevice _physicalDevice;

        static constexpr uint32_t N = CommandQueueType_Transfer + 1;
        vk::Queue _queues[N];
        uint32_t _queuefamilyIndex[N];
        std::vector<void*> _commandpools[N];
    };
}
