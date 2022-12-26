#pragma once

#include "VulkanRHI.hpp"

namespace voyage
{
	struct RenderContext
	{
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
		virtual void Draw(RenderContext* renderContext, vk::CommandBuffer cmd);
	};
}