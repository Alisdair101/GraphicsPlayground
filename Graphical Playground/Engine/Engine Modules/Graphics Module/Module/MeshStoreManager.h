#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <unordered_map>
#include <string>
#include <windows.h>
#include <memory>
#include <DirectXMath.h>
#include <d3d11_4.h>

// Project Dependencies
#include "../Library/GraphicsModuleLibrary.h"

class MeshStoreManager
{
public:
	static MeshStoreManager* Get();

	void AddMeshToList(const std::string& filePath, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, const unsigned int vertexCount, const unsigned int indexCount);
	std::shared_ptr<GXLib::MeshStore> GetMeshStore(const std::string& filePath);

private:
	static MeshStoreManager* instance;

	MeshStoreManager();

	std::unordered_map<std::string, std::shared_ptr<GXLib::MeshStore>> m_MeshList;
};