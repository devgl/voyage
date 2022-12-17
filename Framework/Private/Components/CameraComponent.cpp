#include "CameraComponent.hpp"
#include "Application.hpp"

namespace voyage
{
	CameraComponent::CameraComponent()
	{
		_renderCamera = GetApp()->GetRenderEngine()->CreateRenderCamera();
	}

	CameraComponent::~CameraComponent()
	{

	}
}