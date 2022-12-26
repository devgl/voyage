#include "OpaquePass.hpp"

namespace voyage
{

	OpaquePass::OpaquePass(RHI* rhi)
		: RenderPass(rhi)
		, _rhi(rhi)
	{
		//_CreateDescriptorSet(rhi, )

		vk::PipelineLayoutCreateInfo info{};
		info.setSetLayouts(_descriptorSetLayouts);
		_pipelineLayout = rhi->device.createPipelineLayout(info);

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

		// multi-sample
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
		uint32_t currentFrameIndex = 0;

		cmd.bindPipeline(vk::PipelineBindPoint::eGraphics, _pipeline);
		cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipelineLayout, 0, 1, _perCameraDescriptorSets.data() + currentFrameIndex, 0, nullptr);

		vk::Rect2D scissorRect = renderContext->GetFullScreenScissorRect();
		cmd.setScissor(0, 1, &scissorRect);

		vk::Viewport viewport = renderContext->GetFullScreenViewport();
		cmd.setViewport(0, 1, &viewport);

		vk::RenderingInfo info{};
		cmd.beginRendering(info);

		cmd.bindVertexBuffers(0, 1, &_vertexBuffer, &_vertexBufferOffset);
		cmd.bindIndexBuffer(_indexBuffer, 0, vk::IndexType::eUint16);
		vk::DescriptorSet _perObjectDescriptorSet;
		cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, _pipelineLayout, 1, 1, &_perObjectDescriptorSet, 0, nullptr);
		cmd.drawIndexed(3, 1, 0, 0, 0);

		cmd.endRendering();
	}

	void OpaquePass::_CreateDescriptorSet(RHI* rhi, Swapchain* swapchain)
	{
		_descriptorSetLayouts.resize(2);

		{
			// per camera
			vk::DescriptorSetLayoutBinding bindings[1]{};
			bindings[0].binding = 0;
			bindings[0].descriptorType = vk::DescriptorType::eUniformBuffer;
			bindings[0].descriptorCount = 1;
			bindings[0].stageFlags = vk::ShaderStageFlagBits::eVertex;

			vk::DescriptorSetLayoutCreateInfo info{};
			info.setBindings(bindings);
			_descriptorSetLayouts[0] = rhi->device.createDescriptorSetLayout(info);

			vk::DescriptorPoolSize poolSize{};
			poolSize.descriptorCount = 3;
			poolSize.type = vk::DescriptorType::eUniformBuffer;
			vk::DescriptorPoolCreateInfo poolInfo{};
			poolInfo.maxSets = 3;
			poolInfo.poolSizeCount = 1;
			poolInfo.pPoolSizes = &poolSize;
			_descriptorPool = rhi->device.createDescriptorPool(poolInfo);

			_perCameraDescriptorSets.resize(3);
			vk::DescriptorSetAllocateInfo allocInfo{};
			allocInfo.descriptorPool = _descriptorPool;
			allocInfo.descriptorSetCount = 3;
			allocInfo.pSetLayouts = _descriptorSetLayouts.data() + 1;
			vk::resultCheck(rhi->device.allocateDescriptorSets(&allocInfo, _perCameraDescriptorSets.data()), "failed to allocate descriptor sets");
		}

		{
			// per object
			vk::DescriptorSetLayoutBinding bindings[1]{};
			bindings[0].binding = 0;
			bindings[0].descriptorType = vk::DescriptorType::eUniformBuffer;
			bindings[0].descriptorCount = 1;
			bindings[0].stageFlags = vk::ShaderStageFlagBits::eVertex;

			vk::DescriptorSetLayoutCreateInfo info{};
			info.setBindings(bindings);
			_descriptorSetLayouts[1] = rhi->device.createDescriptorSetLayout(info);

			vk::WriteDescriptorSet write;
			write.descriptorType = vk::DescriptorType::eUniformBuffer;
			write.descriptorCount = 1;

			vk::DescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = _vertexBuffer;
			bufferInfo.offset = 0;
			bufferInfo.range = 1024;
			write.pBufferInfo = &bufferInfo;
			rhi->device.updateDescriptorSets(1, &write, 0, nullptr);
		}
		

		/*
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
		allocInfo.pSetLayouts = _descriptorSetLayouts.data();

		std::vector<vk::DescriptorSet> descriptorSets;
		descriptorSets.resize(frameCount);
		vk::resultCheck(rhi->device.allocateDescriptorSets(&allocInfo, descriptorSets.data()), "failed to allocate descriptor sets");
		*/
	}

}