/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef R_TEXT_H
#define R_TEXT_H

#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <FL/Fl.H>
#include <FL/Fl_Text_Display.H>
#include <stdlib.h>

#undef min
#undef max

class base;

class reporting_text_display : public Fl_Text_Display{
		base* program;
		int handle(int);

	public:
		reporting_text_display(int X, int Y, int W, int H, const char* l, base* program_):Fl_Text_Display(X,Y,W,H,l),program(program_){};
};

#endif //R_TEXT_H

