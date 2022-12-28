#include "RenderPass.hpp"

namespace voyage
{
	const vk::Rect2D& RenderContext::GetFullScreenScissorRect() const
	{
		return _rect;
	}

	const vk::Viewport& RenderContext::GetFullScreenViewport() const
	{
		return _viewport;
	}


	RenderPass::RenderPass(RHI* rhi)
	{

	}

	void RenderPass::Draw(RenderContext* renderContext, vk::CommandBuffer cmd)
	{

	}

	void RenderPass::Create(RHI* rhi, Swapchain* swapchain)
	{

	}

	void RenderPass::Destory(RHI* rhi)
	{
	}
}