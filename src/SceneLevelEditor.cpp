/*!************************************************************************
\file:          SceneLevelEditor.cpp
\author:		
\par DP email:
\par Course:    CSD1171B
\par Software Engineering Project
\date:          09-03-2023
\brief


The functions include:
-
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <AEEngine.h>
#include <RenderSystem.h>

#include <SceneManager.h>
#include <GridManager.h>
#include <BuildingManager.h>
#include <InputManager.h>
#include <UIManager.h>
#include <AudioManager.h>
#include <FileIOManager.h>
#include <ScoreManager.h>

#include <SceneLevelEditor.h>
#include <ColorTable.h>
#include <PauseManager.h>
#include <TextureManager.h>
///////////////////////////////////////////////////////////////////////////
// Forward Declarations
void LoadMap1();
void LoadMap2();
void LoadMap3();
void LoadUserMap1();
void LoadUserMap2();

void CreateLand();
void CreateWater();
void CreateNature();
///////////////////////////////////////////////////////////////////////////
// Variables
std::string selectedMapFilePath;

RenderSystem::Transform infoBackground;	// Rendering data for the hand background
UI::TextBox currentMapText;
UI::TextBox mapsText;
UI::TextBox controlsText;
UI::TextBox keysText;

void SceneLevelEditor::Load() {	
	BuildingManager::Initialize();

	selectedMapFilePath = "Assets/JSON_Data/Maps/map0.momomaps";
	GridManager::Initialize(selectedMapFilePath);

	currentMapText = UI::TextBox({ -AEGfxGetWinMaxX() + 25.0f, AEGfxGetWinMaxY() - 60.0f }, "Current Map: Archipelago", UI::LEFT_JUSTIFY, AEGfxGetWinMaxX() * 2.0f, 50.0f, COLOR_BLACK);
}

void SceneLevelEditor::Initialize() {
	PauseManager::Initialize();

	// Disable score and restart button
	PauseManager::ToggleShowRestart(false);

	// Bind keys 1-6 to the different maps
	InputManager::SubscribeToKey(AEVK_1, InputManager::TRIGGERED, LoadMap1);
	InputManager::SubscribeToKey(AEVK_2, InputManager::TRIGGERED, LoadMap2);
	InputManager::SubscribeToKey(AEVK_3, InputManager::TRIGGERED, LoadMap3);
	InputManager::SubscribeToKey(AEVK_4, InputManager::TRIGGERED, LoadUserMap1);
	InputManager::SubscribeToKey(AEVK_5, InputManager::TRIGGERED, LoadUserMap2);

	// Bind keys W-R to edit the map
	InputManager::SubscribeToKey(AEVK_D, InputManager::TRIGGERED, CreateWater);
	InputManager::SubscribeToKey(AEVK_F, InputManager::TRIGGERED, CreateLand);
	InputManager::SubscribeToKey(AEVK_G, InputManager::TRIGGERED, CreateNature);

	// Set background position and size
	infoBackground.size.x = (float)AEGfxGetWinMaxX() * 1.5f;	// The width of the hand BG default to 3/4 the screen width
	infoBackground.size.y = (float)AEGfxGetWinMaxY() * 0.4f;	// and 40% of the screen height

	infoBackground.pos.x = -infoBackground.size.x / 2.0f;							 // The BG should be at the middle
	infoBackground.pos.y = (float)AEGfxGetWinMinY() * 0.95f + infoBackground.size.y; // and bottom of the screen

	// Set the texts
	mapsText = UI::TextBox({ infoBackground.pos.x + infoBackground.size.x * 0.05f, infoBackground.pos.y - infoBackground.size.y * 0.25f},
						     "Swap maps: Press keys 1 to 5.",
						     UI::LEFT_JUSTIFY, infoBackground.size.x, 30, COLOR_BLACK);

	controlsText = UI::TextBox({ infoBackground.pos.x + infoBackground.size.x * 0.05f, infoBackground.pos.y - infoBackground.size.y * 0.6f },
								 "To edit terrain, hover mouse over a tile and press a key:",
								 UI::LEFT_JUSTIFY, infoBackground.size.x, 30, COLOR_BLACK);

	keysText = UI::TextBox({ infoBackground.pos.x + infoBackground.size.x * 0.05f, infoBackground.pos.y - infoBackground.size.y * 0.85f },
							 "Place water: key D. Place land: key F. Toggle nature: key G.",
							 UI::LEFT_JUSTIFY, infoBackground.size.x, 30, COLOR_BLACK);
}

void SceneLevelEditor::Update() {
	GridManager::UpdateMouseToGrid();
	PauseManager::Update();
	AudioManager::Update();
	TextureManager::Update();
}

void SceneLevelEditor::Draw() {
	AEGfxSetBackgroundColor(WATER);

	// Render the info background
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, infoBackground.pos.x, infoBackground.pos.y, infoBackground.size.x, infoBackground.size.y, TextureManager::BLANK_PROMPT);
	currentMapText.Render();
	mapsText.Render();
	controlsText.Render();
	keysText.Render();

	GridManager::PrepareTileRenderBatch();
	PauseManager::Draw();

	RenderSystem::Render();
}

void SceneLevelEditor::Free() {
	FileIOManager::SaveGridToFile(selectedMapFilePath);
	GridManager::Free();

	// Map switching
	InputManager::UnsubscribeKey(AEVK_1, InputManager::TRIGGERED, LoadMap1);
	InputManager::UnsubscribeKey(AEVK_2, InputManager::TRIGGERED, LoadMap2);
	InputManager::UnsubscribeKey(AEVK_3, InputManager::TRIGGERED, LoadMap3);
	InputManager::UnsubscribeKey(AEVK_4, InputManager::TRIGGERED, LoadUserMap1);
	InputManager::UnsubscribeKey(AEVK_5, InputManager::TRIGGERED, LoadUserMap2);

	// Terrain editting
	InputManager::UnsubscribeKey(AEVK_D, InputManager::TRIGGERED, CreateWater);
	InputManager::UnsubscribeKey(AEVK_F, InputManager::TRIGGERED, CreateLand);
	InputManager::UnsubscribeKey(AEVK_G, InputManager::TRIGGERED, CreateNature);
}

void SceneLevelEditor::Unload() {
	BuildingManager::Clear();
}

///////////////////////////////////////////////////////////////////////////
// Terrain editting

void CreateLand() {
	Vec2<int> mousePos = InputManager::GetMousePos();
	if (GridManager::isCellSafe(GridManager::ScreenPosToIso(mousePos))) {
		return;
	}

	GridManager::ToggleTileRenderable();
}

void CreateWater() {
	Vec2<int> mousePos = InputManager::GetMousePos();
	if (!GridManager::isCellSafe(GridManager::ScreenPosToIso(mousePos))) {
		return;
	}

	GridManager::ToggleTileRenderable();
}

void CreateNature() {
	Vec2<int> mousePos = InputManager::GetMousePos();
	if (!GridManager::isCellSafe(GridManager::ScreenPosToIso(mousePos))) {
		return;
	}

	GridManager::SpawnNature();
}

///////////////////////////////////////////////////////////////////////////
// Map loading

void LoadSelectedMap() {
	GridManager::Free();
	GridManager::Initialize(selectedMapFilePath);
}

void LoadMap1() {
	FileIOManager::SaveGridToFile(selectedMapFilePath);
	selectedMapFilePath = "Assets/JSON_Data/Maps/map0.momomaps";
	currentMapText = UI::TextBox({ -AEGfxGetWinMaxX() + 25.0f, AEGfxGetWinMaxY() - 60.0f }, "Current Map: Archipelago", UI::LEFT_JUSTIFY, AEGfxGetWinMaxX() * 2.0f, 50.0f, COLOR_BLACK);
	LoadSelectedMap();
}

void LoadMap2() {
	FileIOManager::SaveGridToFile(selectedMapFilePath);
	selectedMapFilePath = "Assets/JSON_Data/Maps/map1.momomaps";
	currentMapText = UI::TextBox({ -AEGfxGetWinMaxX() + 25.0f, AEGfxGetWinMaxY() - 60.0f }, "Current Map: Big Donut", UI::LEFT_JUSTIFY, AEGfxGetWinMaxX() * 2.0f, 50.0f, COLOR_BLACK);
	LoadSelectedMap();
}

void LoadMap3() {
	FileIOManager::SaveGridToFile(selectedMapFilePath);
	selectedMapFilePath = "Assets/JSON_Data/Maps/map2.momomaps";
	currentMapText = UI::TextBox({ -AEGfxGetWinMaxX() + 25.0f, AEGfxGetWinMaxY() - 60.0f }, "Current Map: Spiral Island", UI::LEFT_JUSTIFY, AEGfxGetWinMaxX() * 2.0f, 50.0f, COLOR_BLACK);
	LoadSelectedMap();
}

void LoadUserMap1() {
	FileIOManager::SaveGridToFile(selectedMapFilePath);
	selectedMapFilePath = "Assets/JSON_Data/Maps/userMap0.momomaps";
	currentMapText = UI::TextBox({ -AEGfxGetWinMaxX() + 25.0f, AEGfxGetWinMaxY() - 60.0f }, "Current Map: User Map 1", UI::LEFT_JUSTIFY, AEGfxGetWinMaxX() * 2.0f, 50.0f, COLOR_BLACK);
	LoadSelectedMap();
}

void LoadUserMap2() {
	FileIOManager::SaveGridToFile(selectedMapFilePath);
	selectedMapFilePath = "Assets/JSON_Data/Maps/userMap1.momomaps";
	currentMapText = UI::TextBox({ -AEGfxGetWinMaxX() + 25.0f, AEGfxGetWinMaxY() - 60.0f }, "Current Map: User Map 2", UI::LEFT_JUSTIFY, AEGfxGetWinMaxX() * 2.0f, 50.0f, COLOR_BLACK);
	LoadSelectedMap();
}
