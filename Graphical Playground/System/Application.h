#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>
#include <memory>
#include <array>

// Project Dependencies
#include "../Engine/Managers/System Managers/WindowManager.h"

#include "../Engine/Base Interface/EngineModule.h"
#include "../Engine/Engine Modules/Graphics Module/Module/GraphicsEngineModule.h"

// TEMP?
enum ENG_MODULES
{
    ENG_GRAPHICS,

    ENG_MOD_MAX
};

class Application
{
public:
                                    Application();
                                    ~Application();

    void                            Reset();

    HRESULT                         Initialise(HINSTANCE hInstance, int nCmdShow);
    void                            Destroy();

    bool							CheckQuit();

	bool							Run();

    ////////// Member Variables //////////
    std::vector<std::shared_ptr<EngineModule>>   EngineModules;
};