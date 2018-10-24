#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>
#include <d3dx11.h>
#include <D3DCompiler.h>
#include <xnamath.h>

namespace GraphicsModuleLibrary
{
	struct SimpleVertex
	{
		SimpleVertex(float x, float y, float z) : m_Position(x, y, z) {}

		XMFLOAT3 m_Position;
	};

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);
};