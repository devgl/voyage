#pragma once

#include <RenderEngine/RenderEngine.hpp>
#include <Platform/Window.hpp>

namespace voyage
{
	struct Framework
	{
		static void Initialize(Window* window);
		static void Uninitialize();

		static RenderEngine* renderEngine;
	};
}