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
	std::vector<Card> hand;						// Data on rendering for current cards held

	UIManager::Transform cardPositionTemplate;	// Rendering data for a generic card
	UIManager::Transform handBackground;		// Rendering data for the hand background
	int cardSpacing;							// Spacing between cards in hand

	Card* selectedCard;							// Pointer to the current card selected by player

#pragma region Forward Declarations
	void AddToHand(BuildingData cardData);
	void RemoveFromHand(Card *cardToRemove);
	void HandleClick();
	void UpdateHandPositions();
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
		hand.clear();
		InputManager::UnsubscribeKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleClick);
	}

	void PrepareUIRenderBatch() {
		UIManager::AddRectToBatch(handBackground.x, handBackground.y, handBackground.width, handBackground.height, 0, COLOR_CARD_BACKGROUND);

		// Render each card
		for (int index = 0; index < hand.size(); ++index) {
			Card &card = hand[index];

			// Drawing border of the card
			UIManager::AddRectToBatch(card.position.x, card.position.y, card.position.width, card.position.height, 1, card.borderColor);
			// Drawing the card background
			UIManager::AddRectToBatch(card.position.x + card.position.width * 0.05f, card.position.y - card.position.height * 0.035f, card.position.width * 0.9f, card.position.height * 0.925f, 2, card.color);

			// Drawing the building icon of the card
			UIManager::AddRectToBatch(card.iconPos.x, card.iconPos.y, card.iconPos.width, card.iconPos.height, 3, TextureManager::GetTexture((TextureManager::TEX_TYPE)(card.bData.type * BuildingEnum::LEVEL_LENGTH + card.bData.level)));

			// Drawing of the card count at the top right corner of the card
			UIManager::AddRectToBatch(card.countIconPos.x, card.countIconPos.y, card.countIconPos.width, card.countIconPos.height, 2);
			UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).S, card.countTextPos.x, card.countTextPos.y, 3, card.countText, COLOR_BLACK);

			//// Drawing border of the card
			//UIManager::AddRectToBatch(card->position.x, card->position.y, card->position.width, card->position.height, 1, card->borderColor);
			//// Drawing the card background
			//UIManager::AddRectToBatch(card->position.x + card->position.width * 0.05f, card->position.y - card->position.height * 0.035f, card->position.width * 0.9f, card->position.height * 0.925f, 2, card->color);

			//// Drawing the building icon of the card
			//UIManager::AddRectToBatch(card->iconPos.x, card->iconPos.y, card->iconPos.width, card->iconPos.height, 3, TextureManager::GetTexture((TextureManager::TEX_TYPE)(card->deckCardData.bData.type * BuildingEnum::LEVEL_LENGTH + card->deckCardData.bData.level)));

			//// Drawing of the card count at the top right corner of the card
			//UIManager::AddRectToBatch(card->countIconPos.x, card->countIconPos.y, card->countIconPos.width, card->countIconPos.height, 2);
			//UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).S, card->count.x, card->count.y, 3, card->deckCardData.countText, COLOR_BLACK);

			//UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).S, card.name.x / AEGetWindowWidth(), card.name.y / AEGetWindowHeight(), 3, card.deckCardData->card.name, COLOR_BLACK);
			//UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).S, card.desc.x / AEGetWindowWidth(), card.desc.y / AEGetWindowHeight(), 1, card.deckCardData->card.desc, COLOR_BLACK);
		}
	}

	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level) {
		BuildingData buildingData = BuildingManager::GetBuildingData(type, BuildingEnum::_1X1, level);
		AddToHand(buildingData);
	}

	void DrawRandomCard(BuildingEnum::LEVEL level) {
		BuildingData buildingData = BuildingManager::GetRandomBuildingData(level);
		AddToHand(buildingData);
	}

	void DrawRandomCard(BuildingEnum::TYPE type) {
		BuildingData buildingData = BuildingManager::GetRandomBuildingData(type);
		AddToHand(buildingData);
	}

	void AddToHand(BuildingData buildingData) {
		if (!hand.empty()) {
			for (Card card : hand) {
				// If the data for it already exists, add to the count
				if (card.bData.type == buildingData.type && card.bData.size == buildingData.size && card.bData.level == buildingData.level) {
					++card.count;
					card.countText = std::to_string(card.count);
					return;
				}
			}
		}

		// Otherwise, add the new card to the deck
		Card newCard = { cardPositionTemplate, buildingData };
		hand.push_back(newCard);

		UpdateHandPositions();
	}

	void UpdateHandPositions() {
		// Update the background size to wrap around the cards
		handBackground.width = hand.size() * cardPositionTemplate.width + (hand.size() + 1) * cardSpacing;
		handBackground.x = -handBackground.width / 2.0f;

		float currentX = handBackground.x + cardSpacing; // Starting from the left

		// Arrange the cards
		for (Card card : hand) {
			card.position.x = currentX;
			card.UpdateComponentPositions();

			currentX += cardPositionTemplate.width + cardSpacing;
		}
	}

	void PlayCard() {
		--selectedCard->count;
		if (selectedCard->count <= 0) {
			RemoveFromHand(selectedCard);
			selectedCard = nullptr;
		}
	}

	void RemoveFromHand(Card* cardToRemove) {
		// hand.remove(*cardToRemove);

		UpdateHandPositions();
	}

	void HandleClick() {
		// Check if any card clicked
		Vec2<int> mousePos = InputManager::GetMousePos();
	}
}
