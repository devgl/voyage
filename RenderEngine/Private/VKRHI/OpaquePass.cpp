#include "OpaquePass.hpp"

namespace voyage
{

	OpaquePass::OpaquePass(RHI* rhi)
		: RenderPass(rhi)
	{
		
	}


	void OpaquePass::Draw(RHI* rhi, vk::CommandBuffer cmd)
	{
		vk::RenderPassBeginInfo info{};
		//cmd.beginRenderPass(&info, nullptr);



		cmd.endRenderPass();
	}
}