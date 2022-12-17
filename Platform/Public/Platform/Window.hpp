#pragma once

#include <cstdint>
#include <string_view>
#include <functional>

namespace voyage
{
	class PLATFORM_API Window
	{
	public:
		static void Initialize();
		static void Uninitialize();
		static Window* CreateRenderWindow();
		static void DestroyRenderWindow(Window* window);

		virtual ~Window();
		virtual void Tick();
		virtual void Open(int32_t x, int32_t y, int width, int height) = 0;
		virtual void SetTitle(const std::string_view title) = 0;
		virtual void CenterInScreen() = 0;
		virtual intptr_t GetUnderlyingHandle() = 0;
		virtual DirectX::XMINT2 GetSize() = 0;

		std::function<void()> OnCLose;
		std::function<void(int32_t, int32_t)> OnMove;
		std::function<void(uint32_t, uint32_t)> OnResize;

	protected:
		Window();
		virtual void _OnResize(uint32_t width, uint32_t height);
		virtual void _OnMove(int32_t x, int32_t y);
		virtual void _OnClose();
	};
}