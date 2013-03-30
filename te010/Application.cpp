#include "precompiled.h"
#include "Application.h"

//-------------------------------------------------------------------
//	Construction
//-------------------------------------------------------------------

Application::Application(void)
{
	m_ApplicationName = L"Default Application Name";
	m_Width           = 0;
	m_Height          = 0;
	m_Instance        = NULL;
	m_Hwnd            = NULL;
	m_IsFullScreen    = false;
}

Application::Application(const Application & other)
{
}

Application::~Application(void)
{
}

//-------------------------------------------------------------------
//	Callbacks
//-------------------------------------------------------------------
LRESULT CALLBACK Application::MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		{
			g_Input->KeyDown( (unsigned int) wparam);
			return 0;
		}
	case WM_KEYUP:
		{
			g_Input->KeyUp( (unsigned int) wparam);
			return 0;
		}

	default:
		{
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
	}
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
	default:
		{
			return Application::instance()->MessageHandler(hwnd, msg, wparam, lparam);
		}
	}
}

//-------------------------------------------------------------------
//	Initialization & CleanUp
//-------------------------------------------------------------------
void Application::InitializeWindows(int & screenWidth, int & screenHeight)
{
	NAIA_INFO("Initializing windows");

	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	//g_Application     = this;
	m_Instance        = GetModuleHandle(NULL);
	m_ApplicationName = L"tEngine";

	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_Instance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_ApplicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	// Get the desctop resolution
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (m_IsFullScreen)
	{
		NAIA_INFO("Initializing Fullscreen mode");
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields	  = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
		NAIA_INFO("Fullscreen mode initialized");
	}
	else
	{
		NAIA_INFO("Initializing windowed mode");
		// Set window size  to 1024x768
		screenWidth  = 1024;
		screenHeight = 768;

		// Place the window in the middle of the screen
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) /2;
		NAIA_INFO("Windowed mode initialized");
	}

	// Create the window with this display settings
	m_Hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_ApplicationName, m_ApplicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_Instance, NULL);

	ShowWindow(m_Hwnd, SW_SHOW);
	SetFocus(m_Hwnd);
	
	// Hide mouse cursor
	ShowCursor(false);

	return;
}

bool Application::Initialize(void)
{
	Logger::Init("LogSettings.xml");
	
	InitializeWindows(m_Width, m_Height);

	//g_Application = this;
	g_Input = new Input;
	if (!g_Input)
	{
		return false;
	}
	g_Input->Initialize();

	g_Graphics = new Graphics;

	g_Graphics->Initialize();

	NAIA_INFO("All systems are initialized");
	return true;
}

void Application::ShutdownWindows(void)
{
	NAIA_INFO("Shutting down the window");
	// Show mouse cursor
	ShowCursor(true);

	// Fix display settings if leaving fullscreen mode
	if (m_IsFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window
	DestroyWindow(m_Hwnd);
	m_Hwnd = NULL;

	// Remove the application instance
	UnregisterClass(m_ApplicationName, m_Instance);
	m_Instance = NULL;

//	g_Application = NULL;

	return;
}

Application* Application::s_instance = 0;

void Application::Shutdown(void)
{
	if (g_Graphics)
	{
		g_Graphics->Destroy();
	}
	if (g_Input)
	{
		delete g_Input;
		g_Input = 0;
		NAIA_INFO("the input system has been destroyed");
	}

	ShutdownWindows();
	NAIA_INFO("Cleanup complete");
	Logger::Destroy();
}


void Application::Run(void)
{
	MSG msg;
	bool done, result;

	ZeroMemory(&msg, sizeof(MSG));

	// Loop until there is a quit message
	done = false;
	while (!done)
	{
		// Handle the windows messages
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();
			if (!result)
			{
				done = true;
			}
		}
	}

	return;
}

bool Application::Frame(void)
{

	if(g_Input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	g_Graphics->Render();

	return true;
}


