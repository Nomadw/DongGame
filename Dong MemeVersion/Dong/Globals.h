#pragma once

#include "stdafx.h"
// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>
#include <XAudio2.h>
#include <XAudio2fx.h>
#include <xnamath.h>
#include <dinput.h>
#include <vector>
using namespace std;

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

// define the screen resolution
#define SCREEN_WIDTH  1600
#define SCREEN_HEIGHT 1000

#define BALLSPEED 0.0006f
#define BALLSPINSPEED 0.01f
#define PADDLESPEED 0.0004f
#define BACKGROUNDSPEED 0.00005f

// a struct to define a single vertex
struct VERTEX {FLOAT X, Y, Z; D3DXVECTOR3 Normal; FLOAT U, V;};
struct PERFRAME{D3DXCOLOR Color; FLOAT X, Y, Z;};

// a struct to define the constant buffer
struct CBUFFER
{
    D3DXMATRIX Final;
	D3DMATRIX Rotation;
    D3DXVECTOR4 LightVector;
    D3DXCOLOR LightColor;
    D3DXCOLOR AmbientColor;
};

//--enumberate a list of Object IDs - For use in objects
enum ID{PLAYER, PLAYER2, BALL, BORDER};

enum STATE{PLAYING, TITLE};