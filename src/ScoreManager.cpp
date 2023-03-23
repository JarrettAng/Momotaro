#include <ScoreManager.h>
#include <FontManager.h>
#include <RenderSystem.h>
#include <ColorTable.h>
#include <GridManager.h>

namespace ScoreManager {
	struct Level
	{
		int level = 0;
		std::string levelDesc = "";
		bool receivedReward = false;	// Has player received reward for this level?
	};

	const float EXP_MOD = 0.3f;					// Affects amount of exp. Lower values = more XP required per level
	const float EXP_INCREASE_MOD = 1.8f;		// Affects how quicky required exp per level. Higher values = larger gaps between levels

	EventSystem::Event<void> onScoreUpdate;
	EventSystem::Event<void> onLevelChange;

	Level currLevel{};			// Current player's level.
	int score{};				// Current player's score.

	std::vector<Level> levels;	// Contains all level data.

	std::string synergy;
	int potentialScoreGain;

	/*!***********************************************************************
	* FORWARD DECLARATIONS
	*************************************************************************/
	Level GetLevel(int level);	// Get level data from vector.
	Level GetCurrLevel();		// Get player's current level.
	void IntializeLevels();
	void CheckForLevelChange();
	int GetThreshold(int level);


	int GetScore() {
		return score;
	}

	void AddScore(int modifier) {
		score += modifier;
		onScoreUpdate.Invoke();
	}

	void Initialize() {
		IntializeLevels();

		score = potentialScoreGain = 0;
		currLevel.level = 0;
		GridManager::onTotalSynergyUpdate.Subscribe(UpdatePotentialScoreGain);
	}

	void IntializeLevels() {
		// TODO: READ FROM FILE I/O
		int levelNum = 127;
		for (int i = 0; i < levelNum; ++i) {
			levels.push_back(Level{ i, "Temp description." });
		}

		// Every time score is incremented or decremented, check for change in level.
		onScoreUpdate.Subscribe(CheckForLevelChange);
	}

	void Free() {
		levels.clear();
	}

	void CheckForLevelChange() {
		// Level up.
		if (score >= GetThreshold(currLevel.level + 1)) {
			currLevel = GetLevel(currLevel.level + 1);
			onLevelChange.Invoke();
			return;
		}
	}

	void Draw() {
		// Tell ui manager to draw score ui.
		// Use currLevel and score to draw.
		synergy = "SYNERGY " + std::to_string(score) + (potentialScoreGain > 0 ? " +" + std::to_string(potentialScoreGain) : 
			(!potentialScoreGain ? "" : " " + std::to_string(potentialScoreGain))) + " / " + std::to_string(GetThreshold(currLevel.level + 1));
		RenderSystem::AddTextToBatch(RenderSystem::UI_BATCH, -0.9f, 0.8f, FontManager::GetFont(FontManager::SHIN_GO), 60, synergy,0,COLOR_BLACK);
	}

	void UpdatePotentialScoreGain(int newPotentialScore) {
		potentialScoreGain = newPotentialScore;
	}

	Level GetCurrLevel() {
		return currLevel;
	}

	Level GetLevel(int level) {
		for (Level& l : levels) {
			if (level == l.level) {
				return l;
			}
		}
		std::cout << "GETTING OUT OF BOUND LEVEL, RETURNED EMPTY LEVEL DATA.";
		return Level{};
	}

	// Calculate score needed based on current level and score.
	int GetThreshold(int level) {
		return static_cast<int>(pow(level / EXP_MOD, EXP_INCREASE_MOD));
	}
}
