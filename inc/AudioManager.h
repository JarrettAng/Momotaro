/*!************************************************************************
\file:          AudioManager.h
\author:		Amadeus Chia
\par DP email:	amadeusjinhan.chia@digipen.edu
\par Course:    CSD1451B
\par Software Engineering Project
\date:          24-03-2023
\brief
This header file handles everything audio related

The functions include:
- Initialize
Reads the building data from the JSON file upon loading into the game scene
- Clear
Resets the data read on game scene exit
- GetBuildingData
Returns the building information of the given type, size, level
- GetRandomBuildingData
Returns a random building information, can be controlled by parameters
**************************************************************************/
#pragma once
#include <AEAudio.h>
#include <array>

namespace AudioManager {
	enum ClipName{
		NONE=0,
		BGM_MAIN,
		BGM_GAME,
		BGM_CREDITS,
		SFX_CLICK,
		SFX_MERGE1,
		SFX_MERGE2,
		SFX_GAMEOVER,
		SFX_GAINPOINT,
		SFX_LOSEPOINT,
		SFX_THRESHOLD,
		MAX_COUNT
	};

	struct AudioClip{
		AEAudio _audioClip;
		float _volume;
		AEAudioGroup _group;
	};

	//GLOBAL VALUES
	//extern float masterVolume;
	extern AEAudioGroup BGM_GROUP;
	extern AEAudioGroup SFX_GROUP;
	//MANAGER FUNCTIONS
	void Initialize();
	void Load();
	void Update();
	void Unload();

	//HELPER FUNCTIONS
	void PlayBGM(ClipName _name);
	void PlayAudioClip(ClipName _clip);
	void StopAllSounds(void);
	AEAudio GetAudioFromClipName(ClipName _clip);
	//Possible functions
	//ToggleBGM
	//ToggleMute
	//ToggleMuteAll
	//SetMasterVolume(float _volume);



}