#include "Application.hpp"
#include "Framework.hpp"

namespace voyage
{
	Application::Application()
	{
		Framework::Initialize();
	}

	Application::~Application()
	{
		Framework::Uninitialize();
	}
}