#include "Application.hpp"


namespace voyage
{
	Application* Application::g_app = nullptr;

	Application* Application::GetInstance()
	{
		return g_app;
	}

	Application::Application()
	{
		g_app = this;

		Window::Initialize();

		_window = Window::CreateRenderWindow();
		_window->OnCLose = [this]()
		{
			this->_running = false;
		};
		_window->Open(0, 0, 800, 600);
		_window->CenterInScreen();

		_renderEngine = new RenderEngine(_window);
	}

	Application::~Application()
	{
		delete _renderEngine;

		Window::DestroyRenderWindow(_window);
		Window::Uninitialize();
	}

	void Application::Run()
	{
		_running = true;
	}

	void Application::Stop()
	{
		_running = false;
	}

	bool Application::IsRunning() const 
	{
		return _running;
	}

	Window* Application::GetWindow() const
	{
		return _window;
	}

	RenderEngine* Application::GetRenderEngine() const
	{
		return _renderEngine;
	}

}