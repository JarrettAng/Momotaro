/*!************************************************************************
\file:          Card.h
\author:		Jarrett Ang
\par DP email:	a.jiaweijarrett@digipen.edu
\par Course:    CSD1171B
\par Software Engineering Project
\date:          30-01-2023
\brief
This header file contains the information needed for the cards in the player's hand.
The card stores the positions for rendering all of its text and icon as well as the
information, like name, description needed to do so.

The functions include:
- Card
Constructor to create a card, needs information of the building it will represent
- UpdateComponentPositions
Updates the rendering positions of the card elements, usually when the hand size updates
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <UIManager.h>
#include <CardManager.h>
#include <Building.h>

#include <ColorTable.h>
///////////////////////////////////////////////////////////////////////////
// Used by the UI elements to maintain relative position (E.g. Header text should be at the top 10% of the card's height)
// The main purpose of this function is for more readability below
inline float GetPercentageOf(float length, float percent) { return length * percent; }

// This class contains all the data and logic needed to draw the cards on screen
// as well as its own count (how many copies of itself)
class Card {
public:
	///////////////////////////////////////////////////////////////////////
	// Non-default constructor
	Card(RenderSystem::Transform _position, BuildingData _bData) {
		position = _position;
		bData = _bData;

		// Depending on its type, the color of the card changes
		switch (bData.type) {
		case BuildingEnum::RESIDENTIAL:
			color = COLOR_CARD_R;
			break;
		case BuildingEnum::COMMERCIAL:
			color = COLOR_CARD_C;
			break;
		case BuildingEnum::INDUSTRIAL:
			color = COLOR_CARD_I;
			break;
		}

		borderColor = COLOR_CARD_BORDER;
		count = 1; // Set count to 1 (Cards constructed are the first of its kind)

		UpdateComponentPositions(); // Update the position of the child objects of this card
	}

	///////////////////////////////////////////////////////////////////////
	// == Operator overloading for building data and cards
	// Returns true if the level, size, and type matches
	bool operator==(BuildingData const& rhs) {
		if (this->bData.level == rhs.level &&
			this->bData.size == rhs.size &&
			this->bData.type == rhs.type) return true;
		return false;
	}

	bool operator==(Card const& rhs) {
		if (this->bData == rhs.bData) return true;
		return false;
	}

	///////////////////////////////////////////////////////////////////////
	// Member functions

	// Updates the positions of the child texts, icons, and such based on the parent's current position
	void UpdateComponentPositions() {
		// Load name data based on position
		// The name text is at the top 75% of the card with a 10% padding from the left
		Vec2<float> nameTextPos{position.pos.x + GetPercentageOf(position.size.x, 0.1f), position.pos.y - GetPercentageOf(position.size.y, 0.75f)};
		// Add a 20% padding for the text so it does not touch the edge
		nameText = UI::TextBox(nameTextPos, bData.name, UI::CENTER_JUSTIFY, GetPercentageOf(position.size.x, 0.8f), 15.0f, COLOR_BLACK);

		// Load desc data based on position
		descTextPos.pos.x = position.pos.x;
		// The desc text is 10% of the card's height lower than the name text
		descTextPos.pos.y = nameText.GetPos().pos.y + GetPercentageOf(position.size.y , 0.1f);

		// Load icon data based on position
		// Make the size of the icon (building) bigger to fill up the card
		iconPos.size.x = position.size.x * 1.5f;
		iconPos.size.y = iconPos.size.x;
		// Center the icon by moving it half of its size x
		iconPos.pos.x  = position.pos.x - (iconPos.size.x - position.size.x) / 2.0f;
		// Move the icon down by 25% of its size
		iconPos.pos.y  = position.pos.y + GetPercentageOf(iconPos.size.x, 0.25f);

		// Load count data based on position
		// Make the size of the count icon 35% the size of the card
		countIconPos.size.y = countIconPos.size.x = GetPercentageOf(position.size.x, 0.35f);
		// Move the icon to the top right (80% of the card's width)
		countIconPos.pos.x  = position.pos.x + GetPercentageOf(position.size.x, 0.8f);
		countIconPos.pos.y  = position.pos.y + GetPercentageOf(position.size.y, 0.075f);

		UpdateCountText();
	}

	// Updates the text number & position showing the number of copies of the current card
	void UpdateCountText() {
		// Make the text box size bigger than the background icon size (to prevent wrapping in case of large numbers like 9999)
		Vec2<float> countTextPos{ countIconPos.pos.x - GetPercentageOf(countIconPos.size.x, 0.25f), countIconPos.pos.y - GetPercentageOf(countIconPos.size.y, 0.65f) };
		countText = UI::TextBox(countTextPos, std::to_string(count), UI::CENTER_JUSTIFY, countIconPos.size.x * 2.0f, 18.0f, COLOR_BLACK);
	}

	///////////////////////////////////////////////////////////////////////
	// Data members
	RenderSystem::Transform position;		// The parent position, all elements position are local to this
	RenderSystem::Transform iconPos;

	int count;								// How many copies of the card currently herd
	UI::TextBox countText{};				// The text to display the count
	RenderSystem::Transform countIconPos;	// The background icon for the number

	UI::TextBox nameText{};
	RenderSystem::Transform descTextPos;

	BuildingData bData;						// Actual building data loaded onto the card

	Vec4<float> color;
	Vec4<float> borderColor;
};
