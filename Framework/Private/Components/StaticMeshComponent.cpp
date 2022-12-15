#include "StaticMeshComponent.hpp"
#include "Framework.hpp"

namespace voyage
{
	StaticMeshComponent::StaticMeshComponent()
	{
		_renderable = Framework::renderEngine->CreateRendereable();
	}

	StaticMeshComponent::~StaticMeshComponent()
	{
		Framework::renderEngine->DestroyRenderable(_renderable);
	}
}