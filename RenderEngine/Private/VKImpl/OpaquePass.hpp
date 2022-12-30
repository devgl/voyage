#pragma once

#include "RenderPass.hpp"
#include <vector>

namespace voyage
{
	class OpaquePass : public RenderPass
	{
	public:
		void Create(RHI* rhi, Swapchain* swapchain);
		void Destroy(RHI* rhi);
		void Execute(RenderContext* renderContext, vk::CommandBuffer cmd) override;

	private:
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