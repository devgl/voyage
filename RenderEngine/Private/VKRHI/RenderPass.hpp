#pragma once

#include "VulkanRHI.hpp"

namespace voyage
{
	struct RenderContext
	{

	};

	class RenderPass
	{
	public:
		RenderPass(RHI* rhi);
		virtual ~RenderPass() = default;
		virtual void Draw(RenderContext* renderContext, vk::CommandBuffer cmd);
	};
}