// ---------------------------------------------------------------------------
// includes

#include "AEEngine.h"
#include "RenderSystem.h"


// ---------------------------------------------------------------------------
// main

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	int gGameRunning = 1;

	// Initialization of your own variables go here

	// Using custom window procedure
	AESysInit(hInstance, nCmdShow, 800, 600, 1, 60, true, NULL);

	// Changing the window title
	AESysSetWindowTitle("My New Demo!");

	// reset the system modules
	AESysReset();

	RenderSystem::Renderer RS{};
	RenderSystem::RenderSetting redTint;
	redTint.tint = { 1,1,1,1 };
	redTint.blendColor = { 1,1,1,1 };
	redTint.transperancy = 1;
	redTint.blendMode = AE_GFX_BM_BLEND;

	// Game Loop
	while (gGameRunning)
	{
		// Informing the system about the loop's start
		AESysFrameStart();

		// Handling Input
		AEInputUpdate();
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

		// Your own update logic goes here


		// Your own rendering logic goes here
		RS.AddBatch(RenderSystem::TILE_BATCH, RenderSystem::TILE, 0, 0);
		RS.AddBatch(RenderSystem::BUILDING_BATCH, RenderSystem::RESIDENTIAL_S, 50, 0, 99, RenderSystem::MID, redTint);

		RS.Render();

		// Informing the system about the loop's end
		AESysFrameEnd();

		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}


	// free the system
	AESysExit();
}