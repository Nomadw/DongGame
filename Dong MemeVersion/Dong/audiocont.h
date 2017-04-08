#pragma once

// Include headers accessible 
#include "Globals.h"
#include "wave.h"
#include <XAudio2.h>
#include <XAudio2fx.h>



class Audiocontroller
{
public:
	bool musicplaying;

	Audiocontroller(IXAudio2 *engine, IXAudio2MasteringVoice *g_master);
	void LoadSound(IXAudio2 *engine, IXAudio2SourceVoice *music, IXAudio2SourceVoice *sfx,
					 Wave musicbuffer, Wave soundbuffer,
					Wave sound2buffer);
	void PlaySound(IXAudio2SourceVoice *source, Wave *sound);
	void StopSound(IXAudio2SourceVoice *source);
	bool CheckPlaying() {return musicplaying;}
};