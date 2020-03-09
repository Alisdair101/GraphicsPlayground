#include "MeshStoreManager.h"

// Singleton Instance
MeshStoreManager* MeshStoreManager::instance;

MeshStoreManager::MeshStoreManager()
{
	instance = nullptr;
}

MeshStoreManager* MeshStoreManager::Get()
{
	if (instance == nullptr)
	{ 
		instance = new MeshStoreManager();
	} 
	return instance;
}

void MeshStoreManager::AddMeshToList(const std::string& filePath, ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, const unsigned int vertexCount, const unsigned int indexCount)
{
	std::pair<std::string, std::shared_ptr<GXLib::MeshStore>> newMeshStore(filePath, std::make_shared<GXLib::MeshStore>(vertexBuffer, indexBuffer, vertexCount, indexCount));
	m_MeshList.insert(newMeshStore);
}

std::shared_ptr<GXLib::MeshStore> MeshStoreManager::GetMeshStore(const std::string& filePath)
{
	return m_MeshList[filePath];
}