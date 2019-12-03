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
	// TEMP
	VS->Release();
	PS->Release();
	inputLayout->Release();
}

HRESULT GraphicsEngineModule::Initialise(std::shared_ptr<EngineModuleConfig> config)
{
    HRESULT hr = S_OK;

    std::shared_ptr<GraphicsEngineModuleConfig> geModuleConfig = std::dynamic_pointer_cast<GraphicsEngineModuleConfig>(config);

    // Allocate Memory for Managers
	m_WindowMgr = std::make_shared<WindowManager>();
	m_DX11Mgr = std::make_shared<DX11Manager>();

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
		InitialiseBasicCamera();
	}
    
    return hr;
}

// TEMP
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

// TEMP
HRESULT GraphicsEngineModule::InitialiseBasicObject()
{
	HRESULT hr;

	// TEMP
	rot = 0.01f;

	// BUFFERS
	// Create the vertex buffer
	GXLib::SimpleVertex cubeVertices[] =
	{
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)),

		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f))
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(GXLib::SimpleVertex) * 8;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;

	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = cubeVertices;
	hr = m_DX11Mgr->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &cubeVertBuffer);
	if (FAILED(hr)) return hr;

	// Create the Index Buffer
	DWORD indices[] =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * 12 * 3;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	hr = m_DX11Mgr->GetDevice()->CreateBuffer(&indexBufferDesc, &iinitData, &cubeIndexBuffer);
	if (FAILED(hr)) return hr;

	std::shared_ptr<GameObjectConfig> goConfig = std::make_shared<GameObjectConfig>();
	goConfig->m_DX11Mgr = m_DX11Mgr;
	goConfig->m_VertBuffer = cubeVertBuffer;
	goConfig->m_IndexBuffer = cubeIndexBuffer;

	cube1 = std::make_unique<GameObject>();
	cube1->Initialise(goConfig);
	cube2 = std::make_unique<GameObject>();
	cube2->Initialise(goConfig);

	return hr;
}

// TEMP
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

// TEMP
void GraphicsEngineModule::InitialiseBasicCamera()
{
	// Camera positon/at/up
	camPosition = DirectX::XMVectorSet(0.0f, 3.0f, -8.0f, 0.0f);
	camTarget = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	camUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// View/Projection/World matrix creation
	camView = DirectX::XMMatrixLookAtLH(camPosition, camTarget, camUp);
	camProjection = DirectX::XMMatrixPerspectiveFovLH(0.4f * 3.14f, (float)m_WindowMgr->GetWindow(0)->GetWindowWidth() / m_WindowMgr->GetWindow(0)->GetWindowHeight(), 1.0f, 1000.0f);
	World = DirectX::XMMatrixIdentity();
	WVP = World * camView * camProjection;
}

void GraphicsEngineModule::Run()
{
	Update();
	Draw();
}

void GraphicsEngineModule::Update()
{
	// TEST
	// Keep the cubes rotating
	rot += .0005f;
	if (rot > 6.28f)
		rot = 0.0f;


	// Define cube1's world space matrix
	DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	cube1->m_Scale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	cube1->m_Rotation = DirectX::XMMatrixRotationAxis(rotaxis, rot);
	cube1->m_Translation = DirectX::XMMatrixTranslation(0.0f, 0.0f, 4.0f);

	// Set cube1's world space using the transformations
	cube1->Update(camView, camProjection);


	// Define cube2's world space matrix
	cube2->m_Scale = DirectX::XMMatrixScaling(1.3f, 1.3f, 1.3f);
	cube2->m_Rotation = DirectX::XMMatrixRotationAxis(rotaxis, -rot);
	cube2->m_Translation = DirectX::XMMatrixTranslation(1.0f, 1.0f, 1.0f);

	// Update Cube 2
	cube2->Update(camView, camProjection);
}

void GraphicsEngineModule::Draw()
{
	// Clear our backbuffer to the updated color
	m_DX11Mgr->ClearBackBuffer();
	m_DX11Mgr->ClearDepthBuffer();
	
	// TEMP
	{
		// Draw the first cube
		cube1->Draw();

		// Draw the second cube
		cube2->Draw();
	}

	// Present the backbuffer to the screen
	m_DX11Mgr->PresentBackBuffer();
}