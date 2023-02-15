#pragma once
#include <EventSystem.h>
#include <AEEngine.h>
#include <string>
#include <vector>
#include <iostream>

namespace ScoreManger {
	EventSystem::Event<void> onScoreUpdate;
	EventSystem::Event<void> onLevelChange;
	void Intialize();
	void AddScore(int modifier);
}
