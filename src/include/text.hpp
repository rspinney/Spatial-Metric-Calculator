/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef TEXT_H
#define TEXT_H

#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <stdlib.h>
#include <string>
#include <FL/Fl.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>


#undef min
#undef max


struct text_output{
    Fl_Text_Buffer* buff;
    Fl_Text_Display* display;
};


struct update{
    text_output* t;
    std::string s;
};

#endif //TEXT_H

