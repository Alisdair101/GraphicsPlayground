#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>
#include <memory>
#include <DirectXMath.h>

// Shaders
#include "../../../../Content/Shader Headers/SampleVertexShader.h"
#include "../../../../Content/Shader Headers/SamplePixelShader.h"

// Local Library Dependencies
#include "../Library/GraphicsModuleLibrary.h"

// Project Dependencies
#include "../../../Base Interface/EngineModule.h"

#include "../../../Managers/System Managers/WindowManager.h"
#include "../../../Managers/System Managers/DX11Manager.h"

#include "../../../Objects/GameObject.h"
#include "../../../Objects/Camera.h"

#include "MeshStoreManager.h"


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
	void				InitialiseBasicCamera();
	HRESULT				InitialiseBasicTextures();
	HRESULT				InitialiseSamplerState();
#pragma endregion

	virtual void		Run();

	// Public Functionality
public:
	virtual void		Update();
	virtual void		Draw();

	// Private Functionality
private:
	std::shared_ptr<WindowManager> m_WindowMgr;
	std::shared_ptr<DX11Manager> m_DX11Mgr;

#pragma region TEMP VARIABLES
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11InputLayout* inputLayout;
	
	std::shared_ptr<Camera> m_MainCamera;

	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX World;

	std::unique_ptr<GameObject> cube1;
	std::unique_ptr<GameObject> cube2;
	float rot;

	ID3D11ShaderResourceView* m_Texture;
	ID3D11SamplerState* m_SamplerState;
#pragma endregion
};