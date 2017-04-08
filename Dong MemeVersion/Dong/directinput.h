#include "stdafx.h"
#include "Globals.h"

#define MOUSEL 0 
#define MOUSER 1 
#define MOUSEM 2 
// mouse_t; defines the mouse data struct 
typedef struct tagMouse 
{ 
	int x, y, 
	pinstate, 
	pin; 
	DIMOUSESTATE state; 
} mouse_t; 

// keyboard_t; defines the keyboard data struct 
typedef struct tagKeyboard 
{ 
	byte state[256]; 
} keyboard_t; 

extern mouse_t mse; 
extern keyboard_t key; 

extern LPDIRECTINPUT lpdi; // DI interface 
extern LPDIRECTINPUTDEVICE lpKeyboard; // keyboard device 
extern LPDIRECTINPUTDEVICE lpMouse; // mouse device 

void I_Init (HWND hwnd, HINSTANCE instance); // initializes DI interface 
void I_Shutdown (void); // shuts down DI interface 
void I_DeviceInit (void); // resets keyboard and mouse structs to 0 
void I_Get (void); // gets mouse and keyboard input 
void I_GetMouse (void); // gets mouse input only 
void I_GetKeyboard (void); // gets keyboard input only 
void I_ReaquireKeyboard (void);  //reaquire the keyboard device 
void I_ReaquireMouse (void); // reaquire the mouse device