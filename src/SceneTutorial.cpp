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
void ShowTutPart1_Buildings5();
void ShowTutPart1_Merging();
void ShowTutPart1_Merging2();
void ShowTutPart1_Merging3();
void ShowTutPart1_Score2();
void ShowTutPart1_Score3();
void ShowTutPart1_Score4();
void ShowTutPart2_Tiles();	// Key M skips to this part of tutorial
void ShowTutPart2_Tiles2();
void ShowTutPart2_Tiles3();
void ShowTutPart2_Tiles4();
void ShowTutPart2_Tiles5();
void FinalTestUpdate();
void ShowTutPart2_Finish();
void ShowTutPart2_Finish2();
void ShowTutPart2_Finish3();
void LoadMainMenu();

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

int finalTestScore;

void (*nextTut_FuncPtr)();

void SceneTutorial::Load() {
	GridManager::Initialize("Assets/JSON_Data/Maps/tutorial.momomaps");
	BuildingManager::Initialize();
	ScoreManager::Initialize();
}

void SceneTutorial::Initialize() {
	PauseManager::Initialize();
	CardManager::Initialize();

	// Give more residential cards
	CardManager::DrawCard(BuildingEnum::RESIDENTIAL, BuildingEnum::L1);
	CardManager::DrawCard(BuildingEnum::RESIDENTIAL, BuildingEnum::L1);

	showHand = false;
	showScore = false;

	finalTestScore = 1;

	InputManager::SubscribeToKey(AEVK_M, InputManager::TRIGGERED, ShowTutPart2_Tiles);
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

	InputManager::UnsubscribeKey(AEVK_M, InputManager::TRIGGERED, ShowTutPart2_Tiles);
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
	nextTut_FuncPtr = nullptr; // Remove the click trigger, use the event instead
	CardManager::onCardPlaced.Subscribe(ShowTutPart1_Buildings4);
}

void ShowTutPart1_Buildings4(Vec2<int> mousePos) {
	UpdatePopupSize(0.0f, -AEGfxGetWinMaxY() * 0.3f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.3f);
	UpdatePopupText("Wow! Look at those points go up! This is an example of \"synergy\", houses like to be near nature, the coast for example!");

	// What to show next
	CardManager::onCardPlaced.Unsubscribe(ShowTutPart1_Buildings4);
	nextTut_FuncPtr = ShowTutPart1_Buildings5; // Back to the click event
}

void ShowTutPart1_Buildings5() {
	UpdatePopupSize(0.0f, -AEGfxGetWinMaxY() * 0.3f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.4f);
	UpdatePopupText("Let's try placing 3 houses next to each other (NOT diagonally)!! The card you last selected will remain selected, so you don't need to re-select it again.", false);

	// What to show next
	nextTut_FuncPtr = nullptr; // Remove the click trigger, use the event instead
	GridManager::onMergeBuildings.Subscribe(ShowTutPart1_Merging);
}

void ShowTutPart1_Merging() {
	UpdatePopupSize(0.0f, -AEGfxGetWinMaxY() * 0.3f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.3f);
	UpdatePopupText("You're good at this! Placing 3 buildings of the same type will merge it and level up! The new building will remain at the last tile clicked.");

	// What to show next
	GridManager::onMergeBuildings.Unsubscribe(ShowTutPart1_Merging);
	nextTut_FuncPtr = ShowTutPart1_Merging2; // Back to the click event
}

void ShowTutPart1_Merging2() {
	UpdatePopupSize(0.0f, -AEGfxGetWinMaxY() * 0.3f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.3f);
	UpdatePopupText("Did you notice? Merging buildings not only gives more space, but you get a random level 2 card added to your hand as well!!");

	// What to show next
	nextTut_FuncPtr = ShowTutPart1_Merging3;
}

void ShowTutPart1_Merging3() {
	UpdatePopupSize(0.0f, AEGfxGetWinMaxY() * 0.6f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.25f);
	UpdatePopupText("By the way, if you are ever confused on what each card does, you can hover over it to see more info!");

	// What to show next
	nextTut_FuncPtr = ShowTutPart1_Score2;
}

void ShowTutPart1_Score2() {
	UpdatePopupSize(0.0f, AEGfxGetWinMaxY() * 0.6f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.3f);
	UpdatePopupText("Now, another way to get more cards is to hit the \"synergy\" threshold at the top left. Place buildings until you reach the next threshold!", false);

	// What to show next
	nextTut_FuncPtr = nullptr; // Remove the click trigger, use the event instead
	ScoreManager::onLevelChange.Subscribe(ShowTutPart1_Score3);
}

