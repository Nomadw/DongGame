#pragma once

// Include headers accessible 
#include "Globals.h"

class Paddle : public MainObject
{
public:

	int score;

	void Update();
	void Reset();
	Paddle(int ID);
	void MoveUp();
	void MoveDown();
	void Score() {score++;}
	int GetScore() {return score;}
	void ResetScore() {score = 0;}
	void ResetDir() {y_dir = 0;}
};