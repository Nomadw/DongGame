#include "stdafx.h"
#include "Globals.h"
// Include headers accessible 
#include "MainObject.h"


//--Functions for the base Object class. All other classes inherit from here

// Constructor
MainObject::MainObject()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;

	rx = 0.0f;
	ry = 0.0f;
	rz = 0.0f;

	x_bound = 0.0f;
	y_bound = 0.0f;

	show = true;
	collidable = true;
}

void MainObject::Update()
{

}

void MainObject::Init(float x, float y, float z, float x_dir, float y_dir, float x_bound, float y_bound) 
{
	MainObject::x = x;
	MainObject::y = y;
	MainObject::z = z;

	MainObject::x_bound = x_bound;
	MainObject::y_bound = y_bound;
}

void MainObject::Render()
{

}

void MainObject::Reset()
{

}

void MainObject::Render(ID3D11DeviceContext *devcon, vector <ID3D11ShaderResourceView *> *textures, ID3D11Buffer *pCBuffer, D3DXMATRIX matTranslation,
						D3DXMATRIX matRotate, D3DXMATRIX matScale, D3DXMATRIX matFinal, D3DXMATRIX matView,
						D3DXMATRIX matProjection, float red, float green, float blue)
{
	if(CheckShow()) { //If the object is not on show, do not render
		CBUFFER cBuffer;

		cBuffer.LightVector = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 0.0f);
		cBuffer.LightColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		cBuffer.AmbientColor = D3DXCOLOR(red, green, blue, 1.0f);

		D3DXMatrixRotationX(&matRotate, rx);
		D3DXMatrixRotationY(&matRotate, ry);
		D3DXMatrixRotationZ(&matRotate, rz);
		D3DXMatrixScaling(&matScale, scalex, scaley, scalez);
		D3DXMatrixTranslation(&matTranslation, x, y, z);
		
		// load the matrices into the constant buffer
		cBuffer.Final =  matScale * matRotate * matTranslation * matView * matProjection;
		cBuffer.Rotation = matRotate;

        // draw the object
        devcon->UpdateSubresource(pCBuffer, 0, 0, &cBuffer, 0, 0);
		devcon->PSSetShaderResources(0, 1, &textures->at(texturenumber - 1));
        devcon->DrawIndexed(36, 0, 0);
	}
}

bool MainObject::CheckCollisions(MainObject *otherObject)
{
	float oX = otherObject->GetX();
	float oY = otherObject->GetY();

	float obX = otherObject->GetX_Bound();
	float obY = otherObject->GetY_Bound();

	if( x + x_bound > oX - obX &&
		x - x_bound < oX + obX &&
		y + y_bound > oY - obY &&
		y - y_bound < oY + obY)
		return true;
	else
		return false;
}

bool MainObject::CheckObjectID(int ID)
{
	if( Object_ID == ID)
		return true;
	else
		return false;
}

void MainObject::Collided(int objectID)
{}

void MainObject::Collided(MainObject *otherObject) 
{

}

bool MainObject::Collidable()
{
	return collidable;
}