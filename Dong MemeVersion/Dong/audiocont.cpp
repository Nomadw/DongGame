#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 
#include "audiocont.h"
#include "wave.h"



Audiocontroller::Audiocontroller(IXAudio2 *engine, IXAudio2MasteringVoice *g_master)
{
	musicplaying = false;

	//create the engine
	if( FAILED( XAudio2Create( &engine ) ) )
	{
		CoUninitialize();
		//return -1;
	}

	//create the mastering voice
	if( FAILED( engine->CreateMasteringVoice( &g_master ) ) )
	{
		engine->Release();
		CoUninitialize();
		//return -2;
	}

	
}

void Audiocontroller::LoadSound(IXAudio2 *engine, IXAudio2SourceVoice *music, IXAudio2SourceVoice *sfx,
					 Wave musicbuffer, Wave soundbuffer,
					Wave sound2buffer)
{
	//load a music file
	if( !musicbuffer.load( "musicbluebolt.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		//return -3;
	}

	//load a sound file
	if( !soundbuffer.load( "gun.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		//return -3;
	}

	//load a sound file
	if( !sound2buffer.load( "thuglife.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		//return -3;
	}


	//create the source voice, based on loaded wave format
	//if( FAILED( 
		engine->CreateSourceVoice( &music, musicbuffer.wf() ) ;
	//	) )
	//{
	//	engine->Release();
	//	CoUninitialize();
		//return -4;
	//}

		//create the source voice, based on loaded wave format
	//if( FAILED( 
		engine->CreateSourceVoice( &sfx, soundbuffer.wf() ) ;
	//	) )
	//{
		//engine->Release();
		//CoUninitialize();
		//return -4;
	//}

	//start consuming audio in the source voice
	music->Start();
	sfx->Start();
}

void Audiocontroller::PlaySound(IXAudio2SourceVoice *source, Wave *sound)
{ 
	//if(source == music) {
	if(!CheckPlaying()) { //if music is already playing, don't play
	musicplaying = true;
	source->SubmitSourceBuffer(sound->xaBuffer() ); // play music
	} 
	//	} else
	//source->SubmitSourceBuffer(sound->xaBuffer() ); // play sound effect
}

void Audiocontroller::StopSound(IXAudio2SourceVoice *source)
{
	//if(source == music)
	//	musicplaying = false;
		source->Stop();
		source->FlushSourceBuffers();
}