
#include <vector>
#include <list>

#include <UIManager.h>
#include <CardManager.h>

#include <ColorTable.h>
#include <Card.h>

namespace CardManager {
	u16 startingHandSize;
	std::vector<DeckData> deck;					// Data on all cards in play 
	std::list<Card> hand;						// Data on rendering for current cards held

	UIManager::Transform cardPositionTemplate;	// Rendering data for a generic card
	UIManager::Transform handBackground;		// Rendering data for the hand background

	Card *selectedCard;							// Pointer to the current card selected by player

	#pragma region Forward Declarations
	void AddToDeck(BuildingData buildingData);
	void AddToHand();
	void RemoveFromHand(Card* cardToRemove);
	#pragma endregion

	void Initialize() {
		// TODO: Use JSON for all these data
		startingHandSize = 5;
		selectedCard = nullptr;

		handBackground.width = AEGfxGetWinMaxX();
		handBackground.height = AEGfxGetWinMaxY() * 0.25f;

		handBackground.x = -handBackground.width / 2.0f;
		handBackground.y = AEGfxGetWinMinY() * 0.9f + handBackground.height;

		cardPositionTemplate.height = handBackground.height * 0.9f;
		cardPositionTemplate.width = cardPositionTemplate.height * 0.5f;

		// Fill hand with 5 random level 1 cards
		for (u16 count = 0; count < startingHandSize; ++count) {
			DrawRandomCard(BuildingEnum::L1);
		}
	}

	void Free() {
		deck.clear();
		hand.clear();
	}

	void PrepareUIRenderBatch() {
		UIManager::AddRectToBatch(handBackground.x, handBackground.y, handBackground.width, handBackground.height, 0, COLOR_CARD_BACKGROUND);
	}

	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level) {
		BuildingData buildingData = BuildingManager::GetBuildingData(type, BuildingEnum::_1X1, level);
		AddToDeck(buildingData);
	}

	void DrawRandomCard(BuildingEnum::LEVEL level) {
		BuildingData buildingData = BuildingManager::GetRandomBuildingData(level);
		AddToDeck(buildingData);
	}

	void DrawRandomCard(BuildingEnum::TYPE type) {
		BuildingData buildingData = BuildingManager::GetRandomBuildingData(type);
		AddToDeck(buildingData);
	}

	void AddToDeck(BuildingData buildingData) {
		for (DeckData _deck : deck) {
			// If the data for it already exists, add to the count
			if (_deck.card.type == buildingData.type && _deck.card.size == buildingData.size && _deck.card.level == buildingData.level) {
				++_deck.count;
				// UpdateCardPositions();
				return;
			}
		}
		// Otherwise, add the new card to the deck
		DeckData newCard = { 1, buildingData };
		deck.push_back(newCard);
		// UpdateCardPositions();
	}

	void UpdateHandPositions() {
		for (Card card : hand) {

		}
	}

	void PlayCard() {
		for (DeckData _deck : deck) {
			// Find the card in the deck
			if (_deck.card.type == selectedCard->buildingData.type && _deck.card.size == selectedCard->buildingData.size && _deck.card.level == selectedCard->buildingData.level) {
				--_deck.count;
				if (_deck.count <= 0) {
					RemoveFromHand(selectedCard);
					selectedCard = nullptr;
				}
				return;
			}
		}
	}

	void AddToHand() {

	}

	void RemoveFromHand(Card* cardToRemove) {

	}
}
