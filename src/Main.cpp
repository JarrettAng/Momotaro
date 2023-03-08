///////////////////////////////////////////////////////////////////////////
#include <cstdlib>
#include <memory>

#include <AEEngine.h>
#include <RenderSystem.h>
#include <SceneManager.h>
#include <GridManager.h>
#include <TextureManager.h>
#include <InputManager.h>
#include <ScoreManager.h>
#include <FontManager.h>
///////////////////////////////////////////////////////////////////////////

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	///////////////////////////////////////////////////////////////////////
	// Random Seed Initialization
	srand(11);

	//Memory leak checker (for debug mode)
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Engine Initialization
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);	// Using custom window procedure
	AESysSetWindowTitle("Momo Town");								// Changing the window title
	AESysReset();													// Reset the system modules
	AEFrameRateControllerInit(60);
	//AESysToggleFullScreen(true);

	// Manager Initialization
	FontManager::Initialize();
	TextureManager::Initialize();

	SceneManager::Initialize(SceneManager::SPLASHSCREEN);				// Game Loop happens in SceneManager

	///////////////////////////////////////////////////////////////////////
	// Free the things that were initialized here
	GridManager::Free();

	// Engine Exit
	AESysExit();

	return 0;
}