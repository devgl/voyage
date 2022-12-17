#pragma once

#include <Platform/Window.hpp>
#include <RenderEngine/RenderEngine.hpp>

namespace voyage
{
	class FRAMEWORK_API Application
	{
	private:
		static Application* g_app;
	public:
		static Application* GetInstance();

	public:
		Application();
		virtual ~Application();
		virtual void Run();
		virtual void Stop();
		bool IsRunning() const;
		Window* GetWindow() const;
		RenderEngine* GetRenderEngine() const;

	private:
		bool _running{ true };
		Window* _window;
		RenderEngine* _renderEngine;
	};
}

#define GetApp() voyage::Application::GetInstance()