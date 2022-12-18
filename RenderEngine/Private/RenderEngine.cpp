#include "RenderEngine.hpp"
#include "VulkanRHI.hpp"

namespace voyage
{
	void RenderCamera::SetViewMatrix(const DirectX::XMFLOAT4X4& viewmatrix)
	{
		_viewMatrix = viewmatrix;
	}

	void RenderCamera::SetProjMatrix(const DirectX::XMFLOAT4X4& projmatrix)
	{
		_projMatrix = projmatrix;
	}

	void RenderScene::AttachRenderable(Renderable* renderable)
	{

	}

	void RenderScene::DetachRenderable(Renderable* renderable)
	{

	}

	RenderEngine::RenderEngine(Window* window)
	{
		_renderScene = new RenderScene();
		_rhi = new RHI();
		_window = window;

		auto wndSize = _window->GetSize();
		_swapchain = _rhi->CreateSwapchain(wndSize.x, wndSize.y, vk::Format::eR8G8B8A8Unorm, 3);
	}

	RenderEngine::~RenderEngine()
	{
		delete _renderScene;
		delete _rhi;
		_window = nullptr;
	}

	RenderCamera* RenderEngine::CreateRenderCamera()
	{
		return _renderCameras.emplace_back();
	}

	void RenderEngine::DestoryRenderCamera(RenderCamera* renderCamera)
	{
		_renderCameras.erase(std::remove_if(_renderCameras.begin(), _renderCameras.end(), [=](auto cam)->bool { return cam == renderCamera; }));
	}

	Renderable* RenderEngine::CreateRendereable()
	{
		return _renderables.emplace_back();
	}

	void RenderEngine::DestroyRenderable(Renderable* renderable)
	{
		_renderables.erase(std::remove_if(_renderables.begin(), _renderables.end(), [=](auto r) { return r == renderable; }));
	}

	void RenderEngine::Render(RenderCamera* camera)
	{
		
	}
}