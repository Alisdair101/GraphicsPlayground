#include "GraphicsModuleLibrary.h"


std::vector<byte> GXLib::ReadShaderFile(const std::wstring& filename)
{
	std::vector<byte> fileText;
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::ate | std::ios::binary);

	if (file.is_open())
	{
		fileText.resize(file.tellg());
		file.seekg(0, std::ios::beg);
		file.read(reinterpret_cast<char*>(&fileText[0]), fileText.size());
		file.close();
	}

	return fileText;
}