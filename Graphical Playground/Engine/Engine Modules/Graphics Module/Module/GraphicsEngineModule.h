#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>
#include <memory>

// Shaders
#include "../../../../Content/Shader Headers/SampleVertexShader.h"
#include "../../../../Content/Shader Headers/SamplePixelShader.h"

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
	HRESULT				InitialiseBasicObject();
	void				InitialiseBasicViewport();
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
	ID3D11Buffer* squareVertBuffer;
	ID3D11Buffer* squareIndexBuffer;
	ID3D11InputLayout* inputLayout;

	ID3D11DepthStencilView* depthStencilView;
	ID3D11Texture2D* depthStencilBuffer;
#pragma endregion

};