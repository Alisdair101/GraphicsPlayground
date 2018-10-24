#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>
#include <memory>

// Project Dependencies

struct ManagerConfig
{
    ManagerConfig() {};
    virtual ~ManagerConfig() {};
};

class Manager
{
public:
    virtual         ~Manager() {};      // Destructor

    virtual HRESULT Initialise(std::shared_ptr<ManagerConfig> mgrConfig) = 0;

protected:
    virtual void    Reset() = 0;        // Reset Member Variables
    virtual void    Destroy() = 0;      // Cleanup
};