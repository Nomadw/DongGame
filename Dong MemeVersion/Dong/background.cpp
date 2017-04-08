#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 
#include "MainObject.h"
#include "background.h"

Background::Background()
{
	MainObject::Init(0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	MainObject::SetScaleX(15.0f);
	MainObject::SetScaleY(12.0f);
	MainObject::SetScaleZ(0.001f);
	MainObject::SetTexNum(6);

	angle = 0.0f;
	MLG = false;
	SetShow(false);
	SetCollidable(false);
}

void Background::Update()
{
	float originX = 0;
	float originY = 0;

	float Size = 5.5f;

	x = originX + (sin(angle)*Size);
	y = originY + (cos(angle)*Size);

	if(MLG) {
	angle += (BACKGROUNDSPEED * 100);
	SetTexNum(12);
	}
	if(!MLG) {
	angle += BACKGROUNDSPEED;
	SetTexNum(6);
	}
}

void Background::Reset()
{
	SetX(0.0f);
	SetY(0.0f);
	SetShow(false);
	angle = 0.0f;
	MLG = false;
}