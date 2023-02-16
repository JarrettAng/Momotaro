/*!************************************************************************
\file:          CardManager.cpp
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief


The functions include:
-
**************************************************************************/

#include <vector>

#include <UIManager.h>
#include <CardManager.h>
#include <InputManager.h>

#include <ColorTable.h>
#include <Card.h>

#include <iostream>

namespace CardManager {
	int startingHandSize;
	std::vector<DeckData> deck;					// Data on all cards in play 
	std::vector<Card> hand;						// Data on rendering for current cards held

	UIManager::Transform cardPositionTemplate;	// Rendering data for a generic card
	UIManager::Transform handBackground;		// Rendering data for the hand background
	int cardSpacing;							// Spacing between cards in hand

	Card* selectedCard;							// Pointer to the current card selected by player

#pragma region Forward Declarations
	void AddToDeck(BuildingData buildingData);
	void AddToHand(DeckData deckCardData);
	void RemoveFromHand(Card* cardToRemove);
	void HandleClick();
#pragma endregion

	void Initialize() {
		// TODO: Use JSON for all these data
		startingHandSize = 5;
		selectedCard = nullptr;

		handBackground.width = AEGfxGetWinMaxX();
		handBackground.height = AEGfxGetWinMaxY() * 0.25f;

		handBackground.x = -handBackground.width / 2.0f;
		handBackground.y = AEGfxGetWinMinY() * 0.95f + handBackground.height;

		cardSpacing = AEGfxGetWinMaxX() * 0.025f;

		cardPositionTemplate.height = handBackground.height * 0.9f;
		cardPositionTemplate.width = cardPositionTemplate.height * 0.75f;
		cardPositionTemplate.y = handBackground.y - (handBackground.height - cardPositionTemplate.height) / 2.0f;

		// Fill hand with 5 starting cards
		DrawCard(BuildingEnum::RESIDENTIAL, BuildingEnum::L1);
		DrawCard(BuildingEnum::RESIDENTIAL, BuildingEnum::L1);
		DrawCard(BuildingEnum::RESIDENTIAL, BuildingEnum::L1);
		DrawCard(BuildingEnum::COMMERCIAL, BuildingEnum::L1);
		DrawCard(BuildingEnum::INDUSTRIAL, BuildingEnum::L1);

		InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleClick);
	}

	void Free() {
		deck.clear();
		hand.clear();

		InputManager::UnsubscribeKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleClick);
	}

	void PrepareUIRenderBatch() {
		UIManager::AddRectToBatch(handBackground.x, handBackground.y, handBackground.width, handBackground.height, 0, COLOR_CARD_BACKGROUND);

		// Render each card
		for (int index = 0; index < hand.size(); ++index) {
			Card* card = &hand[index];

			UIManager::AddRectToBatch(card->position.x, card->position.y, card->position.width, card->position.height, 1, card->borderColor);

			UIManager::AddRectToBatch(card->position.x + card->position.width * 0.05f, card->position.y - card->position.height * 0.035f, card->position.width * 0.9f, card->position.height * 0.925f, 2, card->color);

			UIManager::AddRectToBatch(card->icon.x, card->icon.y, card->icon.width, card->icon.height, 3, TextureManager::GetTexture((TextureManager::TEX_TYPE)(card->deckCardData.card.type * BuildingEnum::LEVEL_LENGTH + card->deckCardData.card.level)));

			//UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).S, card.name.x / AEGetWindowWidth(), card.name.y / AEGetWindowHeight(), 3, card.deckCardData->card.name, COLOR_BLACK);
			//UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).S, card.desc.x / AEGetWindowWidth(), card.desc.y / AEGetWindowHeight(), 1, card.deckCardData->card.desc, COLOR_BLACK);
		}
	}

	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level) {
		BuildingData buildingData = BuildingManager::GetBuildingData(type, BuildingEnum::_1X1, level);

		//std::cout << "DEBUG: CardManager Get Data of TYPE " << buildingData.type << " ,SIZE " << buildingData.size << " ,LEVEL " << buildingData.level << "\n";

		AddToDeck(buildingData);
	}

	void DrawRandomCard(BuildingEnum::LEVEL level) {
		BuildingData buildingData = BuildingManager::GetRandomBuildingData(level);

		//std::cout << "DEBUG: CardManager Get Data of TYPE " << buildingData.type << " ,SIZE " << buildingData.size << " ,LEVEL " << buildingData.level << "\n";

		AddToDeck(buildingData);
	}

	void DrawRandomCard(BuildingEnum::TYPE type) {
		BuildingData buildingData = BuildingManager::GetRandomBuildingData(type);

		//std::cout << "DEBUG: CardManager Get Data of TYPE " << buildingData.type << " ,SIZE " << buildingData.size << " ,LEVEL " << buildingData.level << "\n";

		AddToDeck(buildingData);
	}

	void AddToDeck(BuildingData buildingData) {
		if (!deck.empty()) {
			for (DeckData _deck : deck) {
				// If the data for it already exists, add to the count
				if (_deck.card.type == buildingData.type && _deck.card.size == buildingData.size && _deck.card.level == buildingData.level) {
					++_deck.count;
					return;
				}
			}
		}

		// Otherwise, add the new card to the deck
		DeckData newDeckCard;
		newDeckCard.count = 1;
		newDeckCard.card = buildingData;
		deck.push_back(newDeckCard);
		AddToHand(deck.back());
	}

	void UpdateHandPositions() {
		// Update the background size to wrap around the cards
		handBackground.width = hand.size() * cardPositionTemplate.width + (hand.size() + 1) * cardSpacing;
		handBackground.x = -handBackground.width / 2.0f;

		float currentX = handBackground.x + cardSpacing; // Starting from the left

		// Arrange the cards
		for (Card& card : hand) {
			card.position.x = currentX;
			card.UpdateComponentPositions();

			currentX += cardPositionTemplate.width + cardSpacing;
		}
	}

	void PlayCard() {
		--selectedCard->deckCardData.count;
		if (selectedCard->deckCardData.count <= 0) {
			RemoveFromHand(selectedCard);
			selectedCard = nullptr;
		}
	}

	void AddToHand(DeckData deckCardData) {
		hand.emplace_back(cardPositionTemplate, deckCardData);

		UpdateHandPositions();
	}

	void RemoveFromHand(Card* cardToRemove) {
		// hand.remove(*cardToRemove);

		UpdateHandPositions();
	}

	void HandleClick() {
		// Check if any card clicked
		Vec2<int> mousePos = InputManager::GetMousePos();

	}

