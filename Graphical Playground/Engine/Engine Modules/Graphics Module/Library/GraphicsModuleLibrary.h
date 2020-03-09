#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>
#include <d3d11_4.h>
#include <DirectXMath.h>
#include <D3DCompiler.h>

// STL Dependencies
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

namespace GXLib
{
	struct SimpleVertex
	{
		SimpleVertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 texCoord) :
			m_Position(position),
			m_TexCoord(texCoord)
		{}

		DirectX::XMFLOAT3 m_Position;
		DirectX::XMFLOAT2 m_TexCoord;
	};

	struct MeshStore
	{
		MeshStore(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, unsigned int vertexCount, unsigned int indexCount) :
			m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), m_VertexCount(vertexCount), m_IndexCount(indexCount)
		{}

		ID3D11Buffer* m_VertexBuffer;
		ID3D11Buffer* m_IndexBuffer;

		unsigned int m_VertexCount;
		unsigned int m_IndexCount;
	};

	struct cbPerObject
	{
		DirectX::XMMATRIX WVP;
	};

	// Function that reads from a binary file asynchronously.
	std::vector<byte> ReadShaderFile(const std::wstring& filename);
};