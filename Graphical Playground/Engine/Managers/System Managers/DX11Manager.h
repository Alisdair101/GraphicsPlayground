#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>
#include <d3dx11.h>
#include <d3dx10.h>

#include <DirectXMath.h>

// Project Dependencies
#include "../Base Interface/Manager.h"
#include "./WindowManager.h"

struct DX11ManagerConfig : ManagerConfig
{
    DX11ManagerConfig() {};
    virtual ~DX11ManagerConfig() {};

	std::shared_ptr<WindowManager> m_WindowMgr;
};

class DX11Manager : Manager
{
public:
	DX11Manager();
	~DX11Manager();

protected:
	void					Reset();
	void					Destroy();

	// Parent Functionality
public:
	virtual HRESULT			Initialise(std::shared_ptr<ManagerConfig> config);

	// Public Functionality
public:
	void					InitialiseParameters(std::shared_ptr<DX11ManagerConfig> dx11Config);

	// Get/Set
	ID3D11Device*			GetDevice() const { return m_D3D11Device; }
	ID3D11DeviceContext*	GetDeviceContext() const { return m_D3D11DeviceContext; }

	void					ClearBackBuffer();
	void					PresentBackBuffer();

    // Private Functionality
private:
    HRESULT					InitialiseDirect3DApp(std::shared_ptr<DX11ManagerConfig> dx11Config);

    ////////// Member Variables //////////
	std::shared_ptr<WindowManager> m_WindowMgr;

    IDXGISwapChain*			m_SwapChain;
    ID3D11Device*			m_D3D11Device;
    ID3D11DeviceContext*	m_D3D11DeviceContext;

	// TEMP!
    ID3D11RenderTargetView* m_RenderTargetView;
};

