#include "Window.hpp"

namespace voyage
{
	Window::Window()
	{

	}

	Window::~Window()
	{

	}

	void Window::Tick()
	{

	}

	void Window::_OnResize(uint32_t width, uint32_t height)
	{
		if (OnResize)
		{
			OnResize(width, height);
		}
	}

	void Window::_OnMove(int32_t x, int32_t y)
	{
		if (OnMove)
		{
			OnMove(x, y);
		}
	}

	void Window::_OnClose()
	{
		if (OnCLose)
		{
			OnCLose();
		}
	}

}