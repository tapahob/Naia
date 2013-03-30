#include <windows.h>
#include "Application.h"

//	Entry point
int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	bool result;
	
	/*g_Application = new Application;
	if (!g_Application)
	{
		return 0;
	}
	*/
	// Initialize everything and start main loop
	result = Application::instance()->Initialize();
	if (result)
	{
		Application::instance()->Run();
	}

	Application::instance()->Shutdown();
	delete Application::instance();

	return 0;
}