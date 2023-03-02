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

#include <algorithm>

#include <GridManager.h>
#include <CardManager.h>
#include <InputManager.h>
#include <FontManager.h>
#include <UIManager.h>
#include <ColorTable.h>
#include <ScoreManager.h>
#include <iostream>

namespace CardManager {
	bool ScoreCompare(SynergyInfo lhs, SynergyInfo rhs) {
		return lhs.score > rhs.score;
	}

	InfoBox::InfoBox() {
		arrowPos = transform = RenderSystem::Transform();

		transform.size.x = AEGfxGetWinMaxX() * 0.5f;
		transform.size.y = AEGfxGetWinMaxY() * 0.5f;
		arrowPos.size.x = arrowPos.size.y = transform.size.y * 0.1f;
		headerLine.size.x = transform.size.x * 0.8f;
		headerLine.size.y = transform.size.y * 0.01f;

		color = COLOR_CARD_BACKGROUND;
	}

	void InfoBox::Render() {
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, transform.pos.x, transform.pos.y, transform.size.x, transform.size.y, color, 0);
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, arrowPos.pos.x, arrowPos.pos.y, arrowPos.size.x, arrowPos.size.y, color, 0);
		name.Render();
		desc.Render();
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, headerLine.pos.x, headerLine.pos.y, headerLine.size.x, headerLine.size.y, COLOR_BOX_HEADERLINE, 0);
		for (UI::TextBox& sName : sNames) {
			sName.Render();
		}
		for (UI::TextBox& sScore : sScores) {
			sScore.Render();
		}
	}

	void InfoBox::UpdateInfo(Vec2<float> cardPos_TopCenter, const Card* hoveredCard) {
		// Update background & arrow pos
		arrowPos.pos.x = cardPos_TopCenter.x - arrowPos.size.x * 0.5f;
		arrowPos.pos.y = cardPos_TopCenter.y + arrowPos.size.y * 1.2f;

		transform.pos.x = cardPos_TopCenter.x - transform.size.x * 0.5f;
		transform.pos.y = arrowPos.pos.y + transform.size.y;

		// Update name text and pos
		Vec2<float> textPos{ transform.pos.x, transform.pos.y - transform.size.y * 0.1f };
		name = UI::TextBox(textPos, hoveredCard->bData.name, UI::CENTER_JUSTIFY, transform.size.x, 20.0f, COLOR_WHITE);

		// Update desc text and pos
		textPos.y -= transform.size.y * 0.1f;
		desc = UI::TextBox(textPos, hoveredCard->bData.desc, UI::CENTER_JUSTIFY, transform.size.x, 15.0f, COLOR_WHITE);

		// Update header divider pos
		textPos.y -= transform.size.y * 0.03f;
		headerLine.pos.x = textPos.x + (transform.size.x - headerLine.size.x) * 0.5f;
		headerLine.pos.y = textPos.y;

		// Update synergy text and pos
		sNames.clear();
		sScores.clear();
		// First order the list based on the synergy points
		std::vector<SynergyInfo> sData;
		sData.push_back(SynergyInfo(hoveredCard->bData.SynergyResidential, "Residential: ", COLOR_BOX_R));
		sData.push_back(SynergyInfo(hoveredCard->bData.SynergyCommercial, "Commerical: ", COLOR_BOX_C));
		sData.push_back(SynergyInfo(hoveredCard->bData.SynergyIndustrial, "Industrial: ", COLOR_BOX_I));
		sData.push_back(SynergyInfo(hoveredCard->bData.SynergyNature, "Nature: ", COLOR_BOX_N));
		std::sort(sData.begin(), sData.end(), ScoreCompare);

		for (SynergyInfo& sDataElement : sData) {
			textPos.y -= transform.size.y * 0.15f;
			sNames.push_back(UI::TextBox({ textPos.x + transform.size.x * 0.1f, textPos.y }, sDataElement.name, UI::RIGHT_JUSTIFY, transform.size.x * 0.5f, 20.0f, sDataElement.color));
			if (sDataElement.score > 0) {
				sDataElement.color = COLOR_BOX_POSITIVE;
			}
			else if (sDataElement.score < 0) {
				sDataElement.color = COLOR_BOX_NEGATIVE;
			}
			else {
				sDataElement.color = COLOR_BOX_NEUTRAL;
			}
			sNames.push_back(UI::TextBox({ textPos.x + transform.size.x * 0.6f, textPos.y }, std::to_string(sDataElement.score), UI::LEFT_JUSTIFY, transform.size.x * 0.4f, 20.0f, sDataElement.color));
		}
	}
}

