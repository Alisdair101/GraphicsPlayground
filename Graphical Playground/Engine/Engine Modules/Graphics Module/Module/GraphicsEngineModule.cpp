#include "GraphicsEngineModule.h"

// Project Includes
#include "../Library/DDSTextureLoader.h"


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
}

void GraphicsEngineModule::Destroy()
{
	// TEMP
	VS->Release();
	PS->Release();
	inputLayout->Release();
	m_Texture->Release();
	m_SamplerState->Release();
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

		hr = InitialiseBasicTextures();
		if (FAILED(hr)) { return hr; }

		hr = InitialiseSamplerState();
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
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 }
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

	ID3D11Buffer* cubeVertBuffer;
	ID3D11Buffer* cubeIndexBuffer;

	// TEMP
	// BUFFERS
	// Create the vertex buffer
	GXLib::SimpleVertex cubeVertices[] =
	{
		// Front Face
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),

		// Back Face
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),

		// Top Face
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),

		// Bottom Face
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),

		// Left Face
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),

		// Right Face
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
		GXLib::SimpleVertex(DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT2(1.0f, 1.0f))
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(GXLib::SimpleVertex) * 24;
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
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
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

	MeshStoreManager::Get()->AddMeshToList("Cube", cubeVertBuffer, cubeIndexBuffer, 24, 36);

	std::shared_ptr<GameObjectConfig> goConfig = std::make_shared<GameObjectConfig>();
	goConfig->m_DX11Mgr = m_DX11Mgr;
	goConfig->m_Mesh = MeshStoreManager::Get()->GetMeshStore("Cube");

	cube1 = std::make_unique<GameObject>();
	cube1->Initialise(goConfig);
	cube2 = std::make_unique<GameObject>();
	cube2->Initialise(goConfig);

	rot = 0.01f;

	return hr;
}

// TEMP
HRESULT GraphicsEngineModule::InitialiseBasicTextures()
{
	HRESULT hr;
	
	hr = DirectX::CreateDDSTextureFromFile(m_DX11Mgr->GetDevice(), L"Content\\Textures\\placeholder.dds", 0, &m_Texture);
	if (FAILED(hr)) return hr;

	m_DX11Mgr->GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

	return hr;
}
 // TEMP
HRESULT GraphicsEngineModule::InitialiseSamplerState()
{
	HRESULT hr;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = m_DX11Mgr->GetDevice()->CreateSamplerState(&sampDesc, &m_SamplerState);
	if (FAILED(hr)) return hr;

	m_DX11Mgr->GetDeviceContext()->PSSetSamplers(0, 1, &m_SamplerState);

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
	std::shared_ptr<CameraConfig> camConfig = std::make_shared<CameraConfig>();
	camConfig->m_WindowMgr = m_WindowMgr;
	camConfig->m_Position = DirectX::XMFLOAT4(0.0f, 3.0f, -8.0f, 0.0f);
	camConfig->m_LookAt = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	camConfig->m_Up = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	m_MainCamera = std::make_shared<Camera>();
	m_MainCamera->Initialise(camConfig);

	// View/Projection/World matrix creation
	World = DirectX::XMMatrixIdentity();
	WVP = World * m_MainCamera->GetView() * m_MainCamera->GetProjection();
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
	cube1->Update(m_MainCamera->GetView(), m_MainCamera->GetProjection());


	// Define cube2's world space matrix
	cube2->m_Scale = DirectX::XMMatrixScaling(1.3f, 1.3f, 1.3f);
	cube2->m_Rotation = DirectX::XMMatrixRotationAxis(rotaxis, -rot);
	cube2->m_Translation = DirectX::XMMatrixTranslation(1.0f, 1.0f, 1.0f);

	// Update Cube 2
	cube2->Update(m_MainCamera->GetView(), m_MainCamera->GetProjection());
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