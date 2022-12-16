#pragma once

#include <cstdint>

namespace voyage
{
	class PLATFORM_API Window
	{
	public:
		static Window* CreateRenderWindow();
		static void DestroyRenderWindow(Window* window);

	public:
		virtual ~Window() = default;
	protected:
		Window();

	public:
		virtual void Open(int32_t x, int32_t y, int width, int height) = 0;
		virtual void CenterInScreen() = 0;
		virtual intptr_t GetUnderlyingHandle() = 0;
		virtual DirectX::XMINT2 GetSize() = 0;

	protected:
		virtual void OnResize(uint32_t width, uint32_t height);
		virtual void OnMove(int32_t x, int32_t y);
		virtual void OnClose();
	};
}