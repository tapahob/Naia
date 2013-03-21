#include <windows.h>
#include "Application.h"

//	Entry point
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	bool result;

	Application* application = new Application;
	if (!application)
	{
		return 0;
	}
	
	// Initialize everything and start main loop
	result = application->Initialize();
	if (result)
	{
		application->Run();
	}

	application->Shutdown();
	delete application;
	application = 0;

	return 0;
}