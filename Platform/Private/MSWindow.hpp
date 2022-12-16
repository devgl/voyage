#pragma once

#include "Window.hpp"
#include <windows.h>

namespace voyage
{
	class MSWindow : public Window
	{
	public:
		MSWindow();
		void Open(int32_t x, int32_t y, int width, int height) override;
		void CenterInScreen() override;
		intptr_t GetUnderlyingHandle() override;
		DirectX::XMINT2 GetSize() override;

	protected:
		void OnResize(uint32_t width, uint32_t height) override;
		void OnMove(int32_t x, int32_t y) override;
		void OnClose() override;

	private:
		HWND _hWnd;
	};
}