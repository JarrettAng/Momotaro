/*!************************************************************************
\file:          SceneTutorial.cpp
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
#include <CardManager.h>
#include <UIManager.h>

#include <SceneTutorial.h>
#include <ColorTable.h>
#include <PauseManager.h>
#include <ScoreManager.h>
#include <TextureManager.h>
///////////////////////////////////////////////////////////////////////////
// Forward Declarations
void InitializePos();

void AdvanceTutorial();
void ShowTutPart1_Goal();
void ShowTutPart1_Score();
void ShowTutPart1_Buildings();
void ShowTutPart1_Buildings2();
void ShowTutPart1_Buildings3();
void ShowTutPart1_Buildings4(Vec2<int> mousePos);
void ShowTutPart1_Tiles();

void TutLoadMap1();
void TutLoadMap2();
void UpdatePopupSize(float pos_x, float pos_y, float size_x, float size_y);
void UpdatePopupText(std::string const& text, bool showClickToCont = true);
///////////////////////////////////////////////////////////////////////////
// Variables
RenderSystem::Transform momoPeachPos;	// Rendering data for Momo the peach
RenderSystem::Transform popupPos;		// Rendering data for the pop-up background
UI::TextBox				popupText;
UI::TextBox				clickToContinueText;

Vec2<int> screenCenter;
std::string lastPopupMsg;

// Flags to hide/show different UI
bool showHand;
bool showScore;

void (*nextTut_FuncPtr)();

void SceneTutorial::Load() {
	GridManager::Initialize("Assets/JSON_Data/Maps/tutorial.momomaps");
	BuildingManager::Initialize();
	ScoreManager::Initialize();
}

void SceneTutorial::Initialize() {
	PauseManager::Initialize();
	CardManager::Initialize();

	showHand = false;
	showScore = false;

	InputManager::SubscribeToKey(AEVK_N, InputManager::TRIGGERED, TutLoadMap1);
	InputManager::SubscribeToKey(AEVK_M, InputManager::TRIGGERED, TutLoadMap2);
	InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, AdvanceTutorial);

	InitializePos();
}

void SceneTutorial::Update() {
	GridManager::UpdateMouseToGrid();
	PauseManager::Update();

	CardManager::Update();

	TextureManager::Update();
}

void SceneTutorial::Draw() {
	AEGfxSetBackgroundColor(WATER);

	GridManager::PrepareTileRenderBatch();
	PauseManager::Draw();

	if (showHand) {
		CardManager::PrepareUIRenderBatch();
	}
	if (showScore) {
		ScoreManager::Draw();
	}

	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, momoPeachPos.pos.x, momoPeachPos.pos.y, momoPeachPos.size.x, momoPeachPos.size.y, TextureManager::MOMO_THE_PEACH, 5);
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, popupPos.pos.x, popupPos.pos.y, popupPos.size.x, popupPos.size.y, TextureManager::BLANK_PROMPT, 4);
	popupText.Render();
	clickToContinueText.Render();

	RenderSystem::Render();
}

void SceneTutorial::Free() {
	CardManager::Free();

	InputManager::UnsubscribeKey(AEVK_N, InputManager::TRIGGERED, TutLoadMap1);
	InputManager::UnsubscribeKey(AEVK_M, InputManager::TRIGGERED, TutLoadMap2);
	InputManager::UnsubscribeKey(AEVK_LBUTTON, InputManager::TRIGGERED, AdvanceTutorial);
}

void SceneTutorial::Unload() {
	GridManager::Free();
	BuildingManager::Clear();
	ScoreManager::Free();
}

void TutLoadMap1() {
	GridManager::Free();
	GridManager::Initialize("Assets/JSON_Data/Maps/tutorial.momomaps");
}

void TutLoadMap2() {
	GridManager::Free();
	GridManager::Initialize("Assets/JSON_Data/Maps/tutorial2.momomaps");
}

///////////////////////////////////////////////////////////////////////////
// Tutorial Sequence

void InitializePos() {
	lastPopupMsg = "Hello welcome to MomoTown! My name is Joe..... Sike! it's Momo, let me teach you the basics.";

	// Keep Momo the peach size 25% of the screen height
	momoPeachPos.size.x = momoPeachPos.size.y = AEGfxGetWinMaxY() * 0.25f;

	// Center the popup background
	UpdatePopupSize(0.0f, 0.0f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.25f);

	// What to show next
	nextTut_FuncPtr = ShowTutPart1_Goal;
}

void AdvanceTutorial() {
	if (!nextTut_FuncPtr) return;

	nextTut_FuncPtr();
}

void ShowTutPart1_Goal() {
	showHand = true;

	UpdatePopupSize(0.0f, -AEGfxGetWinMaxY() * 0.2f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.4f);
	UpdatePopupText("The goal of this game is simple. You have a hand of cards (look below). Use these cards to build buildings and get the highest SCORE possible!!");

	// What to show next
	nextTut_FuncPtr = ShowTutPart1_Score;
}

void ShowTutPart1_Score() {
	showScore = true;

	UpdatePopupSize(0.0f, AEGfxGetWinMaxY() * 0.5f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.35f);
	UpdatePopupText("What do I mean by SCORE? Look at the top left! SCORE is calculated from the \"synergy\" of the placement of your buildings.");

	// What to show next
	nextTut_FuncPtr = ShowTutPart1_Buildings;
}

void ShowTutPart1_Buildings() {
	UpdatePopupSize(0.0f, AEGfxGetWinMaxY() * 0.5f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.4f);
	UpdatePopupText("There are 3 types of buildings in the game. Residential, Commercial, and Industrial. Depending on how you place them, your SCORE will go up or down.");

	// What to show next
	nextTut_FuncPtr = ShowTutPart1_Buildings2;
}

void ShowTutPart1_Buildings2() {
	UpdatePopupSize(0.0f, -AEGfxGetWinMaxY() * 0.2f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.4f);
	UpdatePopupText("Don't worry. We will look at these buildings in more detail. Starting off, your hand contains a building of each type, a house, shop, and factory.");

	// What to show next
	nextTut_FuncPtr = ShowTutPart1_Buildings3;
}

void ShowTutPart1_Buildings3() {
	UpdatePopupSize(0.0f, AEGfxGetWinMaxY() * 0.6f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.25f);
	UpdatePopupText("Let's place a house down, click on the \"Small house\" card and click anywhere on the tiny island to place it down.", false);

	// What to show next
	CardManager::onCardPlaced.Subscribe(ShowTutPart1_Buildings4);
}

void ShowTutPart1_Buildings4(Vec2<int> mousePos) {
	UpdatePopupSize(0.0f, AEGfxGetWinMaxY() * 0.6f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.25f);
	UpdatePopupText("Wow! Look at those points go up! ", false);

	CardManager::onCardPlaced.Unsubscribe(ShowTutPart1_Buildings4);
}

void ShowTutPart1_Tiles() {

}

///////////////////////////////////////////////////////////////////////////
// Pop-up settings

// Resizes the pop-up background size and position
void UpdatePopupSize(float pos_x, float pos_y, float size_x, float size_y) {
	popupPos.size.x = size_x;
	popupPos.size.y = size_y;

	popupPos.pos.x = pos_x - size_x * 0.5f;
	popupPos.pos.y = pos_y + size_y * 0.5f;

	// Keep momo the peach at the bottom right of the popup
	momoPeachPos.pos.x = popupPos.pos.x + popupPos.size.x - momoPeachPos.size.x * 0.5f;
	momoPeachPos.pos.y = popupPos.pos.y - popupPos.size.y + momoPeachPos.size.y * 0.75f;

	UpdatePopupText(lastPopupMsg);
}

void UpdatePopupText(std::string const& text, bool showClickToCont) {
	lastPopupMsg = text;

	// Add a buffer at the edges
	Vec2<float> textPos = popupPos.pos;
	textPos.y -= popupPos.size.y * 0.3f;
	textPos.x += popupPos.size.x * 0.05f;

	popupText = UI::TextBox(textPos, text, UI::LEFT_JUSTIFY, popupPos.size.x * 0.95f, 24.0f, COLOR_CARD_COUNT);

	textPos.y = popupPos.pos.y - popupPos.size.y * 0.9f; // Add a 10% buffer at the botom

	clickToContinueText = UI::TextBox(textPos, showClickToCont ? "Click to continue..." : "", UI::RIGHT_JUSTIFY, popupPos.size.x * 0.925f, 24.0f, COLOR_CARD_COUNT);
}
