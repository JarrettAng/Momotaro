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

#include <CardManager.h>
#include <InputManager.h>
#include <FontManager.h>
#include <UIManager.h>

#include <ColorTable.h>
#include <Card.h>

#include <iostream>

namespace CardManager {
	int startingHandSize;
	std::vector<Card> hand;							// Data on rendering for current cards held

	RenderSystem::Transform cardPositionTemplate;	// Rendering data for a generic card
	RenderSystem::Transform handBackground;			// Rendering data for the hand background
	int cardSpacing;								// Spacing between cards in hand

	Card* selectedCard;								// Pointer to the current card selected by player

#pragma region Forward Declarations
	void AddToHand(BuildingData cardData);
	void RemoveFromHand(Card* cardToRemove);
	void HandleClick();
	void UpdateHandPositions();
#pragma endregion

	void Initialize() {
		// TODO: Use JSON for all these data
		startingHandSize = 5;
		selectedCard = nullptr;

		handBackground.size.x = AEGfxGetWinMaxX();
		handBackground.size.y = AEGfxGetWinMaxY() * 0.25f;

		handBackground.pos.x = -handBackground.size.x / 2.0f;
		handBackground.pos.y = AEGfxGetWinMinY() * 0.95f + handBackground.size.y;

		cardSpacing = AEGfxGetWinMaxX() * 0.025f;

		cardPositionTemplate.size.y = handBackground.size.y * 0.9f;
		cardPositionTemplate.size.x = cardPositionTemplate.size.y * 0.75f;
		cardPositionTemplate.pos.y = handBackground.pos.y - (handBackground.size.y - cardPositionTemplate.size.y) / 2.0f;

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
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, handBackground.pos.x, handBackground.pos.y, handBackground.size.x, handBackground.size.y, COLOR_CARD_BACKGROUND);

		// Render each card
		for (Card card : hand) {
			// Drawing border of the card
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, card.position.pos.x, card.position.pos.y, card.position.size.x, card.position.size.y, card.borderColor, 1);
			// Drawing the card background
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, card.position.pos.x + card.position.size.x * 0.05f, card.position.pos.y - card.position.size.y * 0.035f, card.position.size.x * 0.9f, card.position.size.y * 0.925f, card.color, 2);

			// Drawing the building icon of the card
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, card.iconPos.pos.x, card.iconPos.pos.y, card.iconPos.size.x, card.iconPos.size.y, card.bData.TextureID, 3);

			// Drawing of the card count at the top right corner of the card
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, card.countIconPos.pos.x, card.countIconPos.pos.y, card.countIconPos.size.x, card.countIconPos.size.y, { 1,1,1,1 }, 4);
			RenderSystem::AddTextToBatch(RenderSystem::UI_BATCH, card.countTextPos.pos.x, card.countTextPos.pos.y, FontManager::GetFont(FontManager::ROBOTO), 20, card.countText, 4, COLOR_BLACK);
		}
	}

	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level) {
		BuildingData buildingData = BuildingManager::GetBuildingData(type, Vec2<int>{1, 1}, level);
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
			for (Card& card : hand) {
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
		handBackground.size.x = hand.size() * cardPositionTemplate.size.x + (hand.size() + 1) * cardSpacing;
		handBackground.pos.x = -handBackground.size.x / 2.0f;

		float currentX = handBackground.pos.x + cardSpacing; // Starting from the left

		// Arrange the cards
		for (Card& card : hand) {
			card.position.pos.x = currentX;
			card.UpdateComponentPositions();

			currentX += cardPositionTemplate.size.x + cardSpacing;
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
