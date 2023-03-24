#include "AudioManager.h"
#include <iostream>

namespace AudioManager {

	//This is the main array we are manipulating
	//Using an static array because I feel that I don't need it to be dynamic.
	//Since we won't be loading any new sounds mid game, all these are "pre-determined".
	std::array<AudioClip,ClipName::MAX_COUNT> AudioClipArray;
	int _index{0};

	AEAudioGroup BGM_GROUP;
	AEAudioGroup SFX_GROUP;
	void Load() {
		//TODO FILE SERIALISATION FOR AUDIO CLIP NAMES!
		//* 1) get list of audio names from file I/O
		//* 2) load it all using loop
		BGM_GROUP = AEAudioCreateGroup();
		SFX_GROUP = AEAudioCreateGroup();

		AddAudioClipToArray(
			AudioClip{
				AEAudioLoadMusic("Assets/Audio/GameMenu2.wav"),
				1.0f,
				BGM_MAIN
			}
		);
		// std::cout << "Clip Status " << AEAudioIsValidAudio(AudioClipArray[0]._audioClip) << '\n';

		// std::cout << "Group Status : " << AEAudioIsValidGroup(BGM_GROUP) << '\n';
	}
	void Initialize() {
		

	}
    void Update(){
		AEAudioUpdate();
    }
    void Unload(){

    }

    void AddAudioClipToArray(AudioClip _clip)
    {
		AudioClipArray[_index] = _clip;
		++_index;
    }
	void PlayBGM(ClipName _clip){
		// AEAudio BGM{GetAudioFromClipName(_clip)};
		AEAudioPlay(GetAudioFromClipName(_clip),BGM_GROUP,.75f,1.f, -1);
	}
	AEAudio GetAudioFromClipName(ClipName _clipName){
		for(AudioClip _clip : AudioClipArray){
			if(_clip._name == _clipName){
				return _clip._audioClip;
			}
		}
		std::cerr << "ERROR : UNABLE TO FIND AUDIO\n";
	}
}