/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/undo_blocked_input.hpp"

int undo_blocked_Fl_Float_Input::handle(int e){

	switch (e){
		case FL_KEYBOARD:
      	// Undo?
	      	if ((Fl::event_state()&(FL_CTRL|FL_COMMAND)) && ((Fl::event_key()=='z')||(Fl::event_key()=='y'))) {	        		      		
	        	return 0; //gobble ctrl+z and ctrl+shift+z commands to block undo, but send up 0 so that ctrl+z can be claimed by the main window
	      	}
	      	return Fl_Float_Input::handle(e);
	      	
	    default:
	    	return Fl_Float_Input::handle(e);
	}

}

int undo_blocked_Fl_Int_Input::handle(int e){

	switch (e){
		case FL_KEYBOARD:
      	// Undo?
	      	if ((Fl::event_state()&(FL_CTRL|FL_COMMAND)) && ((Fl::event_key()=='z')||(Fl::event_key()=='y'))) {	        	
	        	return 0; //gobble ctrl+z and ctrl+shift+z commands to block undo, but send up 0 so that ctrl+z can be claimed by the main window
	      	}
	      	return Fl_Int_Input::handle(e);
	      	
	    default:
	    	return Fl_Int_Input::handle(e);
	}

}

int undo_blocked_Fl_Input::handle(int e){

	switch (e){
		case FL_KEYBOARD:
      	// Undo?
	      	if ((Fl::event_state()&(FL_CTRL|FL_COMMAND)) && ((Fl::event_key()=='z')||(Fl::event_key()=='y'))) {	        		      		
	        	return 0; //gobble ctrl+z/y and ctrl+shift+z/y commands to block undo, but send up 0 so that ctrl+z can be claimed by the main window
	      	}
	      	return Fl_Input::handle(e);
	      	
	    default:
	    	return Fl_Input::handle(e);
	}

}