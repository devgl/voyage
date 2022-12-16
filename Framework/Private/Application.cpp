#include "Application.hpp"
#include "Framework.hpp"

namespace voyage
{
	Application::Application()
	{
		_window = Window::CreateRenderWindow();
		Framework::Initialize(_window);
	}

	Application::~Application()
	{
		Framework::Uninitialize();
		Window::DestroyRenderWindow(_window);
	}
}