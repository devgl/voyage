#pragma once

namespace voyage
{
	class Renderer
	{
	public:
		Renderer(class RHI* rhi);
		virtual ~Renderer() = default;
		virtual void Draw(class RHI* rhi, class Swapchain* swapchain, class RenderScene* scene, class RenderCamera* camera);

	private:
		class OpaquePass* _opaquePass{ nullptr };
	};
}