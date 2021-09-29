/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/


#ifndef BUTTON_EX_H
#define BUTTON_EX_H

#include <FL/Fl_Button.H>

class button_ex : public Fl_Button{
    
    base* program;
    int handle (int e){
        
        if (e==8){
            if (Fl::event_key(FL_Enter)){

                this->do_callback(this,program);
               
                return 1;
                
            }
        }
        
        return Fl_Button::handle(e);
    }
    
public:
    button_ex(int X, int Y, int W, int H, const char * L,base* program_):Fl_Button(X,Y,W, H, L),program(program_)
    {}
};


#endif /*BUTTON_EX_H*/

