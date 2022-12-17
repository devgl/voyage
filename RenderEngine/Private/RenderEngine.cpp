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
	}

	RenderEngine::~RenderEngine()
	{
		delete _renderScene;
		delete _rhi;
		_window = nullptr;
	}

	RenderCamera* RenderEngine::CreateRenderCamera()
	{
		

		return nullptr;
	}

	void RenderEngine::DestoryRenderCamera(RenderCamera*)
	{

	}

	Renderable* RenderEngine::CreateRendereable()
	{
		return nullptr;
	}

	void RenderEngine::DestroyRenderable(Renderable* renderable)
	{

	}

	void RenderEngine::Render(RenderCamera* camera)
	{

	}

}