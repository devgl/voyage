#include "MSWindow.hpp"
#include <algorithm>

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

	const DWORD WSFullscreen = WS_OVERLAPPEDWINDOW & ~(WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
	const DWORD WSNormal = WS_OVERLAPPEDWINDOW;
	LPCTSTR WndClassName = TEXT("MSWindowClass");
	LRESULT DefaultWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		auto* const pWnd = reinterpret_cast<MSWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
		return pWnd->WindowProc(hWnd, message, wParam, lParam);
	}
	LRESULT InitWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		if (message == WM_NCCREATE)
		{
			auto pCreate = reinterpret_cast<LPCREATESTRUCT>(lParam);
			auto const pWnd = static_cast<MSWindow*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pWnd);
			SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)DefaultWindowProc);
			return DefaultWindowProc(hWnd, message, wParam, lParam);
		}
		return ::DefWindowProc(hWnd, message, wParam, lParam);
	}

	void Window::Initialize()
	{
		WNDCLASSEX wc = { 0 };
		wc.cbSize = sizeof(wc);
		wc.style = CS_OWNDC;
		wc.lpszClassName = WndClassName;
		wc.lpfnWndProc = &InitWindowProc;
		wc.hInstance = ::GetModuleHandle(NULL);
		::RegisterClassEx(&wc);
	}

	void Window::Uninitialize()
	{
		::UnregisterClass(WndClassName, ::GetModuleHandle(NULL));
	}

	void GetPrimaryMonitorRect(RECT& rect)
	{
		const POINT ptZero = { 0, 0 };
		HMONITOR hMonitor = ::MonitorFromPoint(ptZero, MONITOR_DEFAULTTOPRIMARY);
		MONITORINFOEX monitorInfo = {};
		monitorInfo.cbSize = sizeof(monitorInfo);
		::GetMonitorInfo(hMonitor, &monitorInfo);
		rect = monitorInfo.rcMonitor;
	}

	void GetAdjustedWindowRect(DWORD style, LONG width, LONG height, RECT& rect)
	{
		width = std::max<LONG>(80, width);
		height = std::max<LONG>(60, height);

		RECT monitorRect = { 0 };
		GetPrimaryMonitorRect(monitorRect);
		rect.left = (monitorRect.right - width) / 2;
		rect.top = (monitorRect.bottom - height) / 2;
		rect.right = rect.left + width;
		rect.bottom = rect.top + height;

		::AdjustWindowRect(&rect, style, FALSE);
	}

	MSWindow::MSWindow()
		: _hWnd(nullptr)
	{
	}

	MSWindow::~MSWindow()
	{
		if (_hWnd)
		{
			::DestroyWindow(_hWnd);
			_hWnd = nullptr;
		}
	}

	void MSWindow::Tick()
	{
		MSG msg;
		while (PeekMessage(&msg, _hWnd, 0, 0, PM_REMOVE) != 0)
		{
			DispatchMessage(&msg);
		}
	}

	void MSWindow::Open(int32_t x, int32_t y, int width, int height)
	{
		RECT wndRect{};
		GetAdjustedWindowRect(WSNormal, width, height, wndRect);

		width = wndRect.right - wndRect.left;
		height = wndRect.bottom - wndRect.top;

		HINSTANCE hInst = ::GetModuleHandle(NULL);
		_hWnd = CreateWindowEx(
			0,
			WndClassName,
			TEXT(""),
			WSNormal,
			x, y, width, height,
			nullptr, nullptr, hInst, this);
		::ShowWindow(_hWnd, SW_NORMAL);
	}

	void MSWindow::SetTitle(const std::string_view title)
	{
		::SetWindowText(_hWnd, title.data());
	}

	void MSWindow::CenterInScreen()
	{
		RECT rc;
		::GetWindowRect(_hWnd, &rc);

		int xPos = (::GetSystemMetrics(SM_CXSCREEN) - rc.right) / 2;
		int yPos = (::GetSystemMetrics(SM_CYSCREEN) - rc.bottom) / 2;
		::SetWindowPos(_hWnd, NULL, xPos, yPos, rc.right - rc.left, rc.bottom - rc.top, 0);
	}

	intptr_t MSWindow::GetUnderlyingHandle()
	{
		return reinterpret_cast<intptr_t>(_hWnd);
	}

	DirectX::XMINT2 MSWindow::GetSize()
	{
		return DirectX::XMINT2{ 0, 0 };
	}

	void MSWindow::_OnResize(uint32_t width, uint32_t height)
	{
		Window::_OnResize(width, height);
	}

	void MSWindow::_OnMove(int32_t x, int32_t y)
	{
		Window::_OnMove(x, y);
	}

	void MSWindow::_OnClose()
	{
		Window::_OnClose();
	}

	LRESULT MSWindow::WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CLOSE:
			_OnClose();
			break;
		case WM_SIZE:
			_OnResize(LOWORD(lParam), HIWORD(lParam));
			break;
		case WM_MOVE:
			_OnMove((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
			break;
		}

		return ::DefWindowProc(hwnd, message, wParam, lParam);
	}

}