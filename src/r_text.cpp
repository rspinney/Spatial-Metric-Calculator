/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/base.hpp"
#include "./include/r_text.hpp"

int reporting_text_display::handle(int e){
	
	int ret = Fl_Text_Display::handle(e);

	switch (e){
		case FL_KEYBOARD:
      	// Copy?
	      	if ((Fl::event_state()&(FL_CTRL|FL_COMMAND)) && Fl::event_key()=='c') {
	        	program->push_time();
	        	program->push_output("Copied!\n");
	        	return 1;
	      	}
	      	return ret;
	    default:
	    	return ret;
	}

}