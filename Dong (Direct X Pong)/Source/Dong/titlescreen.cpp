#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 
#include "MainObject.h"
#include "titlescreen.h"

TitleScreen::TitleScreen()
{
	MainObject::Init(0.0f, 0.0f, -17.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	MainObject::SetScaleX(10.0f);
	MainObject::SetScaleY(16.0f);
	MainObject::SetScaleZ(0.1f);
	MainObject::SetTexNum(4);

	MainObject::SetRz(D3DXToRadian(270));

	SetShow(true);
	SetCollidable(false);
}