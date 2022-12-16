#pragma once

#include <Platform/Window.hpp>

namespace voyage
{
	class FRAMEWORK_API Application
	{
	public:
		Application();
		virtual ~Application();

	public:
		Window* GetWindow() const { return _window; }

	private:
		Window* _window;
	};
}