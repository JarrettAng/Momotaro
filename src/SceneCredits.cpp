/*!************************************************************************
\file SceneCredits.cpp
\author Tan Jun Rong
\par DP email: t.junrong@digipen.edu
\par Course: CSD1171B
\par Software Engineering Project
\date 31-03-2023
\brief
This source file handles credits scene of the game.
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <SceneManager.h>

#include <SceneCredits.h>
#include <AEEngine.h>
#include <InputManager.h>
#include <RenderSystem.h>
#include <MomoMaths.h>

#include <TextureManager.h>
#include <AudioManager.h>
///////////////////////////////////////////////////////////////////////////

/*!***********************************************************************
* Const variables.
*************************************************************************/
const float HEADER_FONT_SIZE = 60.0f;
const float NAME_FONT_SIZE = 40.0f;

const float HEADER_OFFSET = 60.0f;
const float NAME_OFFSET = 50.0f;
const float SECTION_OFFSET = 400.0f;

const float SCROLL_SPEED_MULTIPLIER = 0.3f;

/*!***********************************************************************
* Forward Declaractions.
*************************************************************************/
void InitializeTexts();
void InitializeCreditsUI();

void DrawCredits();
RenderSystem::Text GenerateHeader(std::string text);
RenderSystem::Text GenerateName(std::string text);
RenderSystem::Text GenerateText(std::string text, float fontSize, float offset);

void HandleBackBtnClick();
void HandleBackBtnHover();
void ReturnToMenu();

/*!***********************************************************************
* UI.
*************************************************************************/
std::vector<RenderSystem::Text> creditTexts;

RenderSystem::Interactable backBtn{};

/*!***********************************************************************
* Variables.
*************************************************************************/
float textWidth, textHeight;	// For caching text width and height to center the text.
float currTextY{};				// Y position when initializing texts.

/*!***********************************************************************
\brief
	Load SceneCredits.
*************************************************************************/
void SceneCredits::Load() {
	return;
}

/*!***********************************************************************
\brief
	Initialize SceneCredits.
*************************************************************************/
void SceneCredits::Initialize() {
	// Initialize credits name.
	InitializeTexts();
	// Initialize UI.
	InitializeCreditsUI();
	// Subscribe to key events.
	InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleBackBtnClick);
	// Play credit BGM.
	AudioManager::PlayBGM(AudioManager::ClipName::BGM_CREDITS);
}

/*!***********************************************************************
\brief
	Update SceneCredits.
*************************************************************************/
void SceneCredits::Update() {
	// Check for button presses.
	InputManager::HandleInput();
	// Update AudioManager to play credit BGM.
	AudioManager::Update();
}

/*!***********************************************************************
\brief
	Draw SceneCredits.
*************************************************************************/
void SceneCredits::Draw() {
	// Draw background to fit to screen.
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, AEGfxGetWinMinX(), -AEGfxGetWinMinY(), (float)AEGetWindowWidth(), (float)AEGetWindowHeight(), TextureManager::CREDITS_BG, -1);

	// Draw back button.
	RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, backBtn.render.rect.transform.pos.x, backBtn.render.rect.transform.pos.y, backBtn.render.rect.transform.size.x, backBtn.render.rect.transform.size.y, TextureManager::BACK_BTN);

	// Draw credit names.
	DrawCredits();

	// Visual feedback for back btn.
	HandleBackBtnHover();

	// Render all graphics.
	RenderSystem::Render();
}

/*!***********************************************************************
\brief
	Free SceneCredits.
*************************************************************************/
void SceneCredits::Free() {
	creditTexts.clear();
	InputManager::Free();
	RenderSystem::Free();
}

/*!***********************************************************************
\brief
	Unload SceneCredits.
*************************************************************************/
void SceneCredits::Unload() {
	return;
}

/*!***********************************************************************
\brief
	Draw all the names in credits.
*************************************************************************/
void DrawCredits() {
	for (RenderSystem::Text& t : creditTexts) {
		// Scroll the text up.
		t.pos.y += (float)AEFrameRateControllerGetFrameTime() * SCROLL_SPEED_MULTIPLIER;
		// Draw the text.
		RenderSystem::AddTextToBatch(RenderSystem::UI_BATCH, t.pos.x, t.pos.y, t.fontID, t.fontSize, t.text, 1, Vec3<float> {1, 1, 1});
	}

	// Wrap texts if last text reaches the top.
	if (creditTexts.back().pos.y > 1) {
		for (RenderSystem::Text& t : creditTexts) {
			t.pos.y = t.cachedPos.y;
		}
	}
}

