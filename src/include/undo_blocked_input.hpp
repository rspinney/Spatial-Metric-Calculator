/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef UNDO_BLOCKED_H
#define UNDO_BLOCKED_H

#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <stdlib.h>

#include <FL/Fl.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Int_Input.H>

#undef min
#undef max

class undo_blocked_Fl_Float_Input : public Fl_Float_Input{
		
		int handle(int);
		
	public:
		undo_blocked_Fl_Float_Input(int X, int Y, int W, int H, const char* l):Fl_Float_Input(X,Y,W,H,l){};
		~undo_blocked_Fl_Float_Input(){};
};

class undo_blocked_Fl_Int_Input : public Fl_Int_Input{
		
		int handle(int);
		
	public:
		undo_blocked_Fl_Int_Input(int X, int Y, int W, int H, const char* l):Fl_Int_Input(X,Y,W,H,l){};
		~undo_blocked_Fl_Int_Input(){};
};


class undo_blocked_Fl_Input : public Fl_Input{
		
		int handle(int);
		
	public:
		undo_blocked_Fl_Input(int X, int Y, int W, int H, const char* l):Fl_Input(X,Y,W,H,l){};
		~undo_blocked_Fl_Input(){};
};

#endif //UNDO_BLOCKED_H

