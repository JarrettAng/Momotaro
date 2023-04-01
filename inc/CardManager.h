/*!************************************************************************
\file:          CardManager.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This header file holds the data of each card in the player's hand and handles
the creation/deletion of the cards.

The functions include:
- Initialize
Draws the cards for the player to play with at the same of the game scene
- PrepareUIRenderBatch
Sends data to UIManager to render the cards
- DrawCard
Draws a draw into the player's hand of a given type, level, size
- DrawRandomCard
Draws a random draw into the player's hand
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <string>
#include <vector>

#include <Card.h>
#include <BuildingManager.h>
#include <EventSystem.h>
///////////////////////////////////////////////////////////////////////////

namespace CardManager {
	// This class handles the "Additional info box" that appears above the card
	// that the player has hovered over long enough
	class InfoBox {
	public:
		///////////////////////////////////////////////////////////////////
		// Default constructor
		InfoBox();

		///////////////////////////////////////////////////////////////////
		// Member functions
		void Render();															 // Draws the info box
		void UpdateInfo(Vec2<float> cardPos_TopCenter, const Card* hoveredCard); // Changes the pos & info in the info box

	private:
		///////////////////////////////////////////////////////////////////
		// Data members
		
		// Positions of the background
		RenderSystem::Transform transform;  // The position of the window
		RenderSystem::Transform arrowPos;   // The triangle arrow at the bottom of the window
		RenderSystem::Transform headerLine; // White line seperating header from stats

		// Texts
		UI::TextBox name;
		UI::TextBox desc;
		std::vector<UI::TextBox> sNames;	// All the synergy name types (e.g. Residential)
		std::vector<UI::TextBox> sScores;	// All the synergy points types (e.g. -1, 3)

		Vec4<float> color;
	};
	///////////////////////////////////////////////////////////////////////
	// Used to store the information of all synergy types to be sorted in descending order
	struct SynergyInfo {
		int			score; // How much the other building type synergizes with this type
		std::string name;  // Name of the other building type
		Vec3<float> color; // Color of the text

		// Non-default constructor
		SynergyInfo(int _score, std::string _name, Vec3<float> _color) {
			score = _score;
			name = _name;
			color = _color;
		}
	};
}

namespace CardManager {
	///////////////////////////////////////////////////////////////////////
	// Events
	extern EventSystem::Event<const BuildingData*> 	onNewCardSelected;
	extern EventSystem::Event<Vec2<int>>		   	onCardPlaced;
	extern EventSystem::Event<void>					onCardPlacedVoid;
	extern EventSystem::Event<void>					onHandEmpty;

	///////////////////////////////////////////////////////////////////////
	
	void Initialize();												// Start the game with the default 5 cards hand
	void Initialize(std::vector<BuildingData> const& startingHand); // Start the game with a custom starting hand
	void Update();

	void PrepareUIRenderBatch(); // Sends the information of everything to rendersystem for rendering

	std::vector<Card> const& GetCurrentHand();		// Get all the cards currently in the player's hand

	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level);

	void DrawRandomCard(BuildingEnum::LEVEL level); // Draws a random card of this level
 	void DrawRandomCard(BuildingEnum::TYPE type);   // Draws a random card of this type
	void DrawRandomCard();							// Draws a completely random card

	void  ToggleClickable(bool clickable);  // Sets whether the CardManager should check for click input on the cards
	float GetCardTemplateXPos();			// Give the x-pos of the hand background (For aligning with highscore)
	void  Free();							// At the end of the level free and unsubscribe from everything
}
