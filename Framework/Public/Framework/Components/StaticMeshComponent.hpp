#pragma once

#include "Component.hpp"
#include <RenderEngine/RenderEngine.hpp>

namespace voyage
{
	class FRAMEWORK_API StaticMeshComponent : public Component
	{
	public:
		StaticMeshComponent();
		~StaticMeshComponent();

	private:
		Renderable* _renderable;
	};
}