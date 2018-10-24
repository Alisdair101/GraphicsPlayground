#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>

#include <memory>
#include <vector>

// Project Dependencies
#include "../Base Interface/Manager.h"
#include "../Objects/Window/ManagedWindow.h"

struct WindowManagerConfig : ManagerConfig
{
    WindowManagerConfig() {};
    virtual ~WindowManagerConfig() {};

	HINSTANCE   m_WndHandleInstance;
	int         m_ShowWindow;
};

class WindowManager : Manager
{
	struct WindowSetup
	{
		LPCTSTR     m_WindowName;

		short int   m_WindowWidth;
		short int   m_WindowHeight;

		bool		m_Fullscreen;
	};

public:
                        WindowManager();
    virtual             ~WindowManager();

protected:
    void                Reset();
    void                Destroy();

// Parent Functionality
public:
    virtual HRESULT     Initialise(std::shared_ptr<ManagerConfig> config);

// Public Functionality
public:
	// Get/Set
	std::shared_ptr<ManagedWindow> GetWindow(int windowIndex) const { return m_ManagedWindows.at(windowIndex); }

// Private Functionality
private:
	HRESULT				LoadWindowSetup(WindowSetup& windowSetup);
	HRESULT				SetupNewWindow(WindowSetup& windowSetup, std::shared_ptr<WindowManagerConfig> windowMgrConfig);

	////////// Member Variables //////////
	std::vector<std::shared_ptr<ManagedWindow>>	m_ManagedWindows;

	short int			m_WindowsCount;



};