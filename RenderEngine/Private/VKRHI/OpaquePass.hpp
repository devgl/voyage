#pragma once

#include "RenderPass.hpp"

namespace voyage
{
	class OpaquePass : public RenderPass
	{
	public:
		OpaquePass(RHI* rhi);
		~OpaquePass() override;
		void Draw(RenderContext* renderContext, vk::CommandBuffer cmd) override;

	private:
		void _CreateDescriptorSet(RHI* rhi, Swapchain* swapchain);
		void _CreatePipeline(RHI* rhi);

	private:
		RHI* _rhi;
		vk::RenderPass _pass;
		vk::Pipeline _pipeline;

		vk::DescriptorPool _descriptorPool;
		vk::DescriptorSet _descriptorSet;

		vk::DescriptorSetLayout _descriptorSetLayout;
		vk::PipelineLayout _pipelineLayout;


	};
}