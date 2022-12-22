#include "Renderer.hpp"
#include "RenderEngine.hpp"
#include "RenderContext.hpp"
#include "OpaquePass.hpp"
#include "VulkanRHI.hpp"

namespace voyage
{
	Renderer::Renderer(RHI* rhi)
	{
		_opaquePass = new OpaquePass(rhi);
	}

	void Renderer::Draw(RHI* rhi, Swapchain* swapchain, RenderScene* scene, RenderCamera* camera)
	{
		auto index = rhi->GetCurrentFrameIndex(swapchain);
		auto backbuffer = rhi->GetBackbuffer(swapchain, index);

		
	}
}
