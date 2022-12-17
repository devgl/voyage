#include <Framework/Application.hpp>
#include <thread>
#include <chrono>

class Editor : public voyage::Application
{
public:
	void Run() override
	{
		voyage::Application::Run();

		_renderCamera = GetRenderEngine()->CreateRenderCamera();
		auto sleepDuration = std::chrono::milliseconds(10);
		while (IsRunning())
		{
			GetWindow()->Tick();
			GetRenderEngine()->Render(_renderCamera);
			std::this_thread::sleep_for(sleepDuration);
		}
	}

private:
	voyage::RenderCamera* _renderCamera{ nullptr };
};

int main()
{
	Editor editor;
	editor.Run();
	return 0;
}