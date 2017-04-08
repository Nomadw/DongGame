#pragma once

// Include headers accessible 
#include "Globals.h"

class Border : public MainObject
{
public:
	Border(float x, float y);
	void Reset();
	void Update();
};