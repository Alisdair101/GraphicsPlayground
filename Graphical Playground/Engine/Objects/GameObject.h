#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>
#include <memory>
#include <DirectXMath.h>
#include <C:\Program Files (x86)\Windows Kits\10\Include\10.0.15063.0\um\d3d11_4.h>

// Local Library Dependencies
#include "../../Engine/Engine Modules/Graphics Module/Library/GraphicsModuleLibrary.h"

// Project Dependencies
#include "IObject.h"
#include "../Managers/System Managers/DX11Manager.h"

struct GameObjectConfig : IObjectConfig
{
	GameObjectConfig() {};
	virtual ~GameObjectConfig() {};

	std::shared_ptr<DX11Manager> m_DX11Mgr;

	std::shared_ptr<GXLib::MeshStore> m_Mesh;
};


class GameObject : public IObject
{
public:
					GameObject();
	virtual			~GameObject();

protected:
	void                Reset();
	void                Destroy();

	// Parent Functionality
public:
	virtual HRESULT     Initialise(std::shared_ptr<IObjectConfig> config);

	// Public Functionality
public:
	virtual void		Update(DirectX::XMMATRIX camView, DirectX::XMMATRIX camProj); // TEMP cam argument?
	virtual void		Draw();

public:
	DirectX::XMMATRIX m_Scale;
	DirectX::XMMATRIX m_Rotation;
	DirectX::XMMATRIX m_Translation;

private:
	std::shared_ptr<DX11Manager> m_DX11Mgr;

	std::shared_ptr<GXLib::MeshStore> m_Mesh;
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;

	DirectX::XMMATRIX m_World;

	ID3D11Buffer* m_ConstantBuffer;
	GXLib::cbPerObject m_CBObject;

	// Texture
	ID3D11ShaderResourceView* m_Texture;
	ID3D11SamplerState* m_TexSamplerState;
};