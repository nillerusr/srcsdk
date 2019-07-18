/*
consoledisplay.cpp - Console display func implementation
Copyright (C) a1batross

Licensed under WTFPL License
*/

#include "wrapper.h"
#include "icvar.h"

ICvar *icvar;

class CAndroidConsoleDisplayFunc : public IConsoleDisplayFunc 
{
public:
	virtual void ColorPrint( const Color& clr, const char *pMessage )
	{
		LogPrintf( "ColorPrint: %s", pMessage );
	}
	virtual void Print( const char *pMessage )
	{
		LogPrintf( "Print: %s", pMessage );
	}
	virtual void DPrint( const char *pMessage )
	{
		LogPrintf( "DPrint: %s", pMessage );
	}
} displayFunc;
IConsoleDisplayFunc *g_pAndroidDisplayFunc = &displayFunc;

void VStdLib_Init( Module *vstdlib )
{
	/*CreateInterfaceFn (*func)(void) = (CreateInterfaceFn (*)(void))vstdlib->Resolve( "VStdLib_GetICVarFactory" );
	
	if( func )
	{
		CreateInterfaceFn factory = func();
		icvar = (ICvar*)factory(CVAR_INTERFACE_VERSION, NULL);
		icvar->InstallConsoleDisplayFunc( g_pAndroidDisplayFunc );
		icvar->ConsoleDPrintf( "AndroidConsoleDisplayFunc is installed! Starting receiving console log" );
	}*/
}

WRAP_LIBRARY( VStdLib, "libvstdlib.so", VStdLib_Init );
