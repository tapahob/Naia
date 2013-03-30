#pragma once
#ifndef HREN_H
#define HREN_H
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

	static Application *instance()
	{
		if (!s_instance)
			s_instance = new Application;
		return s_instance;
	}

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

	int  m_Width, m_Height;
	HWND m_Hwnd;
	bool m_IsFullScreen;

private:
	bool Frame(void);
	void InitializeWindows(int&, int&);
	void ShutdownWindows(void);

private:
	LPCWSTR	  m_ApplicationName;
	HINSTANCE m_Instance;
	static Application* s_instance;
	
	
};

// Global pointer
//Application* Application::s_instance = 0;

#endif