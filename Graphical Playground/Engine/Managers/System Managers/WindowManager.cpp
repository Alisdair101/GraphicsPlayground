#include "WindowManager.h"

WindowManager::WindowManager()
{
    Reset();
}

WindowManager::~WindowManager()
{
    Destroy();
    Reset();
}

void WindowManager::Reset()
{
	m_WindowsCount = 0;
}

void WindowManager::Destroy()
{

}

HRESULT WindowManager::Initialise(std::shared_ptr<ManagerConfig> config)
{
	HRESULT hr;

    std::shared_ptr<WindowManagerConfig> windowMgrConfig = std::dynamic_pointer_cast<WindowManagerConfig>(config);

	WindowSetup windowSetup;
	hr = LoadWindowSetup(windowSetup);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = SetupNewWindow(windowSetup, windowMgrConfig);

	if (FAILED(hr))
	{
		return hr;
	}

    return S_OK;
}

HRESULT WindowManager::LoadWindowSetup(WindowSetup& windowSetup)
{
	// Settings will come in through config // TODO
	windowSetup.m_WindowName = L"First Window";

	windowSetup.m_WindowWidth = 800;
	windowSetup.m_WindowHeight = 600;

	windowSetup.m_Fullscreen = false;

	return S_OK;
}

HRESULT WindowManager::SetupNewWindow(WindowSetup& windowSetup, std::shared_ptr<WindowManagerConfig> windowMgrConfig)
{
	HRESULT hr;

	std::shared_ptr<ManagedWindowConfig> mgdWdwConfig = std::make_shared<ManagedWindowConfig>();
	mgdWdwConfig->m_WndHandleInstance = windowMgrConfig->m_WndHandleInstance;
	mgdWdwConfig->m_ShowWindow = windowMgrConfig->m_ShowWindow;

	mgdWdwConfig->m_WindowName = windowSetup.m_WindowName;
	mgdWdwConfig->m_WindowWidth = windowSetup.m_WindowWidth;
	mgdWdwConfig->m_WindowHeight = windowSetup.m_WindowHeight;

	m_ManagedWindows.push_back(std::make_shared<ManagedWindow>());
	hr = m_ManagedWindows.back()->Initialise(mgdWdwConfig);

	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}