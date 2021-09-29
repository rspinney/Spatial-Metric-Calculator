/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/base.hpp"
#include "./include/fltk_extend.hpp"

int cursor_text::handle (int e){
                
    if (e==FL_LEAVE){
    	program->root.cursor=1;
    }
    
    return Fl_Text_Display::handle(e);
}		


void double_win_extend::resize(int x,int y,int w,int h){
	
	xtemp = x;
	ytemp = y;
	wtemp = w;
	htemp = h;
	
	if (!program->root.resized){
		program->root.resized=1;
	}
	
	Fl_Double_Window::resize(x,y,w,h); // let fltk resize the window after capturing the size 

	if (program->root.fullscreen){
		program->draw->resize(0,0,this->w(),this->h());// maintain full screen
	}

	program->info_bar_cb(); //total hack
	program->info_bar_cb();
	program->panel_cb();
	program->panel_cb();

}

int double_win_extend::handle(int e){


	int ret = Fl_Double_Window::handle(e);

	switch (e){
		case FL_LEFT_MOUSE:
			return ret;
		case FL_MOVE:
			//if (program->oo->value()==program->o1){
				if ((!Fl::event_inside(program->out_read))&&(!Fl::event_inside(program->draw))){
					if (program->root.cursor!=1){
						cursor(FL_CURSOR_DEFAULT);
						program->root.cursor=1;
					}
				}
			//}

			//if (program->oo->value()==program->o3){
				if ((!Fl::event_inside(program->out_read))&&(!Fl::event_inside(program->plot))){
					if (program->root.cursor!=1){
						cursor(FL_CURSOR_DEFAULT);
						program->root.cursor=1;
					}
				}
			//}

			return ret;
		case FL_KEYBOARD:
			if ((Fl::event_state()) && (Fl::event_key()==FL_Escape)){
				if (program->root.fullscreen){
					program->go_fullscreen();
				}
			}
			else if(program->oo->value()==program->o1){

				if (Fl::event_state()&(FL_CTRL|FL_COMMAND))  {
					if (Fl::event_key()==FL_Right){
						program->oo->value(program->o2);
						return 1;
					}
				}

				if (Fl::event_state()&(FL_CTRL|FL_COMMAND))  {
		      		if (Fl::event_key()=='1'){
		      			program->three_d_choice->value(0);
		      			program->three_d_choice->do_callback();
		      			return 1;
		      		}
		      		else if (Fl::event_key()=='2'){
		      			program->three_d_choice->value(1);
		      			program->three_d_choice->do_callback();
		      			return 1;
		      		}
		      		else if (Fl::event_key()=='3'){
		      			program->three_d_choice->value(2);
		      			program->three_d_choice->do_callback();
		      			return 1;
		      		}
		      		else if (Fl::event_key()=='4'){
		      			program->three_d_choice->value(3);
		      			program->three_d_choice->do_callback();
		      			return 1;
		      		}
		      	}
		      	else if (Fl::event_state()&(FL_ALT))  {
		      		
		      		if (Fl::event_key()=='0'){
		      			program->mark_locations_choice->value(0);
		      			program->mark_locations_choice->do_callback();
		      			return 1;
		      		}
		      		else if (Fl::event_key()=='1'){
		      			program->mark_locations_choice->value(1);
		      			program->mark_locations_choice->do_callback();
		      			return 1;
		      		}
		      		else if (Fl::event_key()=='2'){
		      			program->mark_locations_choice->value(10);
		      			program->mark_locations_choice->do_callback();
		      			return 1;
		      		}
		      		else if (Fl::event_key()=='3'){
		      			program->mark_locations_choice->value(11);
		      			program->mark_locations_choice->do_callback();
		      			return 1;
		      		}
		      		else if (Fl::event_key()=='4'){
		      			program->mark_locations_choice->value(12);
		      			program->mark_locations_choice->do_callback();
		      			return 1;
		      		}
		      		else if (Fl::event_key()=='5'){
		      			program->mark_locations_choice->value(13);
		      			program->mark_locations_choice->do_callback();
		      			return 1;
		      		}
		      		else if (Fl::event_key()=='6'){
		      			program->mark_locations_choice->value(14);
		      			program->mark_locations_choice->do_callback();
		      			return 1;
		      		}
		      		else if(Fl::event_key()=='['){
		      			int val = std::max(0,program->mark_locations_choice->value()-1); 
		      			program->mark_locations_choice->value(val);
		      			program->mark_locations_choice->do_callback();
		      			return 1;	
		      		}
		      		else if(Fl::event_key()==']'){
		      			int val = std::min(14,program->mark_locations_choice->value()+1); 
		      			program->mark_locations_choice->value(val);
		      			program->mark_locations_choice->do_callback();
		      			return 1;	
		      		}
		      	}
		      	else if (Fl::event_state()&(FL_SHIFT))  {
		      		if (Fl::event_key()=='0'){
		      			program->floor_choice->value(0);
		      			program->floor_choice->do_callback();
		      			return 1;
		      		}
		      		else if(Fl::event_key()=='['){
		      			int val = std::max(0,program->floor_choice->value()-1); 
		      			program->floor_choice->value(val);
		      			program->floor_choice->do_callback();
		      			return 1;	
		      		}
		      		else if(Fl::event_key()==']'){
		      			int val = std::min(program->floor_choice->size()-1,program->floor_choice->value()+1); 
		      			program->floor_choice->value(val);
		      			program->floor_choice->do_callback();
		      			return 1;	
		      		}
		      		else if(Fl::event_key()=='1'){
		      			program->building_choice->value(0);
		      			program->building_choice->do_callback();
		      			return 1;	
		      		}
		      		else if(Fl::event_key()=='2'){
		      			program->building_choice->value(1);
		      			program->building_choice->do_callback();
		      			return 1;	
		      		}
		      		else if(Fl::event_key()=='3'){
		      			program->building_choice->value(2);
		      			program->building_choice->do_callback();
		      			return 1;	
		      		}
		      		else if(Fl::event_key()=='4'){		      			
		      			program->building_choice->value(3);
		      			program->building_choice->do_callback();
		      			return 1;	
		      		}
		      	}
	      	}
	      	else if(program->oo->value()==program->o2){


	      		if (Fl::event_state()&(FL_SHIFT))  {
		      		if(Fl::event_key()=='1'){
		      			program->building_data_choice->value(0);
		      			program->building_data_choice->do_callback();
		      			return 1;	
		      		}
		      		else if(Fl::event_key()=='2'){
		      			program->building_data_choice->value(1);
		      			program->building_data_choice->do_callback();
		      			return 1;	
		      		}
		      		else if(Fl::event_key()=='3'){
		      			program->building_data_choice->value(2);
		      			program->building_data_choice->do_callback();
		      			return 1;	
		      		}
		      		else if(Fl::event_key()=='4'){		      			
		      			program->building_data_choice->value(3);
		      			program->building_data_choice->do_callback();
		      			return 1;	
		      		}
		      	}

				if (Fl::event_state()&(FL_CTRL|FL_COMMAND))  {
					if (Fl::event_key()==FL_Right){
						program->oo->value(program->o3);
						return 1;
					}
					if (Fl::event_key()==FL_Left){
						program->oo->value(program->o1);
						return 1;
					}
				}
			}
			else if(program->oo->value()==program->o3){

				if (Fl::event_state()&(FL_CTRL|FL_COMMAND))  {
				
					if (Fl::event_key()==FL_Left){
						program->oo->value(program->o2);
						return 1;
					}
				}
			}
	      	return ret;
	    default:
	    	return ret;
	}

	return ret;
}

