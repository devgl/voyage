#include "RenderPass.hpp"

namespace voyage
{

	RenderPass::RenderPass(RHI* rhi)
	{

	}

	void RenderPass::Draw(RenderContext* renderContext, vk::CommandBuffer cmd)
	{

	}

	const vk::Rect2D& RenderContext::GetFullScreenScissorRect() const
	{
		return _rect;
	}

	const vk::Viewport& RenderContext::GetFullScreenViewport() const
	{
		return _viewport;
	}

}