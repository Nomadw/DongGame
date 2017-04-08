// include the basic windows header files and the Direct3D header files
#include "stdafx.h"

#include "Globals.h"

#include <list>
//--Include other project files (LOCAL)
#include "MainObject.h"
#include "ball.h"
#include "paddle.h"
#include "wave.h"
#include "titlescreen.h"
#include "scoreitem.h"
#include "background.h"
#include "border.h"
//#include "audiocont.h"

//--Create iterator lists, used for handling updating and rendering
std::list<MainObject *> objects;
std::list<MainObject *>::iterator it01;
std::list<MainObject *>::iterator it02;

std::list<ScoreItem *> scoreobjects;
std::list<ScoreItem *>::iterator sc01;

// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    // the pointer to our back buffer
ID3D11DepthStencilView *zbuffer;       // the pointer to our depth buffer
ID3D11InputLayout *pLayout;            // the pointer to the input layout
ID3D11VertexShader *pVS;               // the pointer to the vertex shader
ID3D11PixelShader *pPS;                // the pointer to the pixel shader
ID3D11Buffer *pVBuffer;                // the pointer to the vertex buffer
ID3D11Buffer *pIBuffer;                // the pointer to the index buffer
ID3D11Buffer *pCBuffer;                // the pointer to the constant buffer

vector <ID3D11ShaderResourceView *> textures; //Vector array of texture buffers

//set up audio features
IXAudio2 *engine;
IXAudio2SourceVoice	*music;
IXAudio2SourceVoice *sfx;
IXAudio2SourceVoice *sfx2;
IXAudio2SourceVoice *sfx3;
IXAudio2SourceVoice *sfx4;
IXAudio2SourceVoice *msfx;
IXAudio2SourceVoice *msfx2;
IXAudio2SourceVoice *msfx3;
IXAudio2SourceVoice *msfx4;
IXAudio2SourceVoice *msfx5;
IXAudio2SourceVoice *msfx6;
IXAudio2MasteringVoice *g_master;

Wave musicbuffer;
Wave music2buffer;
Wave music3buffer;
Wave soundbuffer;
Wave sound2buffer;
Wave sound3buffer;
Wave sound4buffer;
Wave msoundbuffer;
Wave msound2buffer;
Wave msound3buffer;
Wave msound4buffer;
Wave msound5buffer;
Wave msound6buffer;

bool musicplaying = true;
bool okeypress = false;
bool pkeypress = false;
bool enterpress = false;
bool sound = true;
int gamestate = TITLE;

bool MLG = false;

bool ai = true;
int lastscored;

float CameraY = 0.0f;
float CameraX = 0.0f;
float CameraZ = 7.5f;
float CameraLookX = 0.0f;
float CameraLookY = 0.0f;
float CameraLookZ = 0.0f;

float red = 0.2f;
float green = 0.2f;
float blue = 0.2f;

//Create devices for direct input
IDirectInputDevice8 *DIKeyboard;
IDirectInputDevice8 *DIMouse;

HWND hWnd;
HRESULT hr;

DIMOUSESTATE mouseLastState;
LPDIRECTINPUT8 DirectInput;

