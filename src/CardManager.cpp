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

///////////////////////////////////////////////////////////////////////////
#include <algorithm>

#include <GridManager.h>
#include <CardManager.h>
#include <InputManager.h>
#include <ScoreManager.h>
#include <FontManager.h>
#include <UIManager.h>
#include <DebugManager.h>
#include <ColorTable.h>
#include <iostream>
///////////////////////////////////////////////////////////////////////////
// Card info box related functions
namespace CardManager {
	// Default constructor for info box
	InfoBox::InfoBox() {
		arrowPos = transform = RenderSystem::Transform();

		// Set its size to be 25% of the screen size
		transform.size.x = AEGfxGetWinMaxX() * 0.5f;
		transform.size.y = AEGfxGetWinMaxY() * 0.5f;
		// The arrow at the bottom is 10% of the box's size
		arrowPos.size.x = arrowPos.size.y = transform.size.y * 0.1f;
		// The header dividing line's height is 1% of the box's height and 80% of the box's width
		headerLine.size.x = transform.size.x * 0.8f;
		headerLine.size.y = transform.size.y * 0.01f;

		color = COLOR_CARD_BACKGROUND;
	}

	// Draws the entire info box objects and its texts
	void InfoBox::Render() {
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, transform.pos.x, transform.pos.y, transform.size.x, transform.size.y, color, 0);
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, headerLine.pos.x, headerLine.pos.y, headerLine.size.x, headerLine.size.y, COLOR_BOX_HEADERLINE, 4);
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, arrowPos.pos.x, arrowPos.pos.y, arrowPos.size.x, arrowPos.size.y, color, 0);

		name.Render();
		desc.Render();

		for (UI::TextBox& sName : sNames) {
			sName.Render();
		}
		for (UI::TextBox& sScore : sScores) {
			sScore.Render();
		}
	}

	///////////////////////////////////////////////////////////////////////
	// Used to sort the synergy scores from biggest to smallest
	bool ScoreCompare(SynergyInfo lhs, SynergyInfo rhs) {
		return lhs.score > rhs.score;
	}

	// Update the info box position and text to show the information of the hovered card
	void InfoBox::UpdateInfo(Vec2<float> cardPos_TopCenter, const Card* hoveredCard) {
		// Move the arrow to the top center of the card
		arrowPos.pos.x = cardPos_TopCenter.x - arrowPos.size.x * 0.5f;   // Anchor is bottom left, so offset by 50% of size
		arrowPos.pos.y = cardPos_TopCenter.y + arrowPos.size.y * 1.2f;   // Leave a 20% size gap between the card

		transform.pos.x = cardPos_TopCenter.x - transform.size.x * 0.5f; // Anchor is bottom left, so offset by 50% of size
		transform.pos.y = arrowPos.pos.y + transform.size.y;

		// Update name text and pos
		Vec2<float> textPos{ transform.pos.x, transform.pos.y - transform.size.y * 0.1f }; // Text height is 10% of the box
		name = UI::TextBox(textPos, hoveredCard->bData.name, UI::CENTER_JUSTIFY, transform.size.x, 20.0f, COLOR_WHITE);

		// Update desc text and pos
		textPos.y -= transform.size.y * 0.1f;  // Offset the height by 10% of the box
		desc = UI::TextBox(textPos, hoveredCard->bData.desc, UI::CENTER_JUSTIFY, transform.size.x, 15.0f, COLOR_WHITE);

		// Update header divider pos
		textPos.y -= transform.size.y * 0.03f; // Offset the height by 3% of the box
		headerLine.pos.x = textPos.x + (transform.size.x - headerLine.size.x) * 0.5f;
		headerLine.pos.y = textPos.y;

		// Update synergy text and pos, first clear the old information
		sNames.clear();
		sScores.clear();

		// Then add the information of all the synergy into a temp vector for sorting
		std::vector<SynergyInfo> sData;
		sData.push_back(SynergyInfo(hoveredCard->bData.SynergyResidential, "Residential: ", COLOR_BOX_R));
		sData.push_back(SynergyInfo(hoveredCard->bData.SynergyCommercial, "Commerical: ", COLOR_BOX_C));
		sData.push_back(SynergyInfo(hoveredCard->bData.SynergyIndustrial, "Industrial: ", COLOR_BOX_I));
		sData.push_back(SynergyInfo(hoveredCard->bData.SynergyNature, "Nature: ", COLOR_BOX_N));

		std::sort(sData.begin(), sData.end(), ScoreCompare); // sorting the list based on the synergy points

		// Update the position of each text (name + points) then add it to the sNames vector
		for (SynergyInfo& sDataElement : sData) {
			textPos.y -= transform.size.y * 0.15f; // Offset the height by 15% of the box

			// Add the name part first
			sNames.push_back(UI::TextBox({ textPos.x + transform.size.x * 0.1f, textPos.y }, sDataElement.name, UI::RIGHT_JUSTIFY, transform.size.x * 0.5f, 20.0f, sDataElement.color));

			// Then add the points/score part second
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
///////////////////////////////////////////////////////////////////////////
// Card Manager related functions
namespace CardManager {
	EventSystem::Event<const BuildingData*> onNewCardSelected;	// Event that gets called when player selects/deselects a card
	EventSystem::Event<Vec2<int>> onCardPlaced;					// Event that gets called when player plays a card
	EventSystem::Event<void>	  onCardPlacedVoid;				// Event that gets called when player plays a card
	EventSystem::Event<void> onHandEmpty;						//Event that gets called when player runs out of cards

	int startingHandSize = 5;						// How many cards should the player start with
	std::vector<Card> hand;							// Data on rendering for current cards held

	RenderSystem::Transform cardPositionTemplate;	// Rendering data for a generic card
	RenderSystem::Transform handBackground;			// Rendering data for the hand background
	float cardSpacing;								// Spacing between cards in hand
	InfoBox cardInfoBox;							// Displays full card information for the selected card

	Card* selectedCard;								// Pointer to the current card selected by the player

	Card* hoveredCard;								// Pointer to the current card hovered over by the player
	float hoverTimeShowThreshold = 0.55f;			// How long the player needs to hover over a card to trigger the box info
	float hoverTimeHideThreshold = 0.4f;			// How long the player needs to NOT hover over a card to hide the box info
	float hoverTimeElapsed;							// How long the player is currently hovering over a card for

	bool spawnMergeBuilding;
	bool isClickable;

	///////////////////////////////////////////////////////////////////////
	// Forward Declarations
	void AddToHand(BuildingData cardData);
	void RemoveFromHand(Card* cardToRemove);
	void HandleClick();
	void UpdateHandPositions();
	void GiveCardOnThreshold();
	void GiveCardOnMerge();
	void GiveRandL1Card();
	void GiveRandL2Card();
	void GiveRandL3Card();
	void ToggleClickableOff();
	///////////////////////////////////////////////////////////////////////
	// Set up the hand properly at the start of the level

	// By default game will start with 5 cards (3 house, 1 shop, 1 factory)
	void Initialize() {
		std::vector<BuildingData> baseStartingHand;
		baseStartingHand.push_back(BuildingManager::GetBuildingData(BuildingEnum::RESIDENTIAL, Vec2<int>{ 1, 1 }, BuildingEnum::L1));
		baseStartingHand.push_back(BuildingManager::GetBuildingData(BuildingEnum::RESIDENTIAL, Vec2<int>{ 1, 1 }, BuildingEnum::L1));
		baseStartingHand.push_back(BuildingManager::GetBuildingData(BuildingEnum::RESIDENTIAL, Vec2<int>{ 1, 1 }, BuildingEnum::L1));
		baseStartingHand.push_back(BuildingManager::GetBuildingData(BuildingEnum::COMMERCIAL, Vec2<int>{ 1, 1 }, BuildingEnum::L1));
		baseStartingHand.push_back(BuildingManager::GetBuildingData(BuildingEnum::INDUSTRIAL, Vec2<int>{ 1, 1 }, BuildingEnum::L1));
		Initialize(baseStartingHand);
	}

	// Start game with a custom starting hand
	void Initialize(std::vector<BuildingData> const& startingHand) {
		selectedCard = nullptr;								// Deselect held card, if any

		handBackground.size.x = (float)AEGfxGetWinMaxX();			// The width of the hand BG default to half the screen width
		handBackground.size.y = (float)AEGfxGetWinMaxY() * 0.4f;	// and 40% of the screen height

		handBackground.pos.x = -handBackground.size.x / 2.0f;					 // The hand BG should be at the middle
		handBackground.pos.y = (float)AEGfxGetWinMinY() * 0.95f + handBackground.size.y; // and bottom of the screen

		cardSpacing = (float)AEGfxGetWinMaxX() * 0.05f;			// The gap between cards should at least be 2.5% of the screen width apart

		cardPositionTemplate.size.y = handBackground.size.y * 0.8f;				 // Keep the height of each card to 90% of the hand BG
		cardPositionTemplate.size.x = cardPositionTemplate.size.y * 0.75f;		 // Card width to height ratio is 3:4
		cardPositionTemplate.pos.y = handBackground.pos.y - (handBackground.size.y - cardPositionTemplate.size.y) / 2.0f; // Set the y-pos so the card is in the middle of hand BG

		// Initialize card information box
		cardInfoBox = InfoBox();
		hoverTimeElapsed = 0.0f; // Reset the elapsed time to zero

		// Fill starting hand 
		hand.reserve(32);										// Reserve enough space for 32 seperate cards to prevent shifting
		for (BuildingData const& card : startingHand) {
			AddToHand(card);
		}

		InputManager::SubscribeToKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleClick);
		ScoreManager::onLevelChange.Subscribe(GiveCardOnThreshold);
		GridManager::onMergeBuildings.Subscribe(GiveCardOnMerge);

		CardManager::onHandEmpty.Subscribe(ToggleClickableOff);

		// Debugging: Press key to spawn cards
		InputManager::SubscribeToKey(AEVK_F, InputManager::TRIGGERED, GiveRandL1Card);
		InputManager::SubscribeToKey(AEVK_G, InputManager::TRIGGERED, GiveRandL2Card);
		InputManager::SubscribeToKey(AEVK_H, InputManager::TRIGGERED, GiveRandL3Card);

		spawnMergeBuilding = false;
		isClickable = true;
	}
	
	// Every frame check if the info box should show
	void Update() {
		// If mouse hovers over any card long enough, show its information
		// First, get the mouse pos
		Vec2<float> mousePos = { (float)InputManager::GetMousePos().x - AEGfxGetWinMaxX(), -((float)InputManager::GetMousePos().y - AEGfxGetWinMaxY()) };
		Vec2<float> cardPos, cardSize;

		// Check each card to see if the player is hovering over it
		for (Card& card : hand) {
			cardPos = { card.position.pos.x, card.position.pos.y };
			cardSize = { card.position.size.x, card.position.size.y };

			// If the player is hovering over a card!
			if (IsPointWithinRect(mousePos, cardPos, cardSize)) {
				hoverTimeElapsed += (f32)AEFrameRateControllerGetFrameTime(); // Add to hover time

				if (hoverTimeElapsed >= hoverTimeShowThreshold) {			  // If the player hovers long enough, show the info box
					if (hoveredCard != &card) {
						hoveredCard = &card;
						cardInfoBox.UpdateInfo(Vec2<float>{hoveredCard->position.pos.x + hoveredCard->position.size.x * 0.5f, hoveredCard->position.pos.y}, hoveredCard);
					}
					hoverTimeElapsed = hoverTimeShowThreshold;				  // Cap the hover time
				}
				return; // Then exit this function
			}
		}

		// Otherwise, if the player is not hovering over any card
		if (hoverTimeElapsed > 0.0f) {
			hoverTimeElapsed -= (f32)AEFrameRateControllerGetFrameTime(); // Reduce the hover time elapsed if needed
			if (hoverTimeElapsed <= hoverTimeHideThreshold) {			  // If hover time is lesser than hide time, remove the hovered card status
				hoveredCard = nullptr;
				hoverTimeElapsed = 0.0f;
			}
		}
	}

	// Sends the information of everything to rendersystem for rendering
	void PrepareUIRenderBatch() {
		// Render the hand BG
		if (hand.size() > 0) {
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, handBackground.pos.x, handBackground.pos.y, handBackground.size.x, handBackground.size.y, TextureManager::BLANK_PROMPT);
		}

		// Render each card
		for (Card card : hand) {
			// Drawing border of the card
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, card.position.pos.x, card.position.pos.y, card.position.size.x, card.position.size.y, card.borderColor, 1);

			// Drawing the card background
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, card.position.pos.x + card.position.size.x * 0.05f, card.position.pos.y - card.position.size.y * 0.035f, card.position.size.x * 0.9f, card.position.size.y * 0.925f, TextureManager::CARD, 2);

			// Drawing the building icon of the card
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, card.iconPos.pos.x, card.iconPos.pos.y, card.iconPos.size.x, card.iconPos.size.y, card.bData.TextureID, 3);

			// Drawing of the card count at the top right corner of the card
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, card.countIconPos.pos.x, card.countIconPos.pos.y, card.countIconPos.size.x, card.countIconPos.size.y, TextureManager::CARD_COUNT, 4);
			card.countText.Render();
			card.nameText.Render();
		}

		// Render the info box if needed
		if (hoveredCard) {
			cardInfoBox.Render();
		}
	}

	///////////////////////////////////////////////////////////////////////
	// Player hand & cards functions

	std::vector<Card> const& GetCurrentHand() {
		return hand;
	}

	// Everytime the player reaches the next threshold, give 4 random L1 cards
	void GiveCardOnThreshold() {
		for (int i{ 0 }; i < 4; ++i) {
			DrawRandomCard(BuildingEnum::L1);
		}
	}

	// Everytime the player merges, set the flag to give more cards to true
	void GiveCardOnMerge() {
		DrawRandomCard(BuildingEnum::L2);
	}

	void GiveRandL1Card() {
		if (!Debug::IsDebugModeOn()) return;
		DrawRandomCard(BuildingEnum::L1);
	}

	void GiveRandL2Card() {
		if (!Debug::IsDebugModeOn()) return;
		DrawRandomCard(BuildingEnum::L2);
	}

	void GiveRandL3Card() {
		if (!Debug::IsDebugModeOn()) return;
		DrawRandomCard(BuildingEnum::L3);
	}

	// Adds a card of the given type and level to the player's hand
	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level) {
		BuildingData buildingData = BuildingManager::GetBuildingData(type, Vec2<int>{1, 1}, level);
		AddToHand(buildingData);
	}

	// Adds a random card of the given level to the player's hand
	void DrawRandomCard(BuildingEnum::LEVEL level) {
		BuildingData buildingData = BuildingManager::GetRandomBuildingData(level);
		AddToHand(buildingData);
	}

	// Adds a random card of the given type to the player's hand
	void DrawRandomCard(BuildingEnum::TYPE type) {
		BuildingData buildingData = BuildingManager::GetRandomBuildingData(type);
		AddToHand(buildingData);
	}

	// Add a random card to the player's hand
	void DrawRandomCard() {
		BuildingData buildingData = BuildingManager::GetRandomBuildingData();
		AddToHand(buildingData);
	}

	// The actual adding to the hand implementation, first it takes the data given
	void AddToHand(BuildingData buildingData) {
		// and checks if already exists in the player's hand
		if (!hand.empty()) {
			for (Card& card : hand) {
				// If it does exist, add to the count then quit
				if (card == buildingData) {
					++card.count;
					card.UpdateCountText();
					return;
				}
			}
		}

		// Otherwise, add the new card to the deck
		hand.emplace_back(cardPositionTemplate, buildingData);
		// Now that there is a new card, recalculate the positions for each card to fit
		UpdateHandPositions();
	}

	// Update the positions of all the cards to keep it at the center of the screen
	void UpdateHandPositions() {
		// Update the background size to fit nicely around the all the cards
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

	// When the player clicks on a valid cell, a card is "played"
	void PlayCard() {
		// First check if the cell clicked is valid
		Vec2<int> mousePos = InputManager::GetMousePos();
		if (!GridManager::isCellSafe(GridManager::ScreenPosToIso(mousePos))) {
			return;
		}

		// If it is, reduce the card count and update its text
		--selectedCard->count;
		selectedCard->UpdateCountText();

		onCardPlaced.Invoke(mousePos);		   // Let the world know a card has been played
		onCardPlacedVoid.Invoke();

		if (selectedCard->count <= 0) {		   // If the card has run out, remove it from the hand
			RemoveFromHand(selectedCard);
			selectedCard = nullptr;
			onNewCardSelected.Invoke(nullptr); // Invoke the card deselected event
		}
	}

	// Removes the selected card from the hand then updates the positions of the remaining cards
	void RemoveFromHand(Card* cardToRemove) {
		size_t index = 0;

		for (; index < hand.size(); ++index) { // Find the card to remove
			if (*cardToRemove == hand[index]) {
				break;
			}
		}

		hand.erase(hand.begin() + index);

		UpdateHandPositions();
	}

	// When the player clicks (for any reason)
	void HandleClick() {
		if (!isClickable) return;

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
		if (!spawnMergeBuilding && hand.empty()) {
			onHandEmpty.Invoke();
		}
	}

	// At the end of the level free and unsubscribe from everything
	void Free() {
		selectedCard = nullptr;
		onNewCardSelected.Invoke(nullptr); // Invoke the card deselected event

		hand.clear();

		InputManager::UnsubscribeKey(AEVK_LBUTTON, InputManager::TRIGGERED, HandleClick);
		InputManager::UnsubscribeKey(AEVK_F, InputManager::TRIGGERED, GiveRandL1Card);
		InputManager::UnsubscribeKey(AEVK_G, InputManager::TRIGGERED, GiveRandL2Card);
		InputManager::UnsubscribeKey(AEVK_H, InputManager::TRIGGERED, GiveRandL3Card);

		ScoreManager::onLevelChange.Unsubscribe(GiveCardOnThreshold);
		GridManager::onMergeBuildings.Unsubscribe(GiveCardOnMerge);
	}


	//HELPER FUNCTIONS 

	void ToggleClickable(bool clickable) {
		isClickable = clickable;
	}

	void ToggleClickableOff() {
		isClickable = false;
		CardManager::onHandEmpty.Unsubscribe(ToggleClickableOff);
	}

	float GetCardTemplateXPos(){
		return handBackground.pos.x;
	}
}
