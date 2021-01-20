#include "GLCore.h"
#include "GameLayer.h"
#include "Defines.h"
using namespace GLCore;

class App : public Application
{
public:
	App(const std::string& name, uint32_t width, uint32_t height)
		: Application(name, width, height)
	{
		PushLayer(new GameLayer());
	}
};

int main()
{
	std::unique_ptr<App> app = std::make_unique<App>("Flappy Bird", WIN_WIDTH, WIN_HEIGHT);
	app->Run();
}