/*!***********************************************************************
\brief
	Visual feedback when player hover over the button.
*************************************************************************/
void HandleBackBtnHover() {
	// Cache mouse position.
	Vec2<int> mousePos = InputManager::GetMousePos();

	// Convert to world space position.
	mousePos.x -= (int)AEGfxGetWinMaxX();
	mousePos.y -= (int)AEGfxGetWinMaxY();

	// Check if mouse is clicking back btn.
	if (MouseInsideButton(mousePos, backBtn.render.rect.transform.pos, backBtn.render.rect.transform.size)) {
		// Scale btn for visual feedback.
		backBtn.render.rect.transform.size = backBtn.render.rect.transform.cachedSize * 1.1f;
	}
	else {
		// Scale btn to original size.
		backBtn.render.rect.transform.size = backBtn.render.rect.transform.cachedSize;
	}
}

/*!***********************************************************************
\brief
	Go back to main menu if player clicked on back btn.
*************************************************************************/
void HandleBackBtnClick() {
	// Cache mouse position.
	Vec2<int> mousePos = InputManager::GetMousePos();

	// Convert to world space position.
	mousePos.x -= (int)AEGfxGetWinMaxX();
	mousePos.y -= (int)AEGfxGetWinMaxY();

	// Check if mouse is clicking back btn.
	if (MouseInsideButton(mousePos, backBtn.render.rect.transform.pos, backBtn.render.rect.transform.size)) {
		backBtn.func();
	}
}

/*!***********************************************************************
\brief
	Initialize all credit text.
*************************************************************************/
void InitializeTexts() {
	currTextY = -1;	// -1 is the bottom of the screen when drawing text. So start drawing below the screen.

	creditTexts.push_back(GenerateHeader("TEAM MOMOTARO"));
	creditTexts.push_back(GenerateName("Ang Jiawei Jarrett"));
	creditTexts.push_back(GenerateName("Amadeus Jinhan Chia"));
	creditTexts.push_back(GenerateName("Chris Esplana"));
	creditTexts.push_back(GenerateName("Tan Jun Rong"));
	currTextY -= SECTION_OFFSET / AEGetWindowHeight();

	creditTexts.push_back(GenerateHeader("INSTRUCTORS"));
	creditTexts.push_back(GenerateName("Cheng Ding Xiang"));
	creditTexts.push_back(GenerateName("Gerald Wong"));
	currTextY -= SECTION_OFFSET / AEGetWindowHeight();

	creditTexts.push_back(GenerateHeader("EXECUTIVES"));
	creditTexts.push_back(GenerateName("Jason Chu"));
	creditTexts.push_back(GenerateName("Christopher Comair"));
	creditTexts.push_back(GenerateName("Michael Gats"));
	creditTexts.push_back(GenerateName("Michele Comair"));
	creditTexts.push_back(GenerateName("Raymond Yam"));
	creditTexts.push_back(GenerateName("Samir Abou Samra"));
	creditTexts.push_back(GenerateName("Prasanna Ghali"));
	creditTexts.push_back(GenerateName("John Bauer"));
	creditTexts.push_back(GenerateName("Dr. Erik Mohrmann"));
	creditTexts.push_back(GenerateName("Melvin Gonsalvez"));
	creditTexts.push_back(GenerateName("Angela Kugler"));
	creditTexts.push_back(GenerateName("Dr. Charles Duba"));
	creditTexts.push_back(GenerateName("Ben Ellinger"));
	creditTexts.push_back(GenerateName("Johnny Deek"));
	currTextY -= SECTION_OFFSET / AEGetWindowHeight();

	creditTexts.push_back(GenerateHeader("CREATED AT"));
	creditTexts.push_back(GenerateName("DigiPen Institute of Technology Singapore"));
	currTextY -= SECTION_OFFSET / AEGetWindowHeight();

	creditTexts.push_back(GenerateHeader("PRESIDENT"));
	creditTexts.push_back(GenerateName("Claude Comair"));
	currTextY -= SECTION_OFFSET / AEGetWindowHeight();

	creditTexts.push_back(GenerateHeader("SPECIAL THANKS"));
	creditTexts.push_back(GenerateName("Ligma Johnson"));
	creditTexts.push_back(GenerateName("Ben Dover"));
	creditTexts.push_back(GenerateName("Sawcon"));
	creditTexts.push_back(GenerateName("Xi Jinping"));
	creditTexts.push_back(GenerateName("Mike Sugandese"));
	creditTexts.push_back(GenerateName("C.D"));
	creditTexts.push_back(GenerateName("Rick Astley"));
	creditTexts.push_back(GenerateName("Joe"));
	creditTexts.push_back(GenerateName("Winnie the Pooh"));
	creditTexts.push_back(GenerateName("Candice"));
	currTextY -= SECTION_OFFSET / AEGetWindowHeight();

	creditTexts.push_back(GenerateHeader("LICENSES"));
	creditTexts.push_back(GenerateName("Soundly Cloud Library"));
	currTextY -= NAME_OFFSET / AEGetWindowHeight();
	creditTexts.push_back(GenerateName("FMOD Sound System"));
	creditTexts.push_back(GenerateName("FireLight Technologies Pty Ltd"));
	currTextY -= SECTION_OFFSET / AEGetWindowHeight();

	creditTexts.push_back(GenerateName("All content copyright 2023"));
	creditTexts.push_back(GenerateName("Digipen Institute of Technology Singapore."));
	currTextY -= NAME_OFFSET / AEGetWindowHeight();
	creditTexts.push_back(GenerateName("All Rights Reserved."));
}

