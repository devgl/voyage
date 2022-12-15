#include "Framework.hpp"

namespace voyage
{
	RenderEngine* Framework::renderEngine = nullptr;

	void Framework::Initialize()
	{
		renderEngine = new RenderEngine();
	}

	void Framework::Uninitialize()
	{
		delete renderEngine;
	}
}