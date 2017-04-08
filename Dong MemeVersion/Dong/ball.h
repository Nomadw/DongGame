#pragma once

// Include headers accessible 
#include "Globals.h"

class Ball : public MainObject
{
public:

	bool passed;
	bool mousemove;
	Ball();
	void Update();
	void Reset();
	void ResetPosition(int start, float x, float y, int ydir);
	bool GetPass() {return Ball::passed;}
	void SetPassed(bool pass) {Ball::passed = pass;}
	void Collided(MainObject *otherObject);
};