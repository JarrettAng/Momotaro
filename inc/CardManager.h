#pragma once

#include <BuildingManager.h>

namespace CardManager {

	struct DeckData {
		u16 count;
		BuildingData card;
	};

	void Initialize();

	void PrepareUIRenderBatch();

	void UpdateCardPositions();

	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level);

	void DrawRandomCard(BuildingEnum::LEVEL level);
	void DrawRandomCard(BuildingEnum::TYPE type);
}
