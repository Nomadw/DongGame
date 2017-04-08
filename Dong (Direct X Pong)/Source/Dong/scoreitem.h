#pragma once

// Include headers accessible 
#include "Globals.h"

class ScoreItem : public MainObject
{
public:
	ScoreItem(int no, int scoreID);
	void Update(float CameraY);
};