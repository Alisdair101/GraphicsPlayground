#include "DX11Manager.h"


DX11Manager::DX11Manager()
{
}

DX11Manager::~DX11Manager()
{
}

void DX11Manager::Reset()
{
	m_SwapChain = nullptr;
	m_D3D11Device = nullptr;
	m_D3D11DeviceContext = nullptr;
	m_RenderTargetView = nullptr;
}

void DX11Manager::Destroy()
{
	//Release the COM Objects we created
	m_SwapChain->Release();
	m_D3D11Device->Release();
	m_D3D11DeviceContext->Release();

	// TEMP
	depthStencilView->Release();
	depthStencilBuffer->Release();
}

HRESULT DX11Manager::Initialise(std::shared_ptr<ManagerConfig> config)
{
	HRESULT hr;
    std::shared_ptr<DX11ManagerConfig> dx11Config = std::dynamic_pointer_cast<DX11ManagerConfig>(config);

	InitialiseParameters(dx11Config);

	hr = InitialiseDirect3DApp(dx11Config);

	if (FAILED(hr))
    {
        return hr;
    }

    return S_OK;
}

void DX11Manager::InitialiseParameters(std::shared_ptr<DX11ManagerConfig> dx11Config)
{
	m_WindowMgr = dx11Config->m_WindowMgr;
}

HRESULT DX11Manager::InitialiseDirect3DApp(std::shared_ptr<DX11ManagerConfig> dx11Config)
{
    HRESULT hr;

	// TODO enable this for multiple monitors.

    // Describe our Buffer
    DXGI_MODE_DESC bufferDesc;

    ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

    bufferDesc.Width = m_WindowMgr->GetWindow(0)->GetWindowWidth();				// This is the Width of the resolution we are going to use. // TEMP
    bufferDesc.Height = m_WindowMgr->GetWindow(0)->GetWindowHeight();			// This is the Height of the resolution we are going to use. // TEMP
    bufferDesc.RefreshRate.Numerator = 60;										// Refresh rate of the monitor in hertz
    bufferDesc.RefreshRate.Denominator = 1;										// Refresh rate of the monitor in hertz
    bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;								// Describes the format of the display, describes the allocation of the colour channels.
    bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;			// Describes how the rasterizer will render onto the surface.
    bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;							// Describes how the image is stretched to fit the monitors resolution.

    // Describe our SwapChain
    DXGI_SWAP_CHAIN_DESC swapChainDesc;

    ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	
    swapChainDesc.BufferDesc = bufferDesc;										// DXGI_MODE_DESC structure which describes the back buffer.
    swapChainDesc.SampleDesc.Count = 1;											// DXGI_SAMPLE_DESC structure which describes the multisampling. Multisampling is used to "smooth" out the choppiness in lines and edges.
    swapChainDesc.SampleDesc.Quality = 0;										// DXGI_SAMPLE_DESC structure which describes the multisampling. Multisampling is used to "smooth" out the choppiness in lines and edges.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;				// DXGI_USAGE enum used to describe the access the CPU has to the surface of the back buffer. We specify DXGI_USAGE_RENDER_TARGET_OUTPUT since we will render to it.
    swapChainDesc.BufferCount = 1;												// Index for amount of back buffers. We are double buffering so set 1. Triple buffering when set to 2.
    swapChainDesc.OutputWindow = m_WindowMgr->GetWindow(0)->GetWindowHandle();	// Handle for output window.
    swapChainDesc.Windowed = TRUE;												// Windowd Mode.
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;						// DXGI_SWAP_EFFECT enum, describes what the display driver should do with the front buffer after swapping it to the back buffer.

    // Create our SwapChain
    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL, nullptr, NULL,
        D3D11_SDK_VERSION, &swapChainDesc, &m_SwapChain, &m_D3D11Device, nullptr, &m_D3D11DeviceContext);

	if (FAILED(hr))
	{
		return hr;
	}

    // Create our BackBuffer
    ID3D11Texture2D* BackBuffer;
    hr = m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	if (FAILED(hr))
	{
		return hr;
	}

    // Create our Render Target
    hr = m_D3D11Device->CreateRenderTargetView(BackBuffer, NULL, &m_RenderTargetView);
    BackBuffer->Release();

	if (FAILED(hr))
	{
		return hr;
	}
	
	// TEMP
	// Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = m_WindowMgr->GetWindow(0)->GetWindowWidth(); // TEMP
	depthStencilDesc.Height = m_WindowMgr->GetWindow(0)->GetWindowHeight(); // TEMP
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	ZeroMemory(&depthStencilBuffer, sizeof(depthStencilBuffer));

	hr = m_D3D11Device->CreateTexture2D(&depthStencilDesc, NULL, &depthStencilBuffer);

	if (FAILED(hr))
	{
		return hr;
	}

	// TEMP
	if (depthStencilBuffer != 0)
	{
		hr = m_D3D11Device->CreateDepthStencilView(depthStencilBuffer, NULL, &depthStencilView);

		if (FAILED(hr))
		{
			return hr;
		}
	}

    // Set our Render Target
	m_D3D11DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, depthStencilView);

	return hr;
}

void DX11Manager::ClearBackBuffer()
{
	// Clear our backbuffer to the updated color
	CONST FLOAT bgColor[4] = { 0.0f, 0.5f, 0.0f, 1.0f };
	m_D3D11DeviceContext->ClearRenderTargetView(m_RenderTargetView, bgColor);
}

void DX11Manager::ClearDepthBuffer()
{
	m_D3D11DeviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0); // TEMP
}

void DX11Manager::PresentBackBuffer()
{
	// Present the backbuffer to the screen
	m_SwapChain->Present(0, 0);
}