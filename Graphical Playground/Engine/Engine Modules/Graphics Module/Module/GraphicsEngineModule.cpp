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
	//m_WindowMgr = nullptr;
	//m_DX11Mgr = nullptr;
}

void GraphicsEngineModule::Destroy()
{
	VS->Release();
	PS->Release();
	squareVertBuffer->Release();
	squareIndexBuffer->Release();
	inputLayout->Release();
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
	if (FAILED(hr)) { return hr; }

	std::shared_ptr<DX11ManagerConfig> dx11MgrConfig = std::make_shared<DX11ManagerConfig>();
	dx11MgrConfig->m_WindowMgr = m_WindowMgr;

    hr = m_DX11Mgr->Initialise(dx11MgrConfig);
	if (FAILED(hr)) { return hr; }

	// TEMP INIT  - Temporary stuff below here
	{
		hr = InitialiseBasicShader();
		if (FAILED(hr)) { return hr; }

		hr = InitialiseBasicObject();
		if (FAILED(hr)) { return hr; }

		InitialiseBasicViewport();
	}
    
    return hr;
}

HRESULT GraphicsEngineModule::InitialiseBasicShader()
{
	HRESULT hr;

	// SHADERS
	// Create the Shader Objects
	hr = m_DX11Mgr->GetDevice()->CreateVertexShader(SampleVertexShader, sizeof(SampleVertexShader), nullptr, &VS);
	if (FAILED(hr)) return hr;
	hr = m_DX11Mgr->GetDevice()->CreatePixelShader(SamplePixelShader, sizeof(SamplePixelShader), nullptr, &PS);
	if (FAILED(hr)) return hr;

	// Set Vertex and Pixel Shaders
	m_DX11Mgr->GetDeviceContext()->VSSetShader(VS, 0, 0);
	m_DX11Mgr->GetDeviceContext()->PSSetShader(PS, 0, 0);

	// INPUT LAYOUT
	// The input-layout description
	D3D11_INPUT_ELEMENT_DESC inputLayoutDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,	 D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOUR",	  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(inputLayoutDesc);

	// Create the Input Layout
	hr = m_DX11Mgr->GetDevice()->CreateInputLayout(inputLayoutDesc, numElements, SampleVertexShader, sizeof(SampleVertexShader), &inputLayout);
	if (FAILED(hr)) return hr;

	// Set the Input Layout
	m_DX11Mgr->GetDeviceContext()->IASetInputLayout(inputLayout);


	// Set Primitive Topology
	m_DX11Mgr->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return hr;
}

HRESULT GraphicsEngineModule::InitialiseBasicObject()
{
	HRESULT hr;

	// BUFFERS
	// Create the vertex buffer
	GXLib::SimpleVertex squareVertices[] =
	{
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-0.5f,  0.5f, 0.5f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(0.5f,  0.5f, 0.5f),  DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f),  DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f))
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(GXLib::SimpleVertex) * 4;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = squareVertices;
	hr = m_DX11Mgr->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &squareVertBuffer);
	if (FAILED(hr)) return hr;


	// Create the Index Buffer
	DWORD indices[] =
	{
		0, 1, 2,
		0, 2, 3,
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 2 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	hr = m_DX11Mgr->GetDevice()->CreateBuffer(&indexBufferDesc, &iinitData, &squareIndexBuffer);
	if (FAILED(hr)) return hr;

	// Set the Vertex Buffer
	UINT stride = sizeof(GXLib::SimpleVertex);
	UINT offset = 0;
	m_DX11Mgr->GetDeviceContext()->IASetVertexBuffers(0, 1, &squareVertBuffer, &stride, &offset);

	// Set the Index Buffer
	m_DX11Mgr->GetDeviceContext()->IASetIndexBuffer(squareIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	return hr;
}

void GraphicsEngineModule::InitialiseBasicViewport()
{
	// VIEWPORT
	// Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_WindowMgr->GetWindow(0)->GetWindowWidth();
	viewport.Height = m_WindowMgr->GetWindow(0)->GetWindowHeight();

	// Set the Viewport
	m_DX11Mgr->GetDeviceContext()->RSSetViewports(1, &viewport);
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
	m_DX11Mgr->GetDeviceContext()->DrawIndexed(6, 0, 0);

	// Present the backbuffer to the screen
	m_DX11Mgr->PresentBackBuffer();
}