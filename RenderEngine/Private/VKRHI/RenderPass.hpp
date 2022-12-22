#pragma once

#include "VulkanRHI.hpp"

namespace voyage
{
	class RenderPass
	{
	public:
		RenderPass(RHI* rhi);
		virtual void Draw();
	};
}