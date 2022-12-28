#pragma once

#include "VulkanRHI.hpp"

namespace voyage
{
	struct RenderContext
	{
		RHI* rhi;
		const vk::Rect2D& GetFullScreenScissorRect() const;
		const vk::Viewport& GetFullScreenViewport() const;

	private:
		vk::Rect2D _rect;
		vk::Viewport _viewport;
	};

	class RenderPass
	{
	public:
		RenderPass(RHI* rhi);
		virtual ~RenderPass() = default;
		virtual void Create(RHI* rhi, Swapchain* swapchain);
		virtual void Destory(RHI* rhi);
		virtual void Draw(RenderContext* renderContext, vk::CommandBuffer cmd);
	};
}