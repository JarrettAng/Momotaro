/*!************************************************************************
\file:          AudioManager.h
\author:		Amadeus Chia
\par DP email:	amadeusjinhan.chia@digipen.edu
\par Course:    CSD1451B
\par Software Engineering Project
\date:          24-03-2023
\brief
This header file handles everything audio related. 
To add audio clips, you have to add it's name to the ClipName enum,
then in the fileio, load it from its filepath.

All content © 2023 DigiPen Institute of Technology Singapore.
All rights reserved.
**************************************************************************/

#pragma once
///////////////////////////////////////////////////////////////////////////
#include <AEAudio.h>
#include <array>
///////////////////////////////////////////////////////////////////////////

namespace AudioManager {
	
	///////////////////////////////////////////////////////////////////////
	// The names of all the audio clips in the game. Also works as the index in the audio clip array
	enum ClipName{
		NONE=0,
		BGM_MAIN,
		BGM_GAME,
		BGM_CREDITS,
		BGM_LEVELSELECT,
		SFX_CLICK,
		SFX_MERGE1,
		SFX_MERGE2,
		SFX_GAMEOVER,
		SFX_GAINPOINT,
		SFX_LOSEPOINT,
		SFX_THRESHOLD,
		MAX_COUNT
	};

	///////////////////////////////////////////////////////////////////////
	// Sttruct that stores the audio clip data. 
	struct AudioClip{
		AEAudio _audioClip;
		float _volume;
		AEAudioGroup _group;
	};
	///////////////////////////////////////////////////////////////////////
	//GLOBAL VALUES
	extern AEAudioGroup BGM_GROUP;			//group for all BGM
	extern AEAudioGroup SFX_GROUP;			//group for all SFX
	//extern float masterVolume;


	///////////////////////////////////////////////////////////////////////
	//MANAGER FUNCTIONS
	void Initialize();
	void Load();
	void Update();
	void Unload();

	///////////////////////////////////////////////////////////////////////
	//HELPER FUNCTIONS
	void PlayBGM(ClipName _name);			//Plays the selected bgm 
	void PlayAudioClip(ClipName _clip);		//Plays the selected audio clip 
	AEAudio GetAudioFromClipName(ClipName _clip);
	//Possible functions
	// void StopAllSounds(void);				
	//ToggleBGM
	//ToggleMute
	//ToggleMuteAll
	//SetMasterVolume(float _volume);
}