int win_extend::handle(int e){
	
	int ret = Fl_Window::handle(e);

#if defined __APPLE__ || defined WIN32 // Avoid hide/show input spamming leading to breaking keyboard input on Linux/X11

	if (e==FL_KEYBOARD){
		if (Fl::event_state()&(FL_ALT))  {
      		if (Fl::event_key()=='m'){
      			do_callback();
      			return 1;
      		}
      	}
    }
#endif

    return ret;
}


int win_extend2::handle(int e){
	
	int ret = Fl_Window::handle(e);

#if defined __APPLE__ || defined WIN32 // Avoid hide/show input spamming leading to breaking keyboard input on Linux/X11

	if (e==FL_KEYBOARD){
		if (Fl::event_state()&(FL_ALT))  {
      		if (Fl::event_key()=='f'){
      			do_callback();
      			return 1;
      		}
      	}
    }
#endif

    return ret;
}

int win_extend_angle::handle(int e){
	
	int ret = Fl_Window::handle(e);

#if defined __APPLE__ || defined WIN32 // Avoid hide/show input spamming leading to breaking keyboard input on Linux/X11

	if (e==FL_KEYBOARD){
		if (Fl::event_state()&(FL_ALT))  {
      		if (Fl::event_key()=='a'){
      			do_callback();
      			return 1;
      		}
      	}
    }
#endif

    return ret;
}

int win_extend_edit::handle(int e){
	
	int ret = Fl_Window::handle(e);

#if defined __APPLE__ || defined WIN32 // Avoid hide/show input spamming leading to breaking keyboard input on Linux/X11

	if (e==FL_KEYBOARD){
		if (Fl::event_state()&(FL_ALT))  {
      		if (Fl::event_key()=='e'){
      			do_callback();
      			return 1;
      		}
      	}
    }
#endif

    return ret;
}

int menu_extend::handle(int e){
	

	int ret = Fl_Sys_Menu_Bar::handle(e);


	// if ((Fl::focus()==this)||(Fl::event_inside(this))){
	// 	if (program->root.cursor!=1){
	// 		cursor(FL_CURSOR_DEFAULT);
	// 		program->root.cursor=1;
	// 	}
	// 	return 1;
	// }

    return ret;
}