#include "AudioManager.h"
#include "FileIOManager.h"
#include <iostream>

namespace AudioManager {

	//This is the main array we are manipulating
	//Using an static array because I feel that I don't need it to be dynamic.
	//Since we won't be loading any new sounds mid game, all these are "pre-determined".
	std::array<AudioClip,ClipName::MAX_COUNT> AudioClipArray;
	int _index{0};
	ClipName CURRENTBGM{ClipName::NONE};
	AEAudioGroup BGM_GROUP;
	AEAudioGroup SFX_GROUP;
	void Load() {
		BGM_GROUP = AEAudioCreateGroup();
		SFX_GROUP = AEAudioCreateGroup();
		FileIOManager::LoadAudioClipsFromFile(AudioClipArray);
	}
	void Initialize() {
		

	}
    void Update(){
		AEAudioUpdate();
    }
    void Unload(){

    }
    
	void PlayBGM(ClipName _clip){
		AEAudioStopGroup(BGM_GROUP);
		if(CURRENTBGM != _clip){
			AEAudioPlay(GetAudioFromClipName(_clip),BGM_GROUP,AudioClipArray[_clip]._volume,1.f, -1);
			CURRENTBGM = _clip;
		}
	}
    void PlayAudioClip(ClipName _clip)
    {
		AEAudioPlay(GetAudioFromClipName(_clip),SFX_GROUP,AudioClipArray[_clip]._volume,1.f,0);
    }
    AEAudio GetAudioFromClipName(ClipName _clipName)
    {
        return AudioClipArray[_clipName]._audioClip;
    }
}