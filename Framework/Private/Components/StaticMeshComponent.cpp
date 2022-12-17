#include "StaticMeshComponent.hpp"
#include "Application.hpp"

namespace voyage
{
	StaticMeshComponent::StaticMeshComponent()
	{
		_renderable = GetApp()->GetRenderEngine()->CreateRendereable();
	}

	StaticMeshComponent::~StaticMeshComponent()
	{
		GetApp()->GetRenderEngine()->DestroyRenderable(_renderable);
	}
}