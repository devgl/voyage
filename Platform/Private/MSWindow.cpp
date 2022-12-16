#include "MSWindow.hpp"

namespace voyage
{
	Window* Window::CreateRenderWindow()
	{
		return new MSWindow();
	}

	void Window::DestroyRenderWindow(Window* window)
	{
		delete window;
	}

	MSWindow::MSWindow()
		: _hWnd(nullptr)
	{

	}

	void MSWindow::Open(int32_t x, int32_t y, int width, int height)
	{
	}

	void MSWindow::CenterInScreen()
	{

	}

	intptr_t MSWindow::GetUnderlyingHandle()
	{
		return reinterpret_cast<intptr_t>(_hWnd);
	}

	DirectX::XMINT2 MSWindow::GetSize()
	{
		return DirectX::XMINT2{ 0, 0 };
	}

	void MSWindow::OnResize(uint32_t width, uint32_t height)
	{
		Window::OnResize(width, height);
	}

	void MSWindow::OnMove(int32_t x, int32_t y)
	{
		Window::OnMove(x, y);
	}

	void MSWindow::OnClose()
	{
		Window::OnClose();
	}
}