// function prototypes
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void RenderFrame(void);     // renders a single frame
void CleanD3D(void);        // closes Direct3D and releases memory
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders
void UpdateProgram(void);
void Scored(Paddle *player);
void GameWon();
void RunTitleScreen(TitleScreen *title);

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    RECT wr = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL,
                          L"WindowClass",
                          L"Dong - DirectX Pong - Damon Wareham 2015",
                          WS_OVERLAPPEDWINDOW,
                          0,
                          0,
                          wr.right - wr.left,
                          wr.bottom - wr.top,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    InitD3D(hWnd);

	//must call this for COM
	CoInitializeEx( NULL, COINIT_MULTITHREADED );

	
	//create the engine
	if( FAILED( XAudio2Create( &engine ) ) )
	{
		CoUninitialize();
		return -1;
	}

	//create the mastering voice
	if( FAILED( engine->CreateMasteringVoice( &g_master ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -2;
	}

	//load a music file
	if( !musicbuffer.load( "audio/music/darkesthour.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a music file
	if( !music2buffer.load( "audio/music/fourhorsemen.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a music file
	if( !music3buffer.load( "audio/MLG/MLGMUSIC.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}

	//load a sound file
	if( !soundbuffer.load( "audio/sound/change.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a sound file
	if( !sound2buffer.load( "audio/sound/metalhit.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a sound file
	if( !sound3buffer.load( "audio/sound/laser.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a sound file
	if( !sound4buffer.load( "audio/sound/xplode.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}

		//load a sound file
	if( !msoundbuffer.load( "audio/MLG/hitmark.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a sound file
	if( !msound2buffer.load( "audio/MLG/intervention.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a sound file
	if( !msound3buffer.load( "audio/MLG/airhorn.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a sound file
	if( !msound4buffer.load( "audio/MLG/noscoped.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a sound file
	if( !msound5buffer.load( "audio/MLG/ooh.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}
	//load a sound file
	if( !msound6buffer.load( "audio/MLG/nicememe.wav" ) )
	{
		engine->Release();
		CoUninitialize();
		return -3;
	}

	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &music, musicbuffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}

	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &music, music2buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}

	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &music, music3buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}

	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &msfx, msoundbuffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}
	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &msfx2, msound2buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}
	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &msfx3, msound3buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}
	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &msfx4, msound4buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}
	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &msfx5, msound5buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}
	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &msfx6, msound6buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}

		//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &sfx, soundbuffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}
	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &sfx2, sound2buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}
	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &sfx3, sound3buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}
	//create the source voice, based on loaded wave format
	if( FAILED( engine->CreateSourceVoice( &sfx4, sound4buffer.wf() ) ) )
	{
		engine->Release();
		CoUninitialize();
		return -4;
	}

	//start consuming audio in the source voice
	music->Start();
	sfx->Start();
	sfx2->Start();
	sfx3->Start();
	sfx4->Start();
	msfx->Start();
	msfx2->Start();
	msfx3->Start();
	msfx4->Start();
	msfx5->Start();
	msfx6->Start();

    //--Initialising Direct Input
	hr = DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DirectInput,
		NULL); 

	hr = DirectInput->CreateDevice(GUID_SysKeyboard,
		&DIKeyboard,
		NULL);

	DirectInput->CreateDevice(GUID_SysMouse,
		&DIMouse,
		NULL);

	hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = DIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = DIMouse->SetDataFormat(&c_dfDIMouse);
	hr = DIMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	//Audiocontroller *audiocont = new Audiocontroller(engine, g_master);
	//audiocont->LoadSound(engine, music, sfx, musicbuffer, soundbuffer, sound2buffer);

	Background *bg = new Background();
	objects.push_back(bg);
	Border *border1 = new Border(0.0f, 3.25f);
	objects.push_back(border1);
	Border *border2 = new Border(0.0f, -3.25f);
	objects.push_back(border2);
	TitleScreen *title = new TitleScreen();
	objects.push_back(title);
	Paddle *paddle1 = new Paddle(PLAYER);
	objects.push_back(paddle1);
	Paddle *paddle2 = new Paddle(PLAYER2);
	objects.push_back(paddle2);
	Ball *ball = new Ball();
	objects.push_back(ball);

	// enter the main loop:
	bool needtoQuit = false;
    MSG msg;

    while(needtoQuit == false)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
           // if the message was a "quit" message...
			if( msg.message == WM_QUIT )
			{
				needtoQuit = true;	// we want to quit asap
			}
			
			// if it was any other type of message (i.e. one we don't care about), process it as normal
            else 
            {
				TranslateMessage( &msg );
				DispatchMessage( &msg );
            }

        }
	//---Direct input
	DIMOUSESTATE mouseCurrState;

	BYTE keyboardState[256];

	DIKeyboard->Acquire();
	DIMouse->Acquire();

	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);

	DIKeyboard->GetDeviceState(sizeof(keyboardState),(LPVOID)&keyboardState);

	if(gamestate == PLAYING && !MLG) {
		if(musicplaying == false) {
			musicplaying = true;
			music->Stop();
			music->FlushSourceBuffers();
				music->Start();
			music->SubmitSourceBuffer(musicbuffer.xaBuffer() ); // play music
		}
	}

	if(sound)
	{
		music->SetVolume(1);
		sfx->SetVolume(0.3f);
		sfx2->SetVolume(0.2f);
		sfx3->SetVolume(1.0f);
		sfx4->SetVolume(1.0f);
		msfx->SetVolume(1.0f);
		msfx2->SetVolume(1.0f);
		msfx3->SetVolume(1.0f);
		msfx4->SetVolume(1.0f);
		msfx5->SetVolume(1.0f);
		msfx6->SetVolume(1.0f);
	} 
	else
	{
		music->SetVolume(0);
		sfx->SetVolume(0);
		sfx2->SetVolume(0);
		sfx3->SetVolume(0);
		sfx4->SetVolume(0);
		msfx->SetVolume(0);
		msfx2->SetVolume(0);
		msfx3->SetVolume(0);
		msfx4->SetVolume(0);
		msfx5->SetVolume(0);
		msfx6->SetVolume(0);
	}

	if(keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(hWnd, WM_DESTROY, 0, 0);

	if((keyboardState[DIK_M] & 0x80) && (keyboardState[DIK_L] & 0x80) && (keyboardState[DIK_G] & 0x80)) { 
		if(gamestate == PLAYING) {
			if(!MLG) {
			msfx6->SubmitSourceBuffer(msound6buffer.xaBuffer() );
			music->Stop();
			music->FlushSourceBuffers();
			music->Start();
			music->SubmitSourceBuffer(music3buffer.xaBuffer() ); // play music
			}
		MLG = true;
		}
	}

	if(keyboardState[DIK_P] & 0x80)
	{
		if(!pkeypress) {
			pkeypress = true;
				if(ai)
					ai = 0;
				else
					ai = 1;
			}
		
	} else {pkeypress = false;}
	if(keyboardState[DIK_O] & 0x80)
	{
		if(!okeypress) {
			okeypress = true;
			if(sound) {
				sound = false;
			} else {
				sound = true;
			}
		}
	} else {okeypress = false;}

	if(keyboardState[DIK_W] & 0x80)
	{
		if(paddle1->CheckShow())
		paddle1->MoveUp();
	}	
	else if(keyboardState[DIK_S] & 0x80)
	{
		if(paddle1->CheckShow())
		paddle1->MoveDown();
	} else {paddle1->ResetDir();}

	if(keyboardState[DIK_UP] & 0x80)
	{
		if(paddle2->CheckShow()) {
			if(!ai)
				paddle2->MoveUp();
		}
	}
	else if(keyboardState[DIK_DOWN] & 0x80)
	{
		if(paddle2->CheckShow()) {
			if(!ai)
				paddle2->MoveDown();
		}
	} else {paddle2->ResetDir();}

	if(keyboardState[DIK_SPACE] & 0x80)
	{
		if(gamestate == PLAYING) {
		if(paddle1->GetScore() >= 5 || paddle2->GetScore() >= 5) {
			GameWon();
		}
		if(!ball->CheckShow()) {
			if(lastscored == PLAYER) {
				ball->ResetPosition(PLAYER, (paddle1->GetX() - 0.5f), paddle1->GetY(), paddle1->GetYdir());
				if(!MLG)
				sfx3->SubmitSourceBuffer(sound3buffer.xaBuffer() );
				if(MLG)
				msfx3->SubmitSourceBuffer(msound3buffer.xaBuffer() );
			}
			else {	
				if(!ai) {
				ball->ResetPosition(PLAYER2, (paddle2->GetX() + 0.5f), paddle2->GetY(), paddle2->GetYdir());
				if(!MLG)
				sfx3->SubmitSourceBuffer(sound3buffer.xaBuffer() );
				if(MLG)
				msfx3->SubmitSourceBuffer(msound3buffer.xaBuffer() );
				}
				}
			}
		}
	}
	if(keyboardState[DIK_RETURN] & 0x80)
	{
		if(!enterpress) {
			enterpress = true;
		if(gamestate == TITLE) {
			gamestate = PLAYING;
			title->SetShow(false);
			paddle1->SetShow(true);
			paddle2->SetShow(true);
			bg->SetShow(true);
			border1->SetShow(true);
			border2->SetShow(true);
		}
		else {
			GameWon();
			}
		}
	} else {enterpress = false;}
	if(mouseCurrState.lY != mouseLastState.lY)
	{
		if(!ball->mousemove) {
			if(!MLG)
				sfx->SubmitSourceBuffer(soundbuffer.xaBuffer() );
				if(MLG)
				msfx->SubmitSourceBuffer(msoundbuffer.xaBuffer() );
		}
		float newnumber;
		newnumber = ball->GetY();
		newnumber -= (mouseCurrState.lY * 0.001f);
		ball->mousemove = true;
		ball->SetY(newnumber);
	}
	mouseLastState = mouseCurrState;


	//AI RUNNING CODE
	if(ai) {
		if(ball->CheckShow()) {
			if(ball->GetXdir() == 1) {
		if(paddle2->GetY() > ball->GetY())
			paddle2->MoveDown();
		if(paddle2->GetY() < ball->GetY())
			paddle2->MoveUp();
			}
		}
		if(!ball->CheckShow()) {
			if(lastscored == PLAYER2) {
				int serve = rand() % 40000;
				int servestyle = rand() % 3;

				serve = rand() % 40000;
				servestyle = rand() % 3;
				for (serve; serve != 0; serve--)
				{
					if(servestyle == 0) {
						if(paddle2->GetY() < 2.0f)
							paddle2->MoveUp();
						if(paddle2->GetY() > 2.0f)
							paddle2->MoveDown();
					}
					if(servestyle == 1) {
						if(paddle2->GetY() > -2.5f)
							paddle2->MoveDown();
						if(paddle2->GetY() < -2.5f)
							paddle2->MoveUp();
					}
					if(servestyle == 2) {
						if(paddle2->GetY() > 0.0)
							paddle2->MoveDown();
						if(paddle2->GetY() < 0.0)
							paddle2->MoveUp();
					}
				}
				ball->ResetPosition(PLAYER2, (paddle2->GetX() + 0.5f), paddle2->GetY(), paddle2->GetYdir());
				if(!MLG)
				sfx3->SubmitSourceBuffer(sound3buffer.xaBuffer() );
				if(MLG)
				msfx3->SubmitSourceBuffer(msound3buffer.xaBuffer() );
				if(paddle2->GetScore() >= 5)
					GameWon();
			}
		}
	}
	//END OF AI RUNNING CODE

	if(gamestate == PLAYING) {
		if(ball->GetX() >= 6) {
			if(!ball->GetPass()) {
				ball->SetPassed(true);
				Scored(paddle2);
				ball->SetYDir(0);
				lastscored = PLAYER2;
				if(MLG) {
				msfx4->SubmitSourceBuffer(msound4buffer.xaBuffer() );
				msfx5->SubmitSourceBuffer(msound5buffer.xaBuffer() );
				}
				if(!MLG)
					sfx4->SubmitSourceBuffer(sound4buffer.xaBuffer() );
			}
		}
		if(ball->GetX() <= -6) {
			if(!ball->GetPass()) {
				ball->SetPassed(true);
				Scored(paddle1);
				ball->SetYDir(0);
				lastscored = PLAYER;
				if(MLG) {
				msfx4->SubmitSourceBuffer(msound4buffer.xaBuffer() );
				msfx5->SubmitSourceBuffer(msound5buffer.xaBuffer() );
				}
				if(!MLG)
					sfx4->SubmitSourceBuffer(sound4buffer.xaBuffer() );
			}
		}
	}

	if(gamestate == PLAYING && ball->CheckShow()) {
		if(!MLG) {
	CameraX = ball->GetX()/10.0f;
	CameraY = ball->GetX()/10.0f;
	CameraLookX = ball->GetX()/10.0f;
	CameraLookY = ball->GetY()/10.0f;
		}
	if(MLG) {
		CameraX = ball->GetX()/5.0f;
		CameraY = ball->GetX()/5.0f;
		CameraLookX = ball->GetX()/5.0f;
		CameraLookY = ball->GetY()/5.0f;
		CameraZ -= 0.001f;
		if(CameraZ < 5.5f)
			CameraZ = 7.5f;
	}
	}
	else {
		CameraZ = 7.5f;
		CameraX = 0.0f;
		CameraY = 0.0f;
		CameraLookZ = 0.0f;
		CameraLookX = 0.0f;
		CameraLookY = 0.0f;
	}

	if(paddle1->GetScore() >= 5 )
		paddle2->Reset();
	if(paddle2->GetScore() >= 5)
		paddle1->Reset();

	if(MLG) {
		for(it01 = objects.begin(); it01 != objects.end(); ++it01)
			(*it01)->MLG = true;
		for(sc01 = scoreobjects.begin(); sc01 != scoreobjects.end(); ++sc01)
			(*sc01)->MLG = true;
		blue += 0.001f;
		red += 0.001f;
		green -= 0.001f;
		if(blue > 1.0f)
			blue = 0.0f;
		if(blue < 0.0f)
			blue = 1.0f;
		if(green > 1.0f)
			green = 0.0f;
		if(green < 0.0f)
			green = 1.0f;
		if(red > 1.0f)
			red = 0.0f;
		if(red < 0.0f)
			red = 1.0f;
		if(sound)
		music->SetVolume(0.5f);
	}
	if(!MLG) {
		for(it01 = objects.begin(); it01 != objects.end(); ++it01)
			(*it01)->MLG = false;
		for(sc01 = scoreobjects.begin(); sc01 != scoreobjects.end(); ++sc01)
			(*sc01)->MLG = false;
		blue = 0.2f;
		red = 0.2f;
		green = 0.2f;
		CameraZ = 7.5f;
	}
	

	if(gamestate == TITLE)
		RunTitleScreen(title);

		UpdateProgram();
        RenderFrame();

    }

    // clean up DirectX and COM
    CleanD3D();

    return msg.wParam;
}


// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            } break;
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
    scd.OutputWindow = hWnd;                               // the window to be used
    scd.SampleDesc.Count = 4;                              // how many multisamples
    scd.Windowed = TRUE;                                   // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE,
                                  NULL,
                                  NULL,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &swapchain,
                                  &dev,
                                  NULL,
                                  &devcon);


    // create the depth buffer texture
    D3D11_TEXTURE2D_DESC texd;
    ZeroMemory(&texd, sizeof(texd));

    texd.Width = SCREEN_WIDTH;
    texd.Height = SCREEN_HEIGHT;
    texd.ArraySize = 1;
    texd.MipLevels = 1;
    texd.SampleDesc.Count = 4;
    texd.Format = DXGI_FORMAT_D32_FLOAT;
    texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D *pDepthBuffer;
    dev->CreateTexture2D(&texd, NULL, &pDepthBuffer);

    // create the depth buffer
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));

    dsvd.Format = DXGI_FORMAT_D32_FLOAT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    dev->CreateDepthStencilView(pDepthBuffer, &dsvd, &zbuffer);
    pDepthBuffer->Release();

    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, zbuffer);


    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;    // set the left to 0
    viewport.TopLeftY = 0;    // set the top to 0
    viewport.Width = SCREEN_WIDTH;    // set the width to the window's width
    viewport.Height = SCREEN_HEIGHT;    // set the height to the window's height
    viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
    viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0

    devcon->RSSetViewports(1, &viewport);

    InitPipeline();
    InitGraphics();
}


// this is the function used to render a single frame
void RenderFrame(void)
{
    D3DXMATRIX matTranslation, matView, matRotate, matProjection, matScale;
    D3DXMATRIX matFinal;

    // create a view matrix
    D3DXMatrixLookAtLH(&matView,
                       &D3DXVECTOR3(CameraX, CameraY, CameraZ),    // the camera position
                       &D3DXVECTOR3(CameraLookX, CameraLookY, CameraLookZ),    // the look-at position
                       &D3DXVECTOR3(0.0f, 1.0f, 0.0f));   // the up direction

    // create a projection matrix
    D3DXMatrixPerspectiveFovLH(&matProjection,
                               (FLOAT)D3DXToRadian(45),                    // field of view
                               (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
                               1.0f,                                       // near view-plane
                               100.0f);                                    // far view-plane

    // clear the back buffer to a deep blue
    devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

    // clear the depth buffer
    devcon->ClearDepthStencilView(zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);

        // select which vertex buffer to display
        UINT stride = sizeof(VERTEX);
        UINT offset = 0;
        devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);
        devcon->IASetIndexBuffer(pIBuffer, DXGI_FORMAT_R32_UINT, 0);

        // select which primtive type we are using
        devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Calls for all objects to perform their render
	for(it01 = objects.begin(); it01 != objects.end(); ++it01) {
			(*it01)->Render(devcon, &textures, pCBuffer, matTranslation,
				            matRotate, matScale, matFinal, matView, matProjection, red, green, blue);
		}
	for(sc01 = scoreobjects.begin(); sc01 != scoreobjects.end(); ++sc01) {
			(*sc01)->Render(devcon, &textures, pCBuffer, matTranslation,
				            matRotate, matScale, matFinal, matView, matProjection, red, green, blue);
		}

    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}


// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    // close and release all existing COM objects
    zbuffer->Release();
    pLayout->Release();
    pVS->Release();
    pPS->Release();
    pVBuffer->Release();
    pIBuffer->Release();
    pCBuffer->Release();
	for(int killtex = 0; killtex < textures.size(); killtex++ )
	{
		textures.at(killtex)->Release();
	}
	textures.clear();
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
	engine->Release(); //Relase audio engine
}


// this is the function that creates the shape to render
void InitGraphics()
{
    // create vertices to represent the corners of the cube
    VERTEX OurVertices[] =
    {
        {-1.0f, -1.0f, 1.0f, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f, 0.0f},    // side 1
        {1.0f, -1.0f, 1.0f, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f},
        {-1.0f, 1.0f, 1.0f, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1.0f, 0.0f},
        {1.0f, 1.0f, 1.0f, D3DXVECTOR3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f},

        {-1.0f, -1.0f, -1.0f, D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f},    // side 2
        {-1.0f, 1.0f, -1.0f, D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0.0f, 1.0f},
        {1.0f, -1.0f, -1.0f, D3DXVECTOR3(0.0f, 0.0f, -1.0f), 1.0f, 0.0f},
        {1.0f, 1.0f, -1.0f, D3DXVECTOR3(0.0f, 0.0f, -1.0f), 1.0f, 1.0f},

        {-1.0f, 1.0f, -1.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f},    // side 3
        {-1.0f, 1.0f, 1.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 0.0f, 1.0f},
        {1.0f, 1.0f, -1.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, 0.0f},
        {1.0f, 1.0f, 1.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), 1.0f, 1.0f},

        {-1.0f, -1.0f, -1.0f, D3DXVECTOR3(0.0f, -1.0f, 0.0f), 0.0f, 0.0f},    // side 4
        {1.0f, -1.0f, -1.0f, D3DXVECTOR3(0.0f, -1.0f, 0.0f), 0.0f, 1.0f},
        {-1.0f, -1.0f, 1.0f, D3DXVECTOR3(0.0f, -1.0f, 0.0f), 1.0f, 0.0f},
        {1.0f, -1.0f, 1.0f, D3DXVECTOR3(0.0f, -1.0f, 0.0f), 1.0f, 1.0f},

        {1.0f, -1.0f, -1.0f, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.0f, 0.0f},    // side 5
        {1.0f, 1.0f, -1.0f, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f},
        {1.0f, -1.0f, 1.0f, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 1.0f, 0.0f},
        {1.0f, 1.0f, 1.0f, D3DXVECTOR3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f},

        {-1.0f, -1.0f, -1.0f, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0.0f, 0.0f},    // side 6
        {-1.0f, -1.0f, 1.0f, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 0.0f, 1.0f},
        {-1.0f, 1.0f, -1.0f, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 1.0f, 0.0f},
        {-1.0f, 1.0f, 1.0f, D3DXVECTOR3(-1.0f, 0.0f, 0.0f), 1.0f, 1.0f},
    };

    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(VERTEX) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    dev->CreateBuffer(&bd, NULL, &pVBuffer);

    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    devcon->Unmap(pVBuffer, NULL);


    // create the index buffer out of DWORDs
    DWORD OurIndices[] = 
    {
        0, 1, 2,    // side 1
        2, 1, 3,
        4, 5, 6,    // side 2
        6, 5, 7,
        8, 9, 10,    // side 3
        10, 9, 11,
        12, 13, 14,    // side 4
        14, 13, 15,
        16, 17, 18,    // side 5
        18, 17, 19,
        20, 21, 22,    // side 6
        22, 21, 23,
    };

    // create the index buffer
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = sizeof(DWORD) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bd.MiscFlags = 0;

    dev->CreateBuffer(&bd, NULL, &pIBuffer);

    devcon->Map(pIBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurIndices, sizeof(OurIndices));                   // copy the data
    devcon->Unmap(pIBuffer, NULL);

	ID3D11ShaderResourceView *pTexture;    // the pointer to the texture

	
    D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/Player1.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading
	textures.push_back(pTexture);



	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/Player2.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading
	
	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/Metal.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/dongtitlescreen.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/dongtitlescreen2.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/space.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/iron.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/playcheat.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/illumunati.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/dew.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/doritos.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);

	D3DX11CreateShaderResourceViewFromFile(dev,        // the Direct3D device
                                       L"textures/nicememe.png",    
                                       NULL,           // no additional information
                                       NULL,           // no multithreading
                                       &pTexture,      // address of the shader-resource-view
                                       NULL);          // no multithreading

	textures.push_back(pTexture);
}


// this function loads and prepares the shaders
void InitPipeline()
{
    // compile the shaders
    ID3D10Blob *VS, *PS;
    D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

    // create the shader objects
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);

    // create the input element object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    // use the input element descriptions to create the input layout
    dev->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);

    // create the constant buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = 176;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    dev->CreateBuffer(&bd, NULL, &pCBuffer);

    devcon->VSSetConstantBuffers(0, 1, &pCBuffer);
}

void UpdateProgram(void)
{
	for(sc01 = scoreobjects.begin(); sc01 != scoreobjects.end(); ++sc01)
			(*sc01)->Update(CameraLookY);

	for(it01 = objects.begin(); it01 != objects.end(); ++it01)
			(*it01)->Update();

	//--Checking objects for collisions
	for(it01 = objects.begin(); it01 != objects.end(); ++it01)
		{
			if( ! (*it01)->Collidable() ) continue;
			if( ! (*it01)->CheckShow() ) continue;
			for(it02 = it01; it02 != objects.end(); ++it02)
			{
				if( !(*it02)->Collidable() ) continue;
				if( (*it01)->GetID() == (*it02)->GetID()) continue;

				if( (*it01)->CheckCollisions( (*it02)))
				{
					(*it01)->Collided( (*it02));
					(*it02)->Collided( (*it01));
					if((*it01)->CheckObjectID(PLAYER) || (*it01)->CheckObjectID(PLAYER2)) {
						if(!MLG)
						sfx->SubmitSourceBuffer(soundbuffer.xaBuffer() );
						if(MLG)
						msfx->SubmitSourceBuffer(msoundbuffer.xaBuffer() );
					}
					if((*it01)->CheckObjectID(BORDER)) {
						if(!MLG)
						sfx2->SubmitSourceBuffer(sound2buffer.xaBuffer() );
						if(MLG)
						msfx2->SubmitSourceBuffer(msound2buffer.xaBuffer() );
					}
				}
			}
		}
}

void Scored(Paddle *player)
{
	player->Score();	
	ScoreItem *score = new ScoreItem(player->GetScore(), player->GetID());
	scoreobjects.push_back(score);
}

void GameWon()
{
	for(int delay = 80000000; delay > 0; delay--)
	{
		CameraX = 0.0f;
		CameraY = 0.0f;
		CameraZ = 0.0f;
		CameraLookX = 0.0f;
		CameraLookY = 0.0f;
		CameraLookZ = 0.0f;
	}
	MLG = false;
	gamestate = TITLE;
}

void RunTitleScreen(TitleScreen *title)
{
	for(sc01 = scoreobjects.begin(); sc01 != scoreobjects.end(); )
	{
		delete (*sc01);
		sc01 = scoreobjects.erase(sc01);
	}
	title->SetShow(true);
	if(ai)
		title->SetTexNum(4);
	else
		title->SetTexNum(5);
	for(it01 = objects.begin(); it01 != objects.end(); ++it01)
	{
		(*it01)->Reset();
	}
	if(musicplaying) {
		music->Stop();
		music->FlushSourceBuffers();
		music->Start();
		music->SubmitSourceBuffer(music2buffer.xaBuffer() );
		musicplaying = false;
	}
	lastscored = 0;
}