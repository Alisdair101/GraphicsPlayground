#include "ManagedWindow.h"

// Windows Callback Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)	// Check message
	{
	case WM_KEYDOWN:	// For a key down if escape key was pressed, display popup box
		if (wParam == VK_ESCAPE)
		{
			if (MessageBox(0, L"Are you sure you want to exit?", L"Really?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				// Release the windows allocated memory  
				DestroyWindow(hwnd);
			}
		}
		return 0;

	case WM_DESTROY:	// If x button in top right was pressed
		PostQuitMessage(0);
		return 0;
	}

	// Return the message for windows to handle it
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

ManagedWindow::ManagedWindow()
{
}


ManagedWindow::~ManagedWindow()
{
}

HRESULT ManagedWindow::Initialise(std::shared_ptr<IObjectConfig> config)
{
	HRESULT hr;

	std::shared_ptr<ManagedWindowConfig> windowMgrConfig = std::dynamic_pointer_cast<ManagedWindowConfig>(config);

	InitialiseParameters(windowMgrConfig);

	hr = InitialiseWindow(windowMgrConfig);
	
	if (FAILED(hr))
	{
		return hr;
	}

	return S_OK;
}

void ManagedWindow::InitialiseParameters(std::shared_ptr<ManagedWindowConfig> mgdWdwConfig)
{
	m_WindowName = mgdWdwConfig->m_WindowName;
	m_WindowWidth = mgdWdwConfig->m_WindowWidth;
	m_WindowHeight = mgdWdwConfig->m_WindowHeight;
}

HRESULT ManagedWindow::InitialiseWindow(std::shared_ptr<ManagedWindowConfig> mgdWdwConfig)
{
	// Start creating the window
	WNDCLASSEX wc;	// Create a new extended windows class

	wc.cbSize = sizeof(WNDCLASSEX);						// Size of our windows class
	wc.style = CS_HREDRAW | CS_VREDRAW;					// Class styles
	wc.lpfnWndProc = WndProc;							// Default windows procedure function
	wc.cbClsExtra = NULL;								// Extra bytes after our wc structure
	wc.cbWndExtra = NULL;								// Extra bytes after our windows instance
	wc.hInstance = mgdWdwConfig->m_WndHandleInstance;	// Instance to current application
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);				// Title bar Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Default mouse Icon
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);		// Window bg color
	wc.lpszMenuName = NULL;								// Name of the menu attached to our window
	wc.lpszClassName = m_WindowName;					// Name of our windows class
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);			// Icon in your taskbar

	if (!RegisterClassEx(&wc))	// Register our windows class
	{
		// If registration failed, display error
		MessageBox(NULL, L"Error registering class", L"Error", MB_OK | MB_ICONERROR);
		return S_FALSE;
	}

	m_WindowHandle = CreateWindowEx(	    // Create our Extended Window
		NULL,								// Extended style
		m_WindowName,	                    // Name of our windows class
		L"Window Title",					// Name in the title bar of our window
		WS_OVERLAPPEDWINDOW,				// Style of our window
		CW_USEDEFAULT, CW_USEDEFAULT,		// Top left corner of window
		m_WindowWidth,						// Width of our window
		m_WindowHeight,						// Height of our window
		NULL,								// Handle to parent window
		NULL,								// Handle to a Menu
		mgdWdwConfig->m_WndHandleInstance,	// Specifies instance of current program
		NULL);								// Used for an MDI client window

	if (!m_WindowHandle)	// Make sure our window has been created
	{
		//If not, display error
		MessageBox(NULL, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
		return S_FALSE;
	}

	ShowWindow(m_WindowHandle, mgdWdwConfig->m_ShowWindow); // Shows our window
	UpdateWindow(m_WindowHandle);	// It's good to update our window

	return S_OK; // If there were no errors, return true
}