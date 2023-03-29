#include <ScoreManager.h>
#include <FontManager.h>
#include <RenderSystem.h>
#include <ColorTable.h>
#include <GridManager.h>
#include <AudioManager.h>

namespace ScoreManager {
	struct Level
	{
		int level = 0;
		std::string levelDesc = "";
		bool receivedReward = false;	// Has player received reward for this level?
	};

	float exp_mod = 0.3f;				// Affects amount of exp. Lower values = more XP required per level
	float exp_increase_mod = 1.8f;		// Affects how quicky required exp per level. Higher values = larger gaps between levels

	EventSystem::Event<void> onScoreUpdate;
	EventSystem::Event<void> onLevelChange;

	Level currLevel{};			// Current player's level.
	int score{};				// Current player's score.

	std::vector<Level> levels;	// Contains all level data.

	std::string synergy;
	std::string highscore;
	int potentialScoreGain;

	int lastHighScore{};
	RenderSystem::Transform background;

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

	void Initialize(float _exp_mod, float _exp_increase_mod) {
		IntializeLevels();
		//Load highscore from file here 
		score = potentialScoreGain = 0;
		currLevel.level = 0;
		GridManager::onTotalSynergyUpdate.Subscribe(UpdatePotentialScoreGain);
		// background.pos.x = -AEGfxGetWinMaxX()/2;
		// background.pos.y = -AEGfxGetWinMaxY()*0.95;
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
			AudioManager::PlayAudioClip(AudioManager::ClipName::SFX_THRESHOLD);
			currLevel = GetLevel(currLevel.level + 1);
			onLevelChange.Invoke();
			return;
		}
	}

	void Draw() {
		// Tell ui manager to draw score ui.
		// Use currLevel and score to draw.
		//////////////////////////////////////////////////////////////////////////////////////
		// DRAW SYNERGY POINTS TEXT
		//////////////////////////////////////////////////////////////////////////////////////
		
		synergy = "SYNERGY " + std::to_string(score) + (potentialScoreGain > 0 ? " +" + std::to_string(potentialScoreGain) : 
			(!potentialScoreGain ? "" : " " + std::to_string(potentialScoreGain))) + " / " + std::to_string(GetThreshold(currLevel.level + 1));
		RenderSystem::AddTextToBatch(RenderSystem::UI_BATCH, -0.95f, 0.85f, FontManager::GetFont(FontManager::SHIN_GO), 50, synergy,0,COLOR_BLACK);

		//////////////////////////////////////////////////////////////////////////////////////
		// HIGH SCORE TEXT
		//////////////////////////////////////////////////////////////////////////////////////
		highscore = "HIGHSCORE : " + std::to_string(lastHighScore);
		RenderSystem::AddTextToBatch(RenderSystem::UI_BATCH, -0.95f, -0.65f, FontManager::GetFont(FontManager::SHIN_GO),40,highscore,1,COLOR_BLACK);
		background.size.x = (float)(highscore.size()*40.f);
		background.size.y = 50.f;
		RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, -AEGfxGetWinMaxX(), -AEGfxGetWinMaxY()*0.56f, background.size.x, background.size.y, TextureManager::BLANK_PROMPT);
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
		return static_cast<int>(pow(level / exp_mod, exp_increase_mod));
	}
}
