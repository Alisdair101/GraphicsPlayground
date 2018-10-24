#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>
#include <memory>

// Project Dependencies

struct EngineModuleConfig
{
    EngineModuleConfig() {};
    virtual ~EngineModuleConfig() {};
};

class EngineModule
{
public:
    virtual         ~EngineModule() {};      // Destructor

    virtual HRESULT Initialise(std::shared_ptr<EngineModuleConfig> engineConfig) = 0;
	virtual void	Run() = 0;

protected:
    virtual void    Reset() = 0;        // Reset Member Variables
    virtual void    Destroy() = 0;      // Cleanup
};