#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>

#include <memory>

struct IObjectConfig
{
	IObjectConfig() {};
	virtual ~IObjectConfig() {};
};

class IObject
{
public:
	virtual         ~IObject() {};      // Destructor

	virtual			HRESULT Initialise(std::shared_ptr<IObjectConfig> obConfig) = 0;

protected:
	virtual void    Reset() = 0;        // Reset Member Variables
	virtual void    Destroy() = 0;      // Cleanup
};

