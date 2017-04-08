#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 
#include "MainObject.h"
#include "paddle.h"

Paddle::Paddle(int ID)
{
	MainObject::Init(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.125f, 0.5f);

	MainObject::SetScaleX(0.125f);
	MainObject::SetScaleY(0.5f);
	MainObject::SetScaleZ(0.125f);
	MainObject::SetID(ID);

	if(ID == PLAYER) {
		MainObject::SetX(4.0f);
		MainObject::SetY(-0.5f);
		MainObject::SetTexNum(1);
	}
	if(ID == PLAYER2) {
		MainObject::SetX(-4.0f);
		MainObject::SetY(0.5f);
		MainObject::SetTexNum(2);
	}
	SetShow(false);
	MLG = 0;
}

void Paddle::MoveUp()
{
	if(!MLG)
	Paddle::y += PADDLESPEED;
	if(MLG)
	Paddle::y += (PADDLESPEED * 2);
	y_dir = 1;
}
void Paddle::MoveDown()
{
	if(!MLG)
	Paddle::y -= PADDLESPEED;
	if(MLG)
	Paddle::y -= (PADDLESPEED * 2);
	y_dir = 2;
}

void Paddle::Update()
{
	if(y > 2.5)
		y = 2.5;
	else if(y < -2.5)
		y = -2.5;
	if(MLG) {
		SetTexNum(10);
		ry += BALLSPINSPEED;
	}
	if(!MLG)
		ry = 0;
}

void Paddle::Reset()
{
	if(Object_ID == PLAYER) {
		MainObject::SetX(4.0f);
		MainObject::SetY(-0.5f);
		MainObject::SetTexNum(1);
	}
	if(Object_ID == PLAYER2) {
		MainObject::SetX(-4.0f);
		MainObject::SetY(0.5f);
		MainObject::SetTexNum(2);
	}
	ResetScore();
	ResetDir();
	SetShow(false);
	MLG = 0;
}