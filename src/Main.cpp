/*!************************************************************************
\file:          Main.cpp
\author:		Amadeus Chia
\par DP email:	amadeusjinhan.chia@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          12-01-2023
\brief
This source file handles the start of the program execution by loading the
first state in the SceneManager, afterwards SceneManager handles the game
loop until the Quit state is called.

The functions include:
- wWinMain
The entry point of the program

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

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
#include <DebugManager.h>
///////////////////////////////////////////////////////////////////////////

// The entry point of the program
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	///////////////////////////////////////////////////////////////////////
	// Random Seed Initialization
	srand(11);

	//Memory leak checker (for debug mode)
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
	//_crtBreakAlloc = 6359;

	// Engine Initialization
	AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);	// Using custom window procedure
	AESysSetWindowTitle("Momo Town");								// Changing the window title
	AESysReset();													// Reset the system modules
	AEFrameRateControllerInit(60);
	// AESysToggleFullScreen(true);

	// Manager Initialization
	FontManager::Initialize();
	TextureManager::Initialize();
	Debug::Initialize();

	SceneManager::Initialize(SceneManager::SPLASHSCREEN);				// Game Loop happens in SceneManager

	///////////////////////////////////////////////////////////////////////
	// Free the things that were initialized here
	TextureManager::Unload();
	FontManager::Unload();
	Debug::Free();

	// Engine Exit
	AESysExit();

	return 0;
}