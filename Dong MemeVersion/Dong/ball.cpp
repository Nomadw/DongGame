#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 
#include "MainObject.h"
#include "ball.h"

Ball::Ball()
{
	MainObject::Init(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.125f, 0.125f);

	MainObject::SetScaleX(0.125f);
	MainObject::SetScaleY(0.125f);
	MainObject::SetScaleZ(0.125f);
	MainObject::SetTexNum(1);

	x_dir = 0;
	y_dir = 0;
	passed = false;
	mousemove = true;
	Object_ID = BALL;
	MLG = 0;
	SetShow(false);
}

void Ball::ResetPosition(int start, float x, float y, int ydir)
{
	x_dir = 0;
	y_dir = 0;
	SetX(x);
	SetY(y);

	SetShow(true);
	
	if(start == PLAYER) {
		x_dir = 1;
	}
	else if(start == PLAYER2) {
		x_dir = 2;
	}

	if(ydir == 1)
		y_dir = 1;
	else if(ydir == 2)
		y_dir = 2;
	else y_dir = 0;

	passed = false;
	mousemove = false;
}
void Ball::Update()
{
	ry += BALLSPINSPEED;
	rx += BALLSPINSPEED;
	rz += BALLSPINSPEED;
	if(ry >= D3DXToRadian(360))
		ry = 0;
	if(rx >= D3DXToRadian(360))
		rx = 0;
	if(rz >= D3DXToRadian(360))
		rz = 0;

	if(!MLG) {
	if(x_dir == 1) {
		x -= BALLSPEED;
		SetTexNum(1);
	}
	if(x_dir == 2) {
		x += BALLSPEED;
		SetTexNum(2);
	}
	if(y_dir == 1)
		y += BALLSPEED;
	if(y_dir == 2)
		y -= BALLSPEED;
	}

	if(MLG) {
		SetTexNum(9);
	if(x_dir == 1) {
		x -= (BALLSPEED*2);
	}
	if(x_dir == 2) {
		x += (BALLSPEED*2);
	}
	if(y_dir == 1)
		y += (BALLSPEED*2);
	if(y_dir == 2)
		y -= (BALLSPEED*2);
	}

	if(mousemove) {
		y_dir = 0;
		if(!MLG)
		SetTexNum(8);
	}

	if(x > 7) {
		Reset();
	}
	else if(x < -7) {
		Reset();
	}

}

void Ball::Collided(MainObject *otherObject) 
{
	if(otherObject->GetID() == PLAYER || otherObject->GetID() == PLAYER2)
	{
	if(x_dir == 1)
		x_dir = 2;
	else if(x_dir == 2)
		x_dir = 1;

	if(otherObject->GetY() > y)
		y_dir = 2;
	if(otherObject->GetY() < y)
		y_dir = 1;
	}
	mousemove = false;

	if(otherObject->GetID() == BORDER)
	{
		if(y_dir == 1)
			y_dir = 2;
		else y_dir = 1;
	}
}

void Ball::Reset()
{
	SetX(0.0f);
	SetY(0.0f);
	passed = false;
	mousemove = true;
	Object_ID = BALL;
	SetShow(false);
	x_dir = 0;
	y_dir = 0;
}