namespace CardManager {
	EventSystem::Event<const BuildingData*> onNewCardSelected;	// Event that gets called when player selects/deselects a card
	EventSystem::Event<Vec2<int>> onCardPlaced;					// Event that gets called when player plays a card

	int startingHandSize;
	std::vector<Card> hand;							// Data on rendering for current cards held

	RenderSystem::Transform cardPositionTemplate;	// Rendering data for a generic card
	RenderSystem::Transform handBackground;			// Rendering data for the hand background
	int cardSpacing;								// Spacing between cards in hand
	InfoBox cardInfoBox;							// Displays full card information for the selected card

	Card* selectedCard;								// Pointer to the current card selected by the player

	Card* hoveredCard;								// Pointer to the current card hovered over by the player
	float hoverTimeShowThreshold;					// How long the player needs to hover over a card to trigger the box info
	float hoverTimeHideThreshold;					// How long the player needs to NOT hover over a card to hide the box info
	float hoverTimeElapsed;							// How long the player is currently hovering over a card for

	bool spawnMergeBuilding;

#pragma region Forward Declarations
	void AddToHand(BuildingData cardData);
	void RemoveFromHand(Card* cardToRemove);
	void HandleClick();
	void UpdateHandPositions();
	void GiveCardOnThreshold();
	void GiveCardOnMerge();
	void GiveRandL1Card();
	void GiveRandL2Card();
	void GiveRandL3Card();
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

		// Initialize card information box
		cardInfoBox = InfoBox();
		hoverTimeShowThreshold = 0.55f;
		hoverTimeHideThreshold = 0.4f;
		hoverTimeElapsed = 0.0f;

		// Fill hand with 5 starting cards
		DrawCard(BuildingEnum::RESIDENTIAL, BuildingEnum::L1);
		DrawCard(BuildingEnum::RESIDENTIAL, BuildingEnum::L1);
		DrawCard(BuildingEnum::RESIDENTIAL, BuildingEnum::L1);
		DrawCard(BuildingEnum::COMMERCIAL, BuildingEnum::L1);
		DrawCard(BuildingEnum::INDUSTRIAL, BuildingEnum::L1);

		InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleClick);

		// Debugging: Spawn cards
		InputManager::SubscribeToKey(AEVK_F, InputManager::TRIGGERED, GiveRandL1Card);
		InputManager::SubscribeToKey(AEVK_G, InputManager::TRIGGERED, GiveRandL2Card);
		InputManager::SubscribeToKey(AEVK_H, InputManager::TRIGGERED, GiveRandL3Card);
		ScoreManger::onLevelChange.Subscribe(GiveCardOnThreshold);
		GridManager::onMergeBuildings.Subscribe(GiveCardOnMerge);

		spawnMergeBuilding = false;
	}

	void Update() {
		if (spawnMergeBuilding) {
			GiveRandL2Card();
			spawnMergeBuilding = false;
		}

		// If mouse hovers over any card long enough, show its information
		Vec2<float> mousePos = { (float)InputManager::GetMousePos().x - AEGfxGetWinMaxX(), -((float)InputManager::GetMousePos().y - AEGfxGetWinMaxY()) };
		Vec2<float> cardPos, cardSize;

		for (Card& card : hand) {
			cardPos = { card.position.pos.x, card.position.pos.y };
			cardSize = { card.position.size.x, card.position.size.y };

			// Player is hovering over a card!
			if (IsPointWithinRect(mousePos, cardPos, cardSize)) {
				hoverTimeElapsed += AEFrameRateControllerGetFrameTime(); // Add to hover time

				if (hoverTimeElapsed >= hoverTimeShowThreshold) { // If the player hovers long enough, show the box
					if (hoveredCard != &card) {
						hoveredCard = &card;
						cardInfoBox.UpdateInfo(Vec2<float>{hoveredCard->position.pos.x + hoveredCard->position.size.x * 0.5f, hoveredCard->position.pos.y}, hoveredCard);
					}
					hoverTimeElapsed = hoverTimeShowThreshold; // Cap the hover time
				}
				return; // Then exit this function
			}
		}

		// If the player is not hovering over any card
		if (hoverTimeElapsed > 0.0f) {
			hoverTimeElapsed -= AEFrameRateControllerGetFrameTime(); // Reduce the hover time elapsed if needed
			if (hoverTimeElapsed <= hoverTimeHideThreshold) { // If hover time is lesser than hide time, remove the hovered card status
				hoveredCard = nullptr;
				hoverTimeElapsed = 0.0f;
			}
		}
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
			card.countText.Render();

			card.nameText.Render();
		}

		// Render the info box if needed
		if (hoveredCard) {
			cardInfoBox.Render();
		}
	}

	void GiveCardOnThreshold() {
		for (int i{ 0 }; i < 4; ++i) {
			GiveRandL1Card();
		}
	}

	void GiveCardOnMerge(){
		spawnMergeBuilding = true;
		//GiveRandL2Card();
	}

	void GiveRandL1Card() {
		DrawRandomCard(BuildingEnum::L1);
	}

	void GiveRandL2Card() {
		DrawRandomCard(BuildingEnum::L2);
	}

	void GiveRandL3Card() {
		DrawRandomCard(BuildingEnum::L3);
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

	void DrawRandomCard() {
		BuildingData buildingData = BuildingManager::GetRandomBuildingData();
		AddToHand(buildingData);
	}

	void AddToHand(BuildingData buildingData) {
		if (!hand.empty()) {
			for (Card& card : hand) {
				// If the data for it already exists, add to the count
				if (card == buildingData) {
					++card.count;
					card.UpdateCountText();
					return;
				}
			}
		}

		// Otherwise, add the new card to the deck
		hand.emplace_back(cardPositionTemplate, buildingData);

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
		Vec2<int> mousePos = InputManager::GetMousePos();
		if (!GridManager::isCellSafe(IsometricGrid::ScreenPosToIso(mousePos))) {
			return;
		}

		onCardPlaced.Invoke(mousePos);

		--selectedCard->count;
		selectedCard->UpdateCountText();

		if (selectedCard->count <= 0) {
			RemoveFromHand(selectedCard);

			selectedCard = nullptr;
			onNewCardSelected.Invoke(nullptr); // Invoke the card deselected event
		}
	}

	void RemoveFromHand(Card* cardToRemove) {
		size_t index = 0; // Max size

		for (; index < hand.size(); ++index) {
			if (*cardToRemove == hand[index]) {
				break;
			}
		}

		hand.erase(hand.begin() + index);
		UpdateHandPositions();
	}

	void HandleClick() {
		Vec2<float> mousePos = { (float)InputManager::GetMousePos().x - AEGfxGetWinMaxX(), -((float)InputManager::GetMousePos().y - AEGfxGetWinMaxY()) };
		Vec2<float> cardPos, cardSize;

		// Check if any card clicked
		for (Card& card : hand) {
			cardPos = { card.position.pos.x, card.position.pos.y };
			cardSize = { card.position.size.x, card.position.size.y };

			// If a card is clicked, update the selected card and border color
			if (IsPointWithinRect(mousePos, cardPos, cardSize)) {
				if (selectedCard) {
					selectedCard->borderColor = COLOR_CARD_BORDER;

					// Check if card clicked is the same card (deselecting)
					if (selectedCard == &card) {
						selectedCard = nullptr;
						onNewCardSelected.Invoke(nullptr); // Invoke the card deselected event
						return;
					}
				}

				selectedCard = &card; // Selected card becomes the new card
				selectedCard->borderColor = COLOR_CARD_BORDER_SELECTED;

				onNewCardSelected.Invoke(&selectedCard->bData); // Invoke the card selected event
				return;
			}
		}

		// Otherwise check if the player has played the card on the grid
		if (selectedCard) {
			PlayCard();
		}
	}

	void Free() {
		hand.clear();
		InputManager::UnsubscribeKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleClick);
		InputManager::UnsubscribeKey(AEVK_F, InputManager::TRIGGERED, GiveRandL1Card);
		InputManager::UnsubscribeKey(AEVK_G, InputManager::TRIGGERED, GiveRandL2Card);
		InputManager::UnsubscribeKey(AEVK_H, InputManager::TRIGGERED, GiveRandL3Card);
		ScoreManger::onLevelChange.Unsubscribe(GiveCardOnThreshold);
		GridManager::onMergeBuildings.Unsubscribe(GiveCardOnMerge);
	}
}
