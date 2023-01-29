
#include <vector>
#include <list>

#include <UIManager.h>
#include <CardManager.h>

#include <ColorTable.h>
#include <Card.h>

#include <iostream>

namespace CardManager {
	u16 startingHandSize;
	std::vector<DeckData> deck;					// Data on all cards in play 
	std::list<Card> hand;						// Data on rendering for current cards held

	UIManager::Transform cardPositionTemplate;	// Rendering data for a generic card
	UIManager::Transform handBackground;		// Rendering data for the hand background
	int cardSpacing;							// Spacing between cards in hand

	Card *selectedCard;							// Pointer to the current card selected by player

	#pragma region Forward Declarations
	void AddToDeck(BuildingData buildingData);
	void AddToHand(DeckData* deckCardData);
	void RemoveFromHand(Card* cardToRemove);
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
	}

	void Free() {
		deck.clear();
		hand.clear();
	}

	void PrepareUIRenderBatch() {
		UIManager::AddRectToBatch(handBackground.x, handBackground.y, handBackground.width, handBackground.height, 0, COLOR_CARD_BACKGROUND);

		// Render each card
		for  (Card& card : hand) {

			UIManager::AddRectToBatch(card.position.x, card.position.y, card.position.width, card.position.height, 1, COLOR_CARD_BORDER);

			switch (card.deckCardData->card.type) {
				case BuildingEnum::RESIDENTIAL:
					UIManager::AddRectToBatch(card.position.x + card.position.width * 0.05f, card.position.y - card.position.height * 0.035f, card.position.width * 0.9f, card.position.height * 0.925f, 2, COLOR_CARD_R);
					break;
				case BuildingEnum::COMMERCIAL:
					UIManager::AddRectToBatch(card.position.x + card.position.width * 0.05f, card.position.y - card.position.height * 0.035f, card.position.width * 0.9f, card.position.height * 0.925f, 2, COLOR_CARD_C);
					break;
				case BuildingEnum::INDUSTRIAL:
					UIManager::AddRectToBatch(card.position.x + card.position.width * 0.05f, card.position.y - card.position.height * 0.035f, card.position.width * 0.9f, card.position.height * 0.925f, 2, COLOR_CARD_I);
					break;
			}

			UIManager::AddRectToBatch(card.icon.x, card.icon.y, card.icon.width, card.icon.height, 3, TextureManager::GetTexture((TextureManager::TEX_TYPE)(card.deckCardData->card.type * BuildingEnum::LEVEL_LENGTH + card.deckCardData->card.level)));

			//UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).S, card.name.x / AEGetWindowWidth(), card.name.y / AEGetWindowHeight(), 3, card.deckCardData->card.name, COLOR_BLACK);
			//UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).S, card.desc.x / AEGetWindowWidth(), card.desc.y / AEGetWindowHeight(), 1, card.deckCardData->card.desc, COLOR_BLACK);
		}
	}

	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level) {
		BuildingData buildingData = BuildingManager::GetBuildingData(type, BuildingEnum::_1X1, level);

		std::cout << "DEBUG: CardManager Get Data of TYPE " << buildingData.type << " ,SIZE " << buildingData.size << " ,LEVEL " << buildingData.level << "\n";

		AddToDeck(buildingData);
	}

	void DrawRandomCard(BuildingEnum::LEVEL level) {
		BuildingData buildingData = BuildingManager::GetRandomBuildingData(level);

		std::cout << "DEBUG: CardManager Get Data of TYPE " << buildingData.type << " ,SIZE " << buildingData.size << " ,LEVEL " << buildingData.level << "\n";

		AddToDeck(buildingData);
	}

	void DrawRandomCard(BuildingEnum::TYPE type) {
		BuildingData buildingData = BuildingManager::GetRandomBuildingData(type);

		std::cout << "DEBUG: CardManager Get Data of TYPE " << buildingData.type << " ,SIZE " << buildingData.size << " ,LEVEL " << buildingData.level << "\n";

		AddToDeck(buildingData);
	}

	void AddToDeck(BuildingData buildingData) {
		for (DeckData _deck : deck) {
			// If the data for it already exists, add to the count
			if (_deck.card.type == buildingData.type && _deck.card.size == buildingData.size && _deck.card.level == buildingData.level) {
				++_deck.count;
				return;
			}
		}
		// Otherwise, add the new card to the deck
		DeckData newDeckCardData = { 1, buildingData };
		deck.push_back(newDeckCardData);
		AddToHand(&deck.back());
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
		--selectedCard->deckCardData->count;
		if (selectedCard->deckCardData->count <= 0) {
			RemoveFromHand(selectedCard);
			selectedCard = nullptr;
		}
	}

	void AddToHand(DeckData *deckCardData) {
		Card newCard = Card(cardPositionTemplate, deckCardData);
		hand.push_back(newCard);

		UpdateHandPositions();
	}

	void RemoveFromHand(Card* cardToRemove) {
		hand.remove(*cardToRemove);

		UpdateHandPositions();
	}
}
