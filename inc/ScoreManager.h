#pragma once
#include <EventSystem.h>
#include <AEEngine.h>
#include <string>
#include <vector>
#include <iostream>

namespace ScoreManager {
	extern EventSystem::Event<void> onScoreUpdate;
	extern EventSystem::Event<void> onLevelChange;
	void UpdatePotentialScoreGain(int);
	void Initialize(float _exp_mod = 0.3f, float _exp_increase_mod = 1.8f);
	void Free();
	void Draw();
	void AddScore(int modifier);
}
