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

#include <UIManager.h>
#include <CardManager.h>
#include <Building.h>

#include <ColorTable.h>

class Card {
public:
	RenderSystem::Transform position;
	RenderSystem::Transform iconPos;

	int count;
	UI::TextBox countText;
	RenderSystem::Transform countIconPos;

	UI::TextBox nameText;
	RenderSystem::Transform descTextPos;

	BuildingData bData;

	Vec4<float> color;
	Vec4<float> borderColor;

	//bool operator == (const Card card) const { return deckCardData == card.deckCardData; }
	//bool operator != (const Card card) const { return !operator==(card); }

	Card(RenderSystem::Transform _position, BuildingData _bData) {
		position = _position;

		bData = _bData;

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

		// Set count to 1 (Cards constructed are the first of its kind)
		count = 1;

		UpdateComponentPositions();
	}

	void UpdateComponentPositions() {
		// Load name data based on position
		Vec2<float> nameTextPos{position.pos.x + position.size.x * 0.1f, position.pos.y - position.size.y * 0.75f};
		nameText = UI::TextBox(nameTextPos, bData.name, UI::CENTER_JUSTIFY, position.size.x * 0.8f, 15.0f, COLOR_BLACK);

		// Load desc data based on position
		descTextPos.pos.x = position.pos.x;
		descTextPos.pos.y = nameText.GetPos().pos.y + position.size.y * 0.1f;

		// Load icon data based on position
		iconPos.size.x = position.size.x * 1.5f;
		iconPos.size.y = iconPos.size.x;
		iconPos.pos.x = position.pos.x - (iconPos.size.x - position.size.x) / 2.0f;
		iconPos.pos.y = position.pos.y + iconPos.size.x * 0.25f;

		// Load count data based on position
		countIconPos.size.y = countIconPos.size.x = position.size.x * 0.35f;
		countIconPos.pos.x = position.pos.x + position.size.x * 0.8f;
		countIconPos.pos.y = position.pos.y + position.size.y * 0.075f;

		UpdateCountText();
	}

	void UpdateCountText() {
		Vec2<float> countTextPos{ countIconPos.pos.x - countIconPos.size.x * 0.1f, countIconPos.pos.y - countIconPos.size.y * 0.65f };
		countText = UI::TextBox(countTextPos, std::to_string(count), UI::CENTER_JUSTIFY, countIconPos.size.x * 1.5f, 18.0f, COLOR_BLACK);
	}
};
