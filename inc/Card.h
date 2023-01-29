#pragma once

#include <UIManager.h>
#include <Building.h>

#include <ColorTable.h>

class Card {
	public:
		UIManager::Transform position;
		UIManager::Transform icon;
		UIManager::TextData count;
		UIManager::TextData name;
		UIManager::TextData desc;
		BuildingData buildingData;

		Card(UIManager::Transform _position, BuildingData _buildingData) {
			position = _position;
			buildingData = _buildingData;

			// Load name data based on position
			name.x = position.x;
			name.y = position.y + position.height * 0.5f;
			name.text = buildingData.name;
			name.color = COLOR_BLACK;

			// Load desc data based on position
			name.x = position.x;
			name.y = name.y + position.height * 0.1f;
			desc.text = buildingData.desc;
			desc.color = COLOR_BLACK;

			// Load icon data based on position
			icon.x = position.x;
			icon.y = position.y;

			// Load count data based on position
			count.x = position.x + position.width;
			count.y = position.y;
			count.text = "0";
			count.color = COLOR_BLACK;
		}
};
