#include "OpaquePass.hpp"

namespace voyage
{

	OpaquePass::OpaquePass(RHI* rhi)
		: RenderPass(rhi)
		, _rhi(rhi)
	{
		//_CreateDescriptorSet(rhi, )

		_CreatePipeline(rhi);

		// input assembly
		vk::PipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		inputAssemblyInfo.primitiveRestartEnable = false;
		inputAssemblyInfo.topology = vk::PrimitiveTopology::eTriangleList;

		// blend state
		vk::PipelineColorBlendStateCreateInfo blendStateInfo{};

		// depth stencil
		vk::PipelineDepthStencilStateCreateInfo depthStencilStateInfo{};

		// rasterization
		vk::PipelineRasterizationStateCreateInfo rasterStateInfo{};

		// muti-sample
		vk::PipelineMultisampleStateCreateInfo multisampleStateInfo{};

		// vertex input
		vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo{};

		// dynamic states
		vk::DynamicState dynamicStates[] = {
			vk::DynamicState::eScissor,
			vk::DynamicState::eViewport,
			vk::DynamicState::eBlendConstants, 
			vk::DynamicState::eStencilOp,
			vk::DynamicState::eStencilReference
		};
		vk::PipelineDynamicStateCreateInfo dynamicStateInfo{};
		dynamicStateInfo.setDynamicStates(dynamicStates);

		vk::GraphicsPipelineCreateInfo pipelineCreateInfo{};
		pipelineCreateInfo.layout = _pipelineLayout;
		pipelineCreateInfo.pInputAssemblyState = &inputAssemblyInfo;
		pipelineCreateInfo.pColorBlendState = &blendStateInfo;
		pipelineCreateInfo.pDepthStencilState = &depthStencilStateInfo;
		pipelineCreateInfo.pRasterizationState = &rasterStateInfo;
		pipelineCreateInfo.pMultisampleState = &multisampleStateInfo;
		pipelineCreateInfo.pVertexInputState = &vertexInputStateInfo;
		pipelineCreateInfo.pDynamicState = &dynamicStateInfo;
		
		auto [result, _pipeline] = rhi->device.createGraphicsPipeline(rhi->pipelineCache, pipelineCreateInfo);
		vk::resultCheck(result, "failed to create opaque render pass pipeline");
	}

	OpaquePass::~OpaquePass()
	{
		_rhi->device.destroyPipeline(_pipeline);
	}

	void OpaquePass::Draw(RenderContext* renderContext, vk::CommandBuffer cmd)
	{
		cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, _pipeline);
		cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipelineLayout, 0, 1, &_descriptorSet, 0, nullptr);


		vk::RenderingInfo info{};
		cmd.beginRendering(info);


		cmd.endRendering();
	}

	void OpaquePass::_CreateDescriptorSet(RHI* rhi, Swapchain* swapchain)
	{
		auto frameCount = rhi->GetFrameCount(swapchain);

		// set layout
		vk::DescriptorSetLayoutBinding binding{};
		binding.binding = 0;
		binding.descriptorType = vk::DescriptorType::eUniformBuffer;
		binding.descriptorCount = 1;
		binding.stageFlags = vk::ShaderStageFlagBits::eVertex;

		vk::DescriptorSetLayoutBinding bindings[2]{};
		bindings[0].binding = 0;
		bindings[0].descriptorType = vk::DescriptorType::eUniformBuffer;
		bindings[0].descriptorCount = 1;
		bindings[0].stageFlags = vk::ShaderStageFlagBits::eVertex;
		bindings[1].binding = 1;
		bindings[1].descriptorType = vk::DescriptorType::eSampledImage;
		bindings[1].descriptorCount = 8;
		bindings[1].stageFlags = vk::ShaderStageFlagBits::eFragment;

		vk::DescriptorSetLayoutCreateInfo info{};
		info.setBindings(bindings);
		_descriptorSetLayout = rhi->device.createDescriptorSetLayout(info);

		// pool
		vk::DescriptorPoolSize poolSizes[] = {
			{vk::DescriptorType::eUniformBuffer, 1},
			{vk::DescriptorType::eSampledImage, 8},
		};
		vk::DescriptorPoolCreateInfo poolInfo{};
		poolInfo.setPoolSizes(poolSizes);
		poolInfo.maxSets = frameCount;
		_descriptorPool = rhi->device.createDescriptorPool(poolInfo);

		// alloc sets
		vk::DescriptorSetAllocateInfo allocInfo{};
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = frameCount;
		allocInfo.pSetLayouts = &_descriptorSetLayout;

		std::vector<vk::DescriptorSet> descriptorSets;
		descriptorSets.resize(frameCount);
		vk::resultCheck(rhi->device.allocateDescriptorSets(&allocInfo, descriptorSets.data()), "failed to allocate descriptor sets");
	}

	void OpaquePass::_CreatePipeline(RHI* rhi)
	{
		vk::PipelineLayoutCreateInfo info{};
		info.setLayoutCount = 1;
		info.pSetLayouts = &_descriptorSetLayout;
		_pipelineLayout = rhi->device.createPipelineLayout(info);
	}

}