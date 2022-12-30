#pragma once

#include <vector>

namespace voyage
{
	class RHI;
	class Swapchain;
	class RenderScene;
	class RenderCamera;
	class RenderContext;
	class RenderPass;

	class Renderer
	{
	public:
		Renderer(RHI* rhi, Swapchain* swapchain);
		virtual ~Renderer() = default;
		virtual void Draw(RHI* rhi, Swapchain* swapchain, RenderScene* scene, RenderCamera* camera);

	private:
		RenderPass* _opaquePass{ nullptr };
		std::vector<RenderContext*> _contexts;
	};
}