#pragma region CARD CODE FROM UIMANAGER
	/*!***********************************************************************
* CARDS DECLARATIONS.
*************************************************************************/
//Individual Card Variables
	struct card {
		float x, y;
		bool clicked = false;
	}; card c1, c2, c3, c4, c5;

	float init_x = -500, init_y = -300;

	//Shared Card variables
	float hand_x, hand_y, c_height, c_width;

	//Cursor Var (s32 is an int)
	int cursor_x, cursor_y;
	// std::list<UIData> ListOfButtons; 
#include <UIManager.h>
	/*************************************************************************/

	void InitializeCards() {
		//Default location of card deck
		//hand_x = (AEGfxGetWinMaxX() - AEGfxGetWinMinX()) * (2.5f / 8.0f);
		hand_x = AEGfxGetWinMinX() + 100.0f;
		hand_y = (AEGfxGetWinMaxY() - AEGfxGetWinMinY()) * (3.0f / 8.0f);

		//Mesh is a square
		c_height = 60.0f;
		c_width = 60.0f;


		//TEMP--------------------------------------------------------
		// World x and y is different from cursor x and y
		// world (signed (+-) windowsize/2), cursor (window size) 
		// ex. -300 to +300 in engine is 0 to 600 on window
		c1.x = init_x;
		c1.y = init_y;

		c2.x = init_x - 50;
		c2.y = init_y;

		c3.x = init_x - 100;
		c3.y = init_y;

		c4.x = init_x - 150;
		c4.y = init_y;

		c5.x = init_x - 200;
		c5.y = init_y;
	}

	/*!***********************************************************************
	* Render the cards on the screen & allow the player to move them
	*************************************************************************/
	void DragCard() {
		//Return s32 == int
		AEInputGetCursorPosition(&cursor_x, &cursor_y);
		//Change cursor to float
		//static_cast<double>(cursor_x);
		//static_cast<double>(cursor_y);

		//RMD: Make into ONE function in future
		//Card1
		{
			//If clicked...
			if (AEInputCheckPrev(AEVK_LBUTTON)) {

				//within card mesh
				if (-cursor_y + 480.0f > c1.y - c_height && -cursor_y + 480.0f < c1.y
					&& cursor_x - 830.0f > c1.x && cursor_x - 830.0f < c1.x + c_width)
				{
					c1.clicked = true;
				}
			}
			else { c1.clicked = false; }

			//Card 1 check
			if (!c2.clicked && !c3.clicked && !c4.clicked && !c5.clicked) {
				if (AEInputCheckCurr(AEVK_LBUTTON) && c1.clicked == true) {
					c1.x = cursor_x - 800.0f - c_width;
					c1.y = -cursor_y + 450.0f + c_height;
					//if(AEInputCheckReleased(AEVK_LBUTTON))
				}
				else {
					//Default locations
					c1.x = init_x;
					c1.y = init_y;
				}
			}

			RenderSystem::AddSpriteBatch(RenderSystem::CARD_BATCH, RenderSystem::CARD, TextureManager::CARD_BLUE, c1.x, c1.y);
		}

		//Card2
		{

			//If clicked...
			if (AEInputCheckPrev(AEVK_LBUTTON)) {

				//within card mesh
				if (-cursor_y + 480.0f > c2.y - c_height && -cursor_y + 480.0f < c2.y
					&& cursor_x - 830.0f > c2.x && cursor_x - 830.0f < c2.x + c_width)
				{
					c2.clicked = true;
				}
			}
			else { c2.clicked = false; }

			//Card 2 check
			if (!c1.clicked && !c3.clicked && !c4.clicked && !c5.clicked) {
				if (AEInputCheckCurr(AEVK_LBUTTON) && c2.clicked == true) {
					c2.x = cursor_x - 800.0f - c_width;
					c2.y = -cursor_y + 450.0f + c_height;
					//if(AEInputCheckReleased(AEVK_LBUTTON))
				}
				else {
					//Default locations
					c2.x = init_x - 50;
					c2.y = init_y;
				}
			}

			RenderSystem::AddSpriteBatch(RenderSystem::CARD_BATCH, RenderSystem::CARD, TextureManager::CARD_BLUE, c2.x, c2.y);
		}

		//Card3
		{

			//If clicked...
			if (AEInputCheckPrev(AEVK_LBUTTON)) {

				//within card mesh
				if (-cursor_y + 480.0f > c3.y - c_height && -cursor_y + 480.0f < c3.y
					&& cursor_x - 830.0f > c3.x && cursor_x - 830.0f < c3.x + c_width)
				{
					c3.clicked = true;
				}
			}
			else { c3.clicked = false; }

			//Card 3 check
			if (!c1.clicked && !c2.clicked && !c4.clicked && !c5.clicked) {
				if (AEInputCheckCurr(AEVK_LBUTTON) && c3.clicked == true) {
					c3.x = cursor_x - 800.0f - c_width;
					c3.y = -cursor_y + 450.0f + c_height;
					//if(AEInputCheckReleased(AEVK_LBUTTON))
				}
				else {
					//Default locations
					c3.x = init_x - 100;
					c3.y = init_y;
				}
			}


			RenderSystem::AddSpriteBatch(RenderSystem::CARD_BATCH, RenderSystem::CARD, TextureManager::CARD_BLUE, c3.x, c3.y);
		}

		//Card4
		{

			//If clicked...
			if (AEInputCheckPrev(AEVK_LBUTTON)) {

				//within card mesh
				if (-cursor_y + 480.0f > c4.y - c_height && -cursor_y + 480.0f < c4.y
					&& cursor_x - 830.0f > c4.x && cursor_x - 830.0f < c4.x + c_width)
				{
					c4.clicked = true;
				}
			}
			else { c4.clicked = false; }

			//Card 4 check
			if (!c1.clicked && !c2.clicked && !c3.clicked && !c5.clicked) {
				if (AEInputCheckCurr(AEVK_LBUTTON) && c4.clicked == true) {
					c4.x = cursor_x - 800.0f - c_width;
					c4.y = -cursor_y + 450.0f + c_height;
					//if(AEInputCheckReleased(AEVK_LBUTTON))
				}
				else {
					//Default locations
					c4.x = init_x - 150;
					c4.y = init_y;
				}
			}

			RenderSystem::AddSpriteBatch(RenderSystem::CARD_BATCH, RenderSystem::CARD, TextureManager::CARD_BLUE, c4.x, c4.y);
		}

		//Card5
		{

			//If clicked...
			if (AEInputCheckPrev(AEVK_LBUTTON)) {

				//within card mesh
				if (-cursor_y + 480.0f > c5.y - c_height && -cursor_y + 480.0f < c5.y
					&& cursor_x - 830.0f > c5.x && cursor_x - 830.0f < c5.x + c_width)
				{
					c5.clicked = true;
				}
			}
			else { c5.clicked = false; }

			//Card 5 check
			if (!c1.clicked && !c2.clicked && !c3.clicked && !c4.clicked) {
				if (AEInputCheckCurr(AEVK_LBUTTON) && c5.clicked == true) {
					c5.x = cursor_x - 800.0f - c_width;
					c5.y = -cursor_y + 450.0f + c_height;
					//if(AEInputCheckReleased(AEVK_LBUTTON))
				}
				else {
					//Default locations
					c5.x = init_x - 200;
					c5.y = init_y;
				}
			}

			RenderSystem::AddSpriteBatch(RenderSystem::CARD_BATCH, RenderSystem::CARD, TextureManager::CARD_BLUE, c5.x, c5.y);
		}

	}
#pragma endregion

}


