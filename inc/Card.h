#pragma once

#include <UIManager.h>
#include <Building.h>

class Card {
	public:
		Card(RenderSystem::SpriteInfo _renderData, BuildingData _buildingData) {
			renderData = _renderData;
			buildingData = _buildingData;
		}
		
	private:
		RenderSystem::SpriteInfo renderData;
		BuildingData buildingData;

};
