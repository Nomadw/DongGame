#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 
#include "MainObject.h"
#include "border.h"

Border::Border(float x, float y)
{
	MainObject::Init(x, y, 0.0f, 0.0f, 0.0f, 10.0f, 0.125f);

	MainObject::SetScaleX(10.0f);
	MainObject::SetScaleY(0.125f);
	MainObject::SetScaleZ(0.125f);
	MainObject::SetTexNum(7);
	MainObject::SetID(BORDER);
	MLG = false;
	SetShow(false);
	SetCollidable(true);
}

void Border::Update()
{
	if(!MLG)
		SetTexNum(7);
	if(MLG)
		SetTexNum(11);
}

void Border::Reset()
{
	SetShow(false);
	MLG = false;
}