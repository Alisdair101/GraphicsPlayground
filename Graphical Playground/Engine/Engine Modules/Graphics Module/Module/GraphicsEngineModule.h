#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <d3dx11.h>
#include <memory>

// Local Library Dependencies
#include "../Library/GraphicsModuleLibrary.h"

// Project Dependencies
#include "../../../Base Interface/EngineModule.h"

#include "../../../Managers/System Managers/WindowManager.h"
#include "../../../Managers/System Managers/DX11Manager.h"

struct GraphicsEngineModuleConfig : EngineModuleConfig
{
    GraphicsEngineModuleConfig() {};
    virtual ~GraphicsEngineModuleConfig() {};

    HINSTANCE   WndHandleInstance;
    int         nCmdShow;
};

class GraphicsEngineModule : public EngineModule
{
public:
						GraphicsEngineModule();
    virtual				~GraphicsEngineModule();

protected:
    void                Reset();
    void                Destroy();

    // Parent Functionality
public:
    virtual HRESULT     Initialise(std::shared_ptr<EngineModuleConfig> config);

#pragma region TEMP FUNCTIONALITY
	HRESULT				InitialiseBasicShader();
#pragma endregion



	virtual void		Run();

    // Public Functionality
public:
	virtual void		Draw();

    // Private Functionality
private:
    std::shared_ptr<WindowManager> m_WindowMgr;
    std::unique_ptr<DX11Manager> m_DX11Mgr;

#pragma region TEMP VARIABLES
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11Buffer* triangleVertBuffer;
	ID3D11InputLayout* vertLayout;
#pragma endregion

};

