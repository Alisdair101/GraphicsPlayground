#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>

#include <memory>

struct ManagedObjectConfig
{
	ManagedObjectConfig() {};
	virtual ~ManagedObjectConfig() {};
};


class ManagedObject
{
public:
	virtual         ~ManagedObject() {};      // Destructor

	virtual			HRESULT Initialise(std::shared_ptr<ManagedObjectConfig> mgrConfig) = 0;

protected:
	virtual void    Reset() = 0;        // Reset Member Variables
	virtual void    Destroy() = 0;      // Cleanup
};

