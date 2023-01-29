
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
		handBackground.y = AEGfxGetWinMinY() * 0.9f + handBackground.height;

		cardPositionTemplate.height = handBackground.height * 0.9f;
		cardPositionTemplate.width = cardPositionTemplate.height * 0.75f;

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

		// Render each card
		for  (Card& card : hand) {
			UIManager::AddRectToBatch(card.position.x, card.position.y, card.position.width, card.position.height, 1, COLOR_CARD_BORDER);
			UIManager::AddRectToBatch(card.position.x + card.position.width * 0.025f, card.position.y - card.position.height * 0.025f, card.position.width * 0.95f, card.position.height * 0.95f, 1, COLOR_CARD);
			//UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).S, card.name.x, card.name.y, 1, card.deckCardData->card.name, COLOR_BLACK);
			//UIManager::AddTextToBatch(UIManager::GetFont(UIManager::ROBOTO).S, card.desc.x, card.desc.y, 1, card.deckCardData->card.desc, COLOR_BLACK);
		}
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
				return;
			}
		}
		// Otherwise, add the new card to the deck
		DeckData newDeckCardData = { 1, buildingData };
		deck.push_back(newDeckCardData);
		AddToHand(&newDeckCardData);
	}

	void UpdateHandPositions() {

	}

	void PlayCard() {
		--selectedCard->deckCardData->count;
		if (selectedCard->deckCardData->count <= 0) {
			RemoveFromHand(selectedCard);
			selectedCard = nullptr;
		}
	}

	void AddToHand(DeckData *deckCardData) {
		cardPositionTemplate.x = handBackground.x;
		cardPositionTemplate.y = handBackground.y;
		Card newCard = Card(cardPositionTemplate, deckCardData);
		hand.push_back(newCard);
	}

	void RemoveFromHand(Card* cardToRemove) {
		hand.remove(*cardToRemove);
	}
}
