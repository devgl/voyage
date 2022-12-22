#include "OpaquePass.hpp"

namespace voyage
{

	OpaquePass::OpaquePass(RHI* rhi)
		: RenderPass(rhi)
	{
		
	}


	void OpaquePass::Draw(RenderContext* renderContext, vk::CommandBuffer cmd)
	{
		vk::RenderingInfo info{};
		cmd.beginRendering(info);
		cmd.endRendering();
	}
}