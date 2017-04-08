#include "stdafx.h"
#include "Globals.h"

extern char dierr_msg[128]; 
extern dword err; 

// direct input errors // Use with these functions: 
extern char *dierrCreate (dword err); // DirectInputCreate 
extern char *dierrCreateDevice (dword err); // CreateDevice 
extern char *dierrCooperate (dword err); // SetCooperativeLevel 
extern char *dierrSetDataFormat (dword err); // SetDataFormat 
extern char *dierrAcquire (dword err); // Acquire 
extern char *dierrGetDeviceState (dword err); // GetDeviceState