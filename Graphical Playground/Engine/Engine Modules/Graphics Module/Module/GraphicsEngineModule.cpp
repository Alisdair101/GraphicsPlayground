#include "GraphicsEngineModule.h"


GraphicsEngineModule::GraphicsEngineModule()
{
    Reset();
}

GraphicsEngineModule::~GraphicsEngineModule()
{
    Destroy();
    Reset();
}

void GraphicsEngineModule::Reset()
{
	m_WindowMgr = nullptr;
	m_DX11Mgr = nullptr;
}

void GraphicsEngineModule::Destroy()
{

}

HRESULT GraphicsEngineModule::Initialise(std::shared_ptr<EngineModuleConfig> config)
{
    HRESULT hr = S_OK;

    std::shared_ptr<GraphicsEngineModuleConfig> geModuleConfig = std::dynamic_pointer_cast<GraphicsEngineModuleConfig>(config);

    // Allocate Memory for Managers
	m_WindowMgr = std::make_shared<WindowManager>();
	m_DX11Mgr = std::make_unique<DX11Manager>();

    // Being setup of Managers
    // WindowMgr Config
    std::shared_ptr<WindowManagerConfig> wndMgrConfig = std::make_shared<WindowManagerConfig>();
    wndMgrConfig->m_WndHandleInstance = geModuleConfig->WndHandleInstance;
    wndMgrConfig->m_ShowWindow = geModuleConfig->nCmdShow;

    hr = m_WindowMgr->Initialise(wndMgrConfig);

    if (FAILED(hr))
    {
        return hr;
    }

	std::shared_ptr<DX11ManagerConfig> dx11MgrConfig = std::make_shared<DX11ManagerConfig>();
	dx11MgrConfig->m_WindowMgr = m_WindowMgr;

    hr = m_DX11Mgr->Initialise(dx11MgrConfig);

    if (FAILED(hr))
    {
        return hr;
    }

	// TEMP INIT  - Temporary stuff below here
	hr = InitialiseBasicShader();

	if (FAILED(hr))
	{
		return hr;
	}
    
    return hr;
}

HRESULT GraphicsEngineModule::InitialiseBasicShader()
{
	// The input-layout description
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		/*{ "COLOUR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }*/
	};

	UINT numElements = ARRAYSIZE(layout);

	HRESULT hr;

	// Compile Shaders from shader file
	ID3DBlob* VS_Buffer;
	ID3DBlob* PS_Buffer;
	hr = GraphicsModuleLibrary::CompileShaderFromFile(L"Effects.fx", "VS", "vs_4_0", &VS_Buffer);
	hr = GraphicsModuleLibrary::CompileShaderFromFile(L"Effects.fx", "PS", "ps_4_0", &PS_Buffer);

	// Create the Shader Objects
	hr = m_DX11Mgr->GetDevice()->CreateVertexShader(VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), NULL, &VS);
	hr = m_DX11Mgr->GetDevice()->CreatePixelShader(PS_Buffer->GetBufferPointer(), PS_Buffer->GetBufferSize(), NULL, &PS);

	// Set Vertex and Pixel Shaders
	m_DX11Mgr->GetDeviceContext()->VSSetShader(VS, 0, 0);
	m_DX11Mgr->GetDeviceContext()->PSSetShader(PS, 0, 0);

	// Create the vertex buffer
	GraphicsModuleLibrary::SimpleVertex v[] =
	{
		GraphicsModuleLibrary::SimpleVertex(0.0f, 0.5f, 0.5f),
		GraphicsModuleLibrary::SimpleVertex(0.5f, -0.5f, 0.5f),
		GraphicsModuleLibrary::SimpleVertex(-0.5f, -0.5f, 0.5f),
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(GraphicsModuleLibrary::SimpleVertex) * 3;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = v;
	hr = m_DX11Mgr->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &triangleVertBuffer);

	// Set the vertex buffer
	UINT stride = sizeof(GraphicsModuleLibrary::SimpleVertex);
	UINT offset = 0;
	m_DX11Mgr->GetDeviceContext()->IASetVertexBuffers(0, 1, &triangleVertBuffer, &stride, &offset);

	// Create the Input Layout
	m_DX11Mgr->GetDevice()->CreateInputLayout(layout, numElements, VS_Buffer->GetBufferPointer(), VS_Buffer->GetBufferSize(), &vertLayout);

	// Set Primitive Topology
	m_DX11Mgr->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_WindowMgr->GetWindow(0)->GetWindowWidth();
	viewport.Height = m_WindowMgr->GetWindow(0)->GetWindowHeight();

	// Set the Viewport
	m_DX11Mgr->GetDeviceContext()->RSSetViewports(1, &viewport);

	return hr;
}

void GraphicsEngineModule::Run()
{
	Draw();
}

void GraphicsEngineModule::Draw()
{
	// Clear our backbuffer to the updated color
	m_DX11Mgr->ClearBackBuffer();

	// Draw the Triangle
	m_DX11Mgr->GetDeviceContext()->Draw(3, 0);

	// Present the backbuffer to the screen
	m_DX11Mgr->PresentBackBuffer();
}