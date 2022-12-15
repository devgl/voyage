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

	RenderEngine::RenderEngine()
	{
		_renderScene = new RenderScene();
		_rhi = new RHI();
	}

	RenderEngine::~RenderEngine()
	{
		delete _rhi;
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

}