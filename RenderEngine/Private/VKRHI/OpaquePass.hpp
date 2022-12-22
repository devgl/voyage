#pragma once

#include "RenderPass.hpp"

namespace voyage
{
	class OpaquePass : public RenderPass
	{
	public:
		OpaquePass(RHI* rhi);
		void Draw(RHI* rhi, vk::CommandBuffer cmd);

	private:
		vk::RenderPass _pass;
	};
}