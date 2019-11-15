#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>
#include <C:\Program Files (x86)\Windows Kits\10\Include\10.0.15063.0\um\d3d11_4.h> // TODO
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
		SimpleVertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT4 colour) :
			m_Position(position),
			m_Colour(colour)
		{}

		DirectX::XMFLOAT3 m_Position;
		DirectX::XMFLOAT4 m_Colour;
	};

	// Function that reads from a binary file asynchronously.
	std::vector<byte> ReadShaderFile(const std::wstring& filename);
};