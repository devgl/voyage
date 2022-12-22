#pragma once

#include "RenderPass.hpp"

namespace voyage
{
	class OpaquePass : public RenderPass
	{
	public:
		OpaquePass(RHI* rhi);
		void Draw(RenderContext* renderContext, vk::CommandBuffer cmd) override;

	private:
		vk::RenderPass _pass;
	};
}