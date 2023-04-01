/*!************************************************************************
\file:          SceneGameLevel.cpp
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This source file implements the SceneGameLevel header file, it handles 
the main gameplay scene for the game.

The functions include:
- MapToLoad
Sets which map to load for the main gameplay (Used by LevelSelect)
- GetCurrentMapName
Gets which map is currently loaded
**************************************************************************/

#include <AEEngine.h>
#include <RenderSystem.h>

#include <SceneManager.h>
#include <GridManager.h>
#include <BuildingManager.h>
#include <InputManager.h>
#include <CardManager.h>
#include <UIManager.h>

#include <SceneGameLevel.h>
#include <ColorTable.h>
#include <PauseManager.h>
#include <ScoreManager.h>
#include <TextureManager.h>
#include <AudioManager.h>
#include <GameOverManager.h>
#include <FileIOManager.h>

// Which file to load the map from, by default it is map0
std::string mapToLoad = "Assets/JSON_Data/Maps/map0.momomaps";

void SceneGameLevel::Load() {
	// Empty by design
}

void SceneGameLevel::Initialize() {
	// Load score
	ScoreManager::Initialize();
	ScoreManager::ToggleShowHighScore(true);

	// Load grid and pause/restart button
	GridManager::Initialize(mapToLoad);
	PauseManager::Initialize();

	// If loading from a save game, initialize starting hand from save file, otherwise load default starting hand
	if(FileIOManager::LoadHandFromFile().empty() || mapToLoad != "Assets/JSON_Data/Maps/lastSaved2.momomaps"){
		CardManager::Initialize();
	}
	else CardManager::Initialize(FileIOManager::LoadHandFromFile());

	GameOverManager::Initialize();
}

// Called before Draw() every frame
void SceneGameLevel::Update() {
	GridManager::UpdateMouseToGrid();
	PauseManager::Update();
	TextureManager::Update();
	CardManager::Update();
	GameOverManager::Update();
}

// Loads all the data needed for rendering, then renders the frame
void SceneGameLevel::Draw() {
	AEGfxSetBackgroundColor(WATER);

	GridManager::PrepareTileRenderBatch();
	CardManager::PrepareUIRenderBatch();
	PauseManager::Draw();
	ScoreManager::Draw();
	GameOverManager::Draw();

	RenderSystem::Render();
}

void SceneGameLevel::Free() {
	if(!GameOverManager::IsGameOver()){
		FileIOManager::SaveHandToFile(CardManager::GetCurrentHand());
		FileIOManager::SaveGridToFile("Assets/JSON_Data/Maps/lastSaved2.momomaps");
		FileIOManager::SaveHighScoreToFile();
	}
	GridManager::Free();
	InputManager::Free();
	CardManager::Free();
	RenderSystem::Free();
	PauseManager::Free();
	ScoreManager::Free();
	GameOverManager::Free();
}

void SceneGameLevel::Unload() {
	BuildingManager::Clear();
}

// Sets which map to load for the main gameplay (Used by LevelSelect)
void MapToLoad(std::string const& mapFilePath) {
	mapToLoad = mapFilePath;
}

// Gets which map is currently loaded
std::string const &GetCurrentMapName() {
    return mapToLoad;
}
