#pragma once

#include <RenderEngine/RenderEngine.hpp>

namespace voyage
{
	struct Framework
	{
		static void Initialize();
		static void Uninitialize();

		static RenderEngine* renderEngine;
	};
}