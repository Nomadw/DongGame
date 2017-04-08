#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 
#include "MainObject.h"
#include "scoreitem.h"

ScoreItem::ScoreItem(int no, int scoreID)
{
	MainObject::Init(0.0f, 2.5f, 0.5f, 0.0f, 0.0f, 0.25f, 0.25f);
	MainObject::SetScaleX(0.125f);
	MainObject::SetScaleY(0.125f);
	MainObject::SetScaleZ(0.125f);

	MainObject::SetID(scoreID);

	if(scoreID == PLAYER)
	{
		MainObject::SetTexNum(1);
		
		SetX((no * 0.35f) + 0.15f);
	}

 	if(scoreID == PLAYER2)
	{
		MainObject::SetTexNum(2);

		SetX((-no * 0.35f) - 0.15f);
	}
	
}

void ScoreItem::Update(float CameraY)
{
	SetY(CameraY + 2.5f);
		if(Object_ID == PLAYER)
			SetTexNum(1);
		if(Object_ID == PLAYER2)
			SetTexNum(2);

}