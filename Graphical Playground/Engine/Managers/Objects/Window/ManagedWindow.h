#pragma once

////////// Dependencies //////////
// Library Dependencies
#include <windows.h>

// Project Dependencies
#include "../../../Objects/ManagedObject.h"

struct ManagedWindowConfig : ManagedObjectConfig
{
	ManagedWindowConfig() {};
	virtual		~ManagedWindowConfig() {};

	HINSTANCE   m_WndHandleInstance;
	int         m_ShowWindow;

	LPCTSTR     m_WindowName;

	short int   m_WindowWidth;
	short int   m_WindowHeight;
};

class ManagedWindow
{
public:
						ManagedWindow();
	virtual				~ManagedWindow();

	// Parent Functionality
public:
	virtual HRESULT     Initialise(std::shared_ptr<ManagedObjectConfig> config);

	// Public Functionality
public:
	// Get/Set
	HWND				GetWindowHandle() const { return m_WindowHandle; }

	short int			GetWindowWidth() const { return m_WindowWidth; }
	short int			GetWindowHeight() const { return m_WindowHeight; }

	// Private Functionality
private:
	void                InitialiseParameters(std::shared_ptr<ManagedWindowConfig> mgdWdwConfig);
	HRESULT             InitialiseWindow(std::shared_ptr<ManagedWindowConfig> mgdWdwConfig);

	////////// Member Variables //////////
private:
	HWND                m_WindowHandle;
	LPCTSTR             m_WindowName;

	short int           m_WindowWidth;
	short int           m_WindowHeight;

	bool                m_Fullscreen;
};
