#pragma once

#include "Window.hpp"
#include <windows.h>

namespace voyage
{
	class MSWindow : public Window
	{
	public:
		MSWindow();
		~MSWindow() override;
		void Tick() override;
		void Open(int32_t x, int32_t y, int width, int height) override;
		void SetTitle(const std::string_view title) override;
		void CenterInScreen() override;
		intptr_t GetUnderlyingHandle() override;
		DirectX::XMINT2 GetSize() override;

	protected:
		void _OnResize(uint32_t width, uint32_t height) override;
		void _OnMove(int32_t x, int32_t y) override;
		void _OnClose() override;

	public:
		LRESULT WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND _hWnd;
	};
}