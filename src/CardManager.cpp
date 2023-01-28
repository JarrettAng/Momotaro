
#include <vector>

#include <BuildingManager.h>
#include <RenderSystem.h>

#include <Card.h>

namespace CardManager {
	int handSize;
	std::vector<Card> hand;

	int selectedIndex;

	void Initialize() {
		handSize = 5;

		// Fill hand with 5 random level 1 cards

	}

	void Free() {
		hand.clear();
	}

	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level) {

	}

	void DrawRandomCard(BuildingEnum::LEVEL level) {
		RenderSystem::SpriteInfo spriteInfo {};
		BuildingData buildingData = BuildingManager::GetRandomBuildingData(level);
		Card newCard = Card(spriteInfo, buildingData);
	}

	void DrawRandomCard(BuildingEnum::TYPE type) {

	}
}
