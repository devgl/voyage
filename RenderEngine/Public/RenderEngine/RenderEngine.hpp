#pragma once

#include <vector>	
#include <Platform/Window.hpp>

namespace voyage
{
	class RENDERENGINE_API Renderable
	{

	};

	class RENDERENGINE_API RenderCamera
	{
	public:
		void SetViewMatrix(const DirectX::XMFLOAT4X4& viewmatrix);
		void SetProjMatrix(const DirectX::XMFLOAT4X4& projmatrix);

	private:
		DirectX::XMFLOAT4X4 _viewMatrix;
		DirectX::XMFLOAT4X4 _projMatrix;
	};

	class RENDERENGINE_API RenderScene
	{
	public:
		void AttachRenderable(Renderable* renderable);
		void DetachRenderable(Renderable* renderable);

	private:
	};

	class RENDERENGINE_API RenderEngine
	{
	public:
		RenderEngine(Window* window);
		~RenderEngine();

		RenderCamera* CreateRenderCamera();
		void DestoryRenderCamera(RenderCamera* rendercamera);

		Renderable* CreateRendereable();
		void DestroyRenderable(Renderable* renderable);

		RenderScene* GetRenderScene() { return _renderScene; }

		void Render(RenderCamera* camera);

	private:
		std::vector<RenderCamera*> _renderCameras;
		RenderScene* _renderScene{ nullptr };
		Window* _window{ nullptr };
		class RHI* _rhi{ nullptr };
	};
}