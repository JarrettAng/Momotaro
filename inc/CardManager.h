#pragma once

#include <BuildingManager.h>

namespace CardManager {

	struct DeckData {
		int count;
		BuildingData card;
	};

	void Initialize();

	void PrepareUIRenderBatch();

	void DrawCard(BuildingEnum::TYPE type, BuildingEnum::LEVEL level);

	void DrawRandomCard(BuildingEnum::LEVEL level);
	void DrawRandomCard(BuildingEnum::TYPE type);
}
