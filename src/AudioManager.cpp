/*!************************************************************************
\file:          AudioManager.cpp
\author:		Amadeus Chia
\par DP email:	amadeusjinhan.chia@digipen.edu
\par Course:    CSD1451B
\par Software Engineering Project
\date:          24-03-2023
\brief
This source file implements the AudioManager functions defined in the header file.
Update needs to be called in order for the sounds to be played
PlayBGM will loop any clip that it plays
PlayAudioClip will simply play any clip without looping
**************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "AudioManager.h"
#include "FileIOManager.h"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace AudioManager {

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//This is the main array we are manipulating
	//Using an static array because I feel that I don't need it to be dynamic.
	//Since we won't be loading any new sounds mid game, all these are "pre-determined".
	std::array<AudioClip,ClipName::MAX_COUNT> AudioClipArray;
	int _index{0};
	ClipName CURRENTBGM{ClipName::NONE};
	AEAudioGroup BGM_GROUP;
	AEAudioGroup SFX_GROUP;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Manager functions
	/**
	 * @brief Creates audio groups and loads audio clips from file
	 * 
	 */
	void Load() {
		BGM_GROUP = AEAudioCreateGroup();
		SFX_GROUP = AEAudioCreateGroup();
		FileIOManager::LoadAudioClipsFromFile(AudioClipArray);
	}
	void Initialize() {} //Not used for now
	/**
	 * @brief Has to be called in order for music to play
	 * 
	 */
    void Update(){
		AEAudioUpdate();
    }
	///@brief  AE already helps unload the audio files so we don't have to.
    void Unload(){}
    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Helper Functions
	/**
	 * @brief Plays selected clip to BGM group (loops!)
	 * 
	 * @param _clip name of the clip to play (set in clipname enum)
	 */
	void PlayBGM(ClipName _clip) {
		//First we need to stop the current group, then play our new bgm
		AEAudioStopGroup(BGM_GROUP);
		AEAudioPlay(GetAudioFromClipName(_clip),BGM_GROUP,AudioClipArray[_clip]._volume,1.f, -1);
	}

	/**
	 * @brief Plays the selected clip to SFX group (does not loop)
	 * 
	 * @param _clip name of the clip to play (set in clipname enum)
	 */
    void PlayAudioClip(ClipName _clip) {
		//Since the sfx doesn't loop, we don't have to stop it
		AEAudioPlay(GetAudioFromClipName(_clip),SFX_GROUP,AudioClipArray[_clip]._volume,1.f,0);
    }

	/**
	 * @brief Get the Audio From Clip Name object
	 * 
	 * @param _clipName name of the clip to search (set in clipname enum)
	 * @return AEAudio 
	 */
    AEAudio GetAudioFromClipName(ClipName _clipName) {
        return AudioClipArray[_clipName]._audioClip;
    }
}