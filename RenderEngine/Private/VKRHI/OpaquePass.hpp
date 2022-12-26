#pragma once

#include "RenderPass.hpp"
#include <vector>

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

	private:
		RHI* _rhi;
		vk::RenderPass _pass;
		vk::Pipeline _pipeline;

		vk::DescriptorPool _descriptorPool;
		vk::DescriptorSet _descriptorSet;

		std::vector<vk::DescriptorSet> _perCameraDescriptorSets;
		std::vector<vk::DescriptorSetLayout> _descriptorSetLayouts;

		vk::PipelineLayout _pipelineLayout;


		vk::Buffer _indexBuffer;
		vk::Buffer _vertexBuffer;
		vk::DeviceSize _vertexBufferOffset;
	};
}