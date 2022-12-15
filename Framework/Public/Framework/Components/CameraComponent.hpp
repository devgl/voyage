#pragma once

#include "Component.hpp"
#include <RenderEngine/RenderEngine.hpp>

namespace voyage
{
	class FRAMEWORK_API CameraComponent : public Component
	{
	public:
		CameraComponent();
		~CameraComponent();

	private:
		RenderCamera* _renderCamera;
	};
}