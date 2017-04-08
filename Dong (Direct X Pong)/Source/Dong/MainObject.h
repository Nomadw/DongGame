#pragma once

// Include headers accessible 
#include "Globals.h"

//--The main Object base class. All other objects/classes inherit from here

class MainObject
{
private:
	bool show;				// On screen (or not)
	bool collidable;		// can collide with other objects

protected:
	float x;				// x - coordinate
	float y;				// y - coordinate
	float z;				// z - coordinate

	float ry;				//rotation
	float rx;
	float rz;

	float x_bound;			// width of bounding box	
	float y_bound;			// height of bounding box

	//bool show;

	int x_dir;				// direction x is moving (left or right)
	int y_dir;				// direction y is moving (up or down)

	int texturenumber;
	float scalex;
	float scaley;
	float scalez;

public:
	// Constructor
	MainObject();
	int Object_ID;			// ID number
	// Virtual methods (may be replaced by 'child' methods)

	void virtual Render();
	void virtual Update();
	void virtual Render(ID3D11DeviceContext *devcon, vector <ID3D11ShaderResourceView *> *textures, ID3D11Buffer *pCBuffer, D3DXMATRIX matTranslation,
						D3DXMATRIX matRotate, D3DXMATRIX matScale, D3DXMATRIX matFinal, D3DXMATRIX matView,
						D3DXMATRIX matProjection, float red, float green, float blue);

	void virtual Collided(MainObject *otherObject);
	void virtual Collided(int Object_ID);

	void virtual Reset();

	// ACCESSOR methods (reads and returns a value)
	int GetID() {return Object_ID;}
	//bool GetAlive() {return alive;}
	bool GetCollidable() {return collidable;}

	float GetX() {return x;}
	float GetY() {return y;}
	float GetZ() {return z;}

	float GetRx() {return rx;}
	float GetRy() {return ry;}
	float GetRz() {return rz;}

	float GetX_Bound() {return x_bound;}
	float GetY_Bound() {return y_bound;}

	float GetScaleX() {return scalex;}
	float GetScaleY() {return scaley;}
	float GetScaleZ() {return scalez;}

	int GetTexNum() {return texturenumber;}

	bool CheckObjectID(int ID);
	bool CheckShow() {return show;}

	int GetXdir() {return x_dir;}
	int GetYdir() {return y_dir;}

	// MUTATOR methods (changes a value)
	void SetID(int ID) {MainObject::Object_ID = ID;}
	//void SetAlive(bool alive) {MainObject::alive = alive;}
	void SetCollidable(bool collidable) {MainObject::collidable = collidable;}

	void SetX(float x) {MainObject::x = x;}
	void SetY(float y) {MainObject::y = y;}
	void SetZ(float z) {MainObject::z = z;}

	void SetRx(float x) {MainObject::rx = x;}
	void SetRy(float y) {MainObject::ry = y;}
	void SetRz(float z) {MainObject::rz = z;}

	void SetScaleX(float x) {MainObject::scalex = x;}
	void SetScaleY(float y) {MainObject::scaley = y;}
	void SetScaleZ(float z) {MainObject::scalez = z;}

	void SetTexNum(int texnum) {MainObject::texturenumber = texnum;}

	void SetShow(bool show) {MainObject::show = show;}

	void SetXDir(int x) {MainObject::x_dir = x;}
	void SetYDir(int y) {MainObject::y_dir = y;}

	// Additional methods
	void Init(float x, float y, float z, float x_dir, float y_dir, float x_bound, float y_bound);
	bool CheckCollisions(MainObject *otherObject);
	bool Collidable();
};