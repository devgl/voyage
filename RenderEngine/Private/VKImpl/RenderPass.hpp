#pragma once

#include "VulkanRHI.hpp"

namespace voyage
{
	struct RenderContext
	{
		RHI* rhi;
		vk::Rect2D fullScreenScissorRect;
		vk::Viewport fullScreenViewport;
	};

	class RenderPass
	{
	public:
		virtual ~RenderPass() = default;
		virtual void Create(RHI* rhi, Swapchain* swapchain);
		virtual void Destory(RHI* rhi);
		virtual void Execute(RenderContext* renderContext, vk::CommandBuffer cmd);
	};
}