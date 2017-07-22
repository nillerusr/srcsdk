/*
cvar.cpp -- ICvar wrap
Copyright (C) 2017 a1batross

Licensed under WTFPL license.
Thanks to Valve for SDK, me for idea. 
Please, don't punish, Mr. Newell. :)
*/ 

#include "icvar.h"

ICVar *g_pCVar = NULL;

class CWrapICVar : public ICVar
{
	
}


EXPOSE_SINGLE_INTERFACE_GLOBALVAR( 
