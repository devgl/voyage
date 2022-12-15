#include "CameraComponent.hpp"
#include "Framework.hpp"

namespace voyage
{
	CameraComponent::CameraComponent()
	{
		_renderCamera = Framework::renderEngine->CreateRenderCamera();
	}

	CameraComponent::~CameraComponent()
	{

	}
}