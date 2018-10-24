////////// Dependencies //////////
// Library Dependencies
#include <windows.h>

// Project Dependencies
#include "../System/Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    std::unique_ptr<Application> App = std::make_unique<Application>();

    if (FAILED(App->Initialise(hInstance, nCmdShow)))
    {
        return -1;
    }

    while (!App->CheckQuit())
    {
		App->Run();
    }

    MSG msg = { 0 };
    PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

    return (int)msg.wParam;
}