#pragma once

#include <vector>	

namespace voyage
{
	class RENDERENGINE_API Renderable
	{

	};

	class RENDERENGINE_API RenderCamera
	{

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
		RenderEngine();
		~RenderEngine();

		RenderCamera* CreateRenderCamera();
		void DestoryRenderCamera(RenderCamera* rendercamera);

		Renderable* CreateRendereable();
		void DestroyRenderable(Renderable* renderable);

		RenderScene* GetRenderScene() { return _renderScene; }

	private:
		std::vector<RenderCamera*> _renderCameras;
		RenderScene* _renderScene{nullptr};
		class RHI* _rhi{nullptr};
	};
}