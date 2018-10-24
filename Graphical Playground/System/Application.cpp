#include "Application.h"


Application::Application()
{
    Reset();
}

Application::~Application()
{
    Destroy();
}

void Application::Reset()
{
	EngineModules.clear();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
    // Allocate memory for Engine Modules
    EngineModules.push_back(std::make_shared<GraphicsEngineModule>());

    // Initialise Engine Modules
    std::shared_ptr<GraphicsEngineModuleConfig> geModuleConfig = std::make_shared<GraphicsEngineModuleConfig>();
    geModuleConfig->WndHandleInstance = hInstance;
    geModuleConfig->nCmdShow = nCmdShow;

	EngineModules.at(ENG_GRAPHICS)->Initialise(geModuleConfig);

    return S_OK;
}

void Application::Destroy()
{

}

bool Application::CheckQuit()
{
    // Main message loop
    MSG msg = { 0 };

    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        bool handled = false;

		if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
		{
			//handled = HandleKeyboard(msg, timeStep); // TODO
		}
        else if (WM_QUIT == msg.message)
        {
            return true;
        }

        if (!handled)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return false;
}

bool Application::Run()
{
	for each (std::shared_ptr<EngineModule> engineModule in EngineModules)
	{
		engineModule->Run();
	}

	return true;
}