/*!***********************************************************************
\brief
	Create a header text object.
\param text
	Text string.
\return
	Text object.
*************************************************************************/
RenderSystem::Text GenerateHeader(std::string text) {
	return GenerateText(text, HEADER_FONT_SIZE, HEADER_OFFSET);
}

/*!***********************************************************************
\brief
	Create a name text object.
\param text
	Text string.
\return
	Text object.
*************************************************************************/
RenderSystem::Text GenerateName(std::string text) {
	return GenerateText(text, NAME_FONT_SIZE, NAME_OFFSET);
}

/*!***********************************************************************
\brief
	Create a text object.
\param text
	Text string.
\param fontSize
	Size of font.
\param offset
	Offset to apply to next text.
\return
	Text object.
*************************************************************************/
RenderSystem::Text GenerateText(std::string text, float fontSize, float offset) {
	// Initialize text object.
	RenderSystem::Text name;
	// Set font and text.
	name.fontID = FontManager::GetFont(FontManager::SHIN_GO);
	name.fontSize = (int)fontSize;
	name.text = text;


	AEGfxGetPrintSize(name.fontID, const_cast<char*>(text.c_str()), fontSize / FontManager::DEFAULT_FONT_SIZE, textWidth, textHeight);
	textWidth /= 2;

	// Align to middle of screen.
	name.pos = Vec2<float>{ -textWidth, currTextY };
	name.cachedPos = name.pos;

	// Apply offset.
	currTextY -= textHeight + offset / AEGetWindowHeight();

	return name;
}

/*!***********************************************************************
\brief
	Initialize UI.
*************************************************************************/
void InitializeCreditsUI() {
	backBtn.render.rect.graphics.tex = TextureManager::BACK_BTN;
	backBtn.func = ReturnToMenu;

	backBtn.render.rect.transform.pos.x = GetWorldXByPercentage(88.0f);
	backBtn.render.rect.transform.pos.y = GetWorldYByPercentage(13.2f);

	backBtn.render.rect.transform.size.x = 150.0f;
	backBtn.render.rect.transform.size.y = 78.0f;
	backBtn.render.rect.transform.cachedSize = backBtn.render.rect.transform.size;
}

/*!***********************************************************************
\brief
	Load Main Menu scene.
*************************************************************************/
void ReturnToMenu() {
	SceneManager::LoadScene(SceneManager::MAIN_MENU);
}
