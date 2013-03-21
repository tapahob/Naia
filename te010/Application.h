#pragma once
class Application
{
public:

	//	Construction
	Application(void);
	Application(const Application&);
	~Application(void);

	//	Initializer
	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame(void);
	void InitializeWindows(int&, int&);
	void ShutdownWindows(void);

private:
	LPCWSTR	  m_ApplicationName;
	HINSTANCE m_Instance;
	HWND      m_Hwnd;
	int		  m_Width, m_Height;
	bool	  m_IsFullScreen;
};

// Global pointer
static Application* g_Application = 0;