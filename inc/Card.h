#pragma once

#include <CardManager.h>
#include <UIManager.h>
#include <Building.h>

#include <ColorTable.h>

class Card {
	public:
		UIManager::Transform position;
		UIManager::Transform icon;
		UIManager::Transform count;
		UIManager::Transform name;
		UIManager::Transform desc;
		CardManager::DeckData *deckCardData;

		bool operator == (const Card& card) const { return deckCardData == card.deckCardData; }
		bool operator != (const Card& card) const { return !operator==(card); }

		Card(UIManager::Transform _position, CardManager::DeckData * _deckCardData) {
			position = _position;
			deckCardData = _deckCardData;

			UpdateComponentPositions();
		}

		void UpdateComponentPositions() {
			// Load name data based on position
			name.x = position.x;
			name.y = position.y;

			// Load desc data based on position
			desc.x = position.x;
			desc.y = name.y + position.height * 0.1f;

			// Load icon data based on position
			icon.width = position.width * 1.5f;
			icon.height = icon.width;
			icon.x = position.x - (icon.width - position.width) / 2.0f;
			icon.y = position.y + icon.width * 0.25f;

			// Load count data based on position
			count.x = position.x + position.width;
			count.y = position.y;
		}
};