void ShowTutPart1_Score3() {
	UpdatePopupSize(0.0f, -AEGfxGetWinMaxY() * 0.3f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.3f);
	UpdatePopupText("Nice one! You get a few level 1 cards every time a threshold is reached. However, the amount needed will get bigger over time, so plan ahead!");

	// What to show next
	ScoreManager::onLevelChange.Unsubscribe(ShowTutPart1_Score3);
	nextTut_FuncPtr = ShowTutPart1_Score4; // Back to the click event
}

void ShowTutPart1_Score4() {
	UpdatePopupSize(0.0f, 0.0f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.25f);
	UpdatePopupText("Anyways, this map is getting a little too small, let's go to a bigger one.");

	// What to show next
	nextTut_FuncPtr = ShowTutPart2_Tiles;
}

// Key M skips the tutorial to this part
void ShowTutPart2_Tiles() {
	TutLoadMap2();

	// Reset everything
	showHand = showScore = true;
	
	CardManager::Free();
	CardManager::Initialize();

	ScoreManager::Free();
	ScoreManager::Initialize();


	UpdatePopupSize(0.0f, -AEGfxGetWinMaxY() * 0.3f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.3f);
	UpdatePopupText("Ahh. Much better! So much more room to build so much MOREEE.");

	// What to show next
	nextTut_FuncPtr = ShowTutPart2_Tiles2;
}

void ShowTutPart2_Tiles2() {
	UpdatePopupSize(0.0f, -AEGfxGetWinMaxY() * 0.3f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.3f);
	UpdatePopupText("I only have but one more thing to teach you before you become a city building master!");

	// What to show next
	nextTut_FuncPtr = ShowTutPart2_Tiles3;
}

void ShowTutPart2_Tiles3() {
	UpdatePopupSize(0.0f, -AEGfxGetWinMaxY() * 0.3f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.3f);
	UpdatePopupText("Maps can have existing \"nature\" tiles (like rocks and ponds), you cannot remove or build over it. So plan your city wisely!");

	// What to show next
	nextTut_FuncPtr = ShowTutPart2_Tiles4;
}

void ShowTutPart2_Tiles4() {
	UpdatePopupSize(0.0f, -AEGfxGetWinMaxY() * 0.3f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.3f);
	UpdatePopupText("Before we end, let's put your new city building skills to the test. Can you hit the 4th threshold with your starting hand?");

	// What to show next
	nextTut_FuncPtr = ShowTutPart2_Tiles5;
}

void ShowTutPart2_Tiles5() {
	UpdatePopupSize(0.0f, -AEGfxGetWinMaxY() * 0.35f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.25f);
	UpdatePopupText("(Hit the 4th score threshold to end the tutorial, or quit using the pause button at the top right)", false);

	// What to show next
	nextTut_FuncPtr = nullptr; // Remove the click trigger, use the event instead
	ScoreManager::onLevelChange.Subscribe(FinalTestUpdate);
}

void FinalTestUpdate() {
	++finalTestScore;
	if (finalTestScore >= 5) {
		ShowTutPart2_Finish();
		ScoreManager::onLevelChange.Unsubscribe(FinalTestUpdate);
	}
}

void ShowTutPart2_Finish() {
	UpdatePopupSize(0.0f, 0.0f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.25f);
	UpdatePopupText("Woohoo!");

	// What to show next
	nextTut_FuncPtr = ShowTutPart2_Finish2; // Back to the click event
}

void ShowTutPart2_Finish2() {
	UpdatePopupSize(0.0f, 0.0f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.25f);
	UpdatePopupText("Congrats! For graduating this tutorial, here's your degree in city building, and it didn't even cost you $50k!!");

	// What to show next
	nextTut_FuncPtr = ShowTutPart2_Finish3; // Back to the click event
}

void ShowTutPart2_Finish3() {
	UpdatePopupSize(0.0f, 0.0f, AEGfxGetWinMaxX(), AEGfxGetWinMaxY() * 0.25f);
	UpdatePopupText("Goodbye, it was fun teaching you!!! See you next time. (Click again to return to main menu)");

	// The end!
	InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, LoadMainMenu);
}

void LoadMainMenu() {
	SceneManager::LoadScene(SceneManager::MAIN_MENU);
	InputManager::UnsubscribeKey(AEVK_LBUTTON, InputManager::TRIGGERED, LoadMainMenu);
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
