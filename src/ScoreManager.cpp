/*!************************************************************************
\file			ScoreManager.cpp
\author			Tan Jun Rong
\par DP email:	t.junrong@digipen.edu
\par Course:	CSD1171B
\par Software Engineering Project
\date			31-03-2023
\brief
This source file kep track of the current synergy (score) of the game.

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

///////////////////////////////////////////////////////////////////////////
#include <ScoreManager.h>
#include <FontManager.h>
#include <RenderSystem.h>
#include <ColorTable.h>
#include <GridManager.h>
#include <AudioManager.h>
#include <DebugManager.h>
#include <UIManager.h>
#include <CardManager.h>
#include <FileIOManager.h>
///////////////////////////////////////////////////////////////////////////

namespace ScoreManager {
	/*!***********************************************************************
	\brief
		Stores the synergy level.
	*************************************************************************/
	struct Level
	{
		int level = 0;
		std::string levelDesc = "";
		bool receivedReward = false;	// Has player received reward for this level?
	};

	/*!***********************************************************************
	* Const Variable.
	*************************************************************************/
	const float EXP_MOD = 0.3f;				// Affects amount of exp. Lower values = more XP required per level
	const float EXP_INCREASE_MOD = 1.8f;	// Affects how quicky required exp per level. Higher values = larger gaps between levels

	/*!***********************************************************************
	* Events.
	*************************************************************************/
	EventSystem::Event<void> onScoreUpdate;
	EventSystem::Event<void> onLevelChange;

	/*!***********************************************************************
	* Variables.
	*************************************************************************/
	Level currLevel{};			// Current player's level.
	int score{};				// Current player's score.

	std::vector<Level> levels;	// Contains all level data.

	std::string synergy;
	std::string highscore;
	UI::TextBox HighScoreTextBox;


	RenderSystem::Transform background;
	
	int potentialScoreGain;
	int lastHighScore{};
	const float highScoreFontSize{24.f};
	bool showHighScore;

	/*!***********************************************************************
	* Forward Declarations.
	*************************************************************************/
	Level GetLevel(int level);	// Get level data from vector.
	Level GetCurrLevel();		// Get player's current level.
	void IntializeLevels();
	void CheckForLevelChange();
	int GetThreshold(int level);

	/*!***********************************************************************
	\brief
		Get player's highscore.	
	\return
		Highscore.
	*************************************************************************/
	int GetHighScore() {
		return max(score,lastHighScore);
	}

	/*!***********************************************************************
	\brief
		Get player's current level.	
	\return
		Level.
	*************************************************************************/
	int GetLevel() {
		return currLevel.level;
	}
	int GetCurrScore(){
		return score;
	}
	/*!***********************************************************************
	\brief
		Set player's highscore.
	\param _score
		Score.
	*************************************************************************/
	void SetHighScore(int _score){
		lastHighScore = max(_score,lastHighScore);
	}
	/*!***********************************************************************
	\brief
		Set player's level.
	\param _score
		Level.
	*************************************************************************/
	void SetLevel(int _level){
		currLevel.level = _level;
	}

	void SetScore(int _currScore){
		score = _currScore;
	}

	/*!***********************************************************************
	\brief
		Add score.
	\param modifier
		Amount of score to add.
	*************************************************************************/
	void AddScore(int modifier) {
		score += modifier;
		if(score<0) score = 0;
		onScoreUpdate.Invoke();
	}

	/*!***********************************************************************
	\brief
		Initialize ScoreManager.
	*************************************************************************/
	void Initialize() {
		IntializeLevels();
		//Load highscore from file here 
		FileIOManager::LoadHighScoreFromFile();
		score = potentialScoreGain = 0;
		currLevel.level = 0;
		GridManager::onTotalSynergyUpdate.Subscribe(UpdatePotentialScoreGain);
	}

	/*!***********************************************************************
	\brief
		Initialize the levels of the player.
	*************************************************************************/
	void IntializeLevels() {
		// TODO: READ FROM FILE I/O
		int levelNum = 127;
		for (int i = 0; i < levelNum; ++i) {
			levels.push_back(Level{ i, "Temp description." });
		}

		// Every time score is incremented or decremented, check for change in level.
		onScoreUpdate.Subscribe(CheckForLevelChange);
	}

	/*!***********************************************************************
	\brief
		Free ScoreManager.
	*************************************************************************/
	void Free() {
		levels.clear();
	}

	/*!***********************************************************************
	\brief
		Toggle ShowHighScore state.
	\param state
		True / false.
	*************************************************************************/
	void ToggleShowHighScore(bool state){
		showHighScore = state;
	}

	/*!***********************************************************************
	\brief
		Check if level had changed.
	*************************************************************************/
	void CheckForLevelChange() {
		// Level up.
		if (score >= GetThreshold(currLevel.level + 1)) {
			// Get next level.
			currLevel = GetLevel(currLevel.level + 1);
			onLevelChange.Invoke();
			// Level up sound.
			AudioManager::PlayAudioClip(AudioManager::ClipName::SFX_THRESHOLD);
			return;
		}
	}

	/*!***********************************************************************
	\brief
		Draw ScoreManager.
	*************************************************************************/
	void Draw() {
		// Draw synergy points text.
		synergy = "SYNERGY " + std::to_string(score) + (potentialScoreGain > 0 ? " +" + std::to_string(potentialScoreGain) : 
			(!potentialScoreGain ? "" : " " + std::to_string(potentialScoreGain))) + " / " + std::to_string(GetThreshold(currLevel.level + 1));
		RenderSystem::AddTextToBatch(RenderSystem::UI_BATCH, -0.95f, 0.85f, FontManager::GetFont(FontManager::SHIN_GO), 50, synergy,0,COLOR_BLACK);

		// Draw highscore text.
		if(showHighScore){
			highscore = "HIGHSCORE : " + std::to_string(GetHighScore());
			// RenderSystem::AddTextToBatch(RenderSystem::UI_BATCH, -0.5f, 0.85f, FontManager::GetFont(FontManager::SHIN_GO),40,highscore,1,COLOR_BLACK);
			HighScoreTextBox = UI::TextBox{
				Vec2<float>{
					CardManager::GetCardTemplateXPos()+highScoreFontSize,AEGfxGetWinMaxY()*-0.54f	
					//AEGfxGetWinMaxX()*-0.55f,AEGfxGetWinMaxY()*-0.525f	
				},
				highscore,
				UI::LEFT_JUSTIFY,
				640,
				highScoreFontSize,
				COLOR_BLACK
			};
			HighScoreTextBox.Render();
			// Get highscore.
			
			// Draw background.
			background.size.x = (float)(highscore.size()*highScoreFontSize);
			background.size.y = highScoreFontSize + 5.f;
			RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, CardManager::GetCardTemplateXPos(), -AEGfxGetWinMaxY()*0.485f, background.size.x, background.size.y, TextureManager::BLANK_PROMPT);
			// RenderSystem::AddRectToBatch(RenderSystem::UI_BATCH, -AEGfxGetWinMaxX()*0.59f, -AEGfxGetWinMaxY()*0.472f, background.size.x, background.size.y, TextureManager::BLANK_PROMPT);
		}
	}

	/*!***********************************************************************
	\brief
		Update potential score text.
	\param newPotentialScore
		Potential score.
	*************************************************************************/
	void UpdatePotentialScoreGain(int newPotentialScore) {
		potentialScoreGain = newPotentialScore;
	}

	/*!***********************************************************************
	\brief
		Get current level.
	\return
		Current level.
	*************************************************************************/
	Level GetCurrLevel() {
		return currLevel;
	}

	/*!***********************************************************************
	\brief
		Get level info of a particular level.
	\param level
		Level to get.
	\return
		Level.
	*************************************************************************/
	Level GetLevel(int level) {
		for (Level& l : levels) {
			if (level == l.level) {
				return l;
			}
		}
		Debug::Print("GETTING OUT OF BOUND LEVEL, RETURNED EMPTY LEVEL DATA.\n");
		return Level{};
	}

	/*!***********************************************************************
	\brief
		Calculate score needed based on current level and score.
	\param level
		Level to use.
	\return
		Threshold.
	*************************************************************************/
	int GetThreshold(int level) {
		return static_cast<int>(pow(level / EXP_MOD, EXP_INCREASE_MOD));
	}
}
