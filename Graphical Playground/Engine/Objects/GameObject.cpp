#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

HRESULT GameObject::Initialise(std::shared_ptr<IObjectConfig> config)
{
	HRESULT hr;

	std::shared_ptr<GameObjectConfig> goObjectConfig = std::dynamic_pointer_cast<GameObjectConfig>(config);

	m_DX11Mgr = goObjectConfig->m_DX11Mgr;
	m_VertBuffer = goObjectConfig->m_VertBuffer;
	m_IndexBuffer = goObjectConfig->m_IndexBuffer;

	// Create the buffer to send to the cbuffer in effect file
	D3D11_BUFFER_DESC cbbd;
	ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

	cbbd.Usage = D3D11_USAGE_DEFAULT;
	cbbd.ByteWidth = sizeof(GXLib::cbPerObject);
	cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbbd.CPUAccessFlags = 0;
	cbbd.MiscFlags = 0;

	hr = m_DX11Mgr->GetDevice()->CreateBuffer(&cbbd, NULL, &m_ConstantBuffer);
	if (FAILED(hr)) return hr;

	m_World = DirectX::XMMatrixIdentity();

	return hr;
}

void GameObject::Reset()
{

}

void GameObject::Destroy()
{
	m_ConstantBuffer->Release();
}

void GameObject::Update(DirectX::XMMATRIX camView, DirectX::XMMATRIX camProj) // TEMP cam argument?
{
	m_World = m_Scale * m_Rotation * m_Translation;

	m_CBObject.WVP = XMMatrixTranspose(m_World * camView * camProj);
	m_DX11Mgr->GetDeviceContext()->UpdateSubresource(m_ConstantBuffer, 0, NULL, &m_CBObject, 0, 0);
}

void GameObject::Draw()
{
	// Set the Vertex Buffer
	UINT stride = sizeof(GXLib::SimpleVertex);
	UINT offset = 0;
	m_DX11Mgr->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertBuffer, &stride, &offset);

	// Set the Index Buffer
	m_DX11Mgr->GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	m_DX11Mgr->GetDeviceContext()->VSSetConstantBuffers(0, 1, &m_ConstantBuffer);

	// Draw
	m_DX11Mgr->GetDeviceContext()->DrawIndexed(36, 0, 0); // TEMP hard coded index count
}