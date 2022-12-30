#include "Renderer.hpp"
#include "RenderEngine.hpp"
#include "OpaquePass.hpp"
#include "VulkanRHI.hpp"

namespace voyage
{
	Renderer::Renderer(RHI* rhi, Swapchain* swapchain)
	{
		auto frameCount = rhi->GetFrameCount(swapchain);
		_contexts.resize(frameCount);
		for (uint32_t i = 0; i < frameCount; i++)
		{
			_contexts[i] = new RenderContext();
		}

		_opaquePass = new OpaquePass();
		_opaquePass->Create(rhi, swapchain);
	}

	void Renderer::Draw(RHI* rhi, Swapchain* swapchain, RenderScene* scene, RenderCamera* camera)
	{
		auto index = rhi->GetCurrentFrameIndex(swapchain);
		auto backbuffer = rhi->GetBackbuffer(swapchain, index);
		auto renderContext = _contexts[index];

		vk::CommandBuffer cmd;
		_opaquePass->Execute(renderContext, cmd);
		
	}
}
