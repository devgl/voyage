#include "Framework.hpp"


namespace voyage
{
	RenderEngine* Framework::renderEngine = nullptr;

	void Framework::Initialize(Window* window)
	{
		renderEngine = new RenderEngine(window);
	}

	void Framework::Uninitialize()
	{
		delete renderEngine;
	}
}