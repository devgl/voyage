#include <RenderSystem/VulkanRHI.h>

int main()
{
	auto rhi = new voyage::VulkanRHI();
	//auto swapchain = rhi->CreateSwapchain();
	//
	//if (rhi->NextFrameReady(swapchain))
	//{
	//	auto cmd = rhi->AllocateCommandBuffer();
	//
	//	vk::CommandBufferBeginInfo beginInfo{};
	//	cmd.begin(beginInfo);
	//	cmd.end();
	//
	//	//rhi->Submit();
	//	rhi->Present(swapchain);
	//}

	delete rhi;

	return 0;
}