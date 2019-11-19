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
#pragma endregion



	virtual void		Run();

	// Public Functionality
public:
	virtual void		Update();
	virtual void		Draw();

	// Private Functionality
private:
	std::shared_ptr<WindowManager> m_WindowMgr;
	std::unique_ptr<DX11Manager> m_DX11Mgr;

#pragma region TEMP VARIABLES
	ID3D11VertexShader* VS;
	ID3D11PixelShader* PS;
	ID3D11Buffer* cubeVertBuffer;
	ID3D11Buffer* cubeIndexBuffer;
	ID3D11InputLayout* inputLayout;

	DirectX::XMMATRIX cube1World;
	DirectX::XMMATRIX cube2World;

	DirectX::XMMATRIX Rotation;
	DirectX::XMMATRIX Scale;
	DirectX::XMMATRIX Translation;
	float rot;

	ID3D11Buffer* cbPerObjectBuffer;
	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX World;
	DirectX::XMMATRIX camView;
	DirectX::XMMATRIX camProjection;

	DirectX::XMVECTOR camPosition;
	DirectX::XMVECTOR camTarget;
	DirectX::XMVECTOR camUp;

	struct cbPerObject
	{
		DirectX::XMMATRIX  WVP;
	};

	cbPerObject cbPerObj;
#pragma endregion

};