#pragma once

// Include headers accessible 
#include "Globals.h"

class Background : public MainObject
{
public:
	Background();
	void Update();
	void Reset();
	float angle;
};