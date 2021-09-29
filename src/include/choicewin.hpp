/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef CHOICEWIN_H
#define CHOICEWIN_H


#include "config.h"

#if defined(WIN32)
	#include "stdafx.h"
#endif

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include "preprocessor.hpp"
#include "base.hpp"
#include "fltk_extend.hpp"

class choicewin{

	Fl_Window *win;
	button_extend *but1,*but2,*but3;
	Fl_Box *message_box; 
	base *program;
	std::vector<std::string> messages;
	int &state;
	int built;

	static void one_cb(Fl_Widget*,void* data){
		choicewin *self = (choicewin*) data;
		self->state = 0;
		self->win->hide();
		
	}

	static void two_cb(Fl_Widget*,void* data){
		choicewin *self = (choicewin*) data;
		self->state = 1;
		self->win->hide();
		
	}

	static void three_cb(Fl_Widget*,void* data){
		choicewin *self = (choicewin*) data;
		self->state = 2;
		self->win->hide();
		
	}

	static void null_cb(Fl_Widget*,void* data){
		choicewin *self = (choicewin*) data;
		self->win->hide();
		
	}

	void delete_self(){
      delete this;
   }

	public:

	choicewin(base* prog_in,int &s,const char* str0,const char* str1,const char* str2,const char* str3,const char* str4):program(prog_in),state(s),built(0){
		state = -1;
		messages.push_back(str0);
		messages.push_back(str1);
		messages.push_back(str2);
		messages.push_back(str3);
		messages.push_back(str4);
	}

	~choicewin(){
		if (built){
			delete win; //will delete child widgets
		}
	}
	void show(){

		int w  = 400;
		int h = 150;

		if (messages[0].size()<150){
			h=130;
		}

		if (messages[0].size()>170){
			h=165;
			w=370;
		}

		if (messages[0].size()>200){
			h=165;
			w=420;
		}


		if (messages[0].size()>250){
			h=185;
			w=440;
		}	

		if (messages[0].size()>325){
			h=220;
			w=500;
		}


    	Fl_Color back_color = fl_rgb_color(247,247,247);
		Fl_Color button_color = fl_rgb_color(BUTTON_R,BUTTON_G,BUTTON_B);

		int x = program->win->x()+0.5*program->win->w()-0.5*w;
		int y = program->win->y()+0.5*program->win->h()-0.5*h;

		int but_w = 90;
		int gap = 15;
		int but_x1 = w - 3*but_w -3*gap-5;
		int but_x2 = w - 2*but_w -2*gap-5;
		int but_x3 = w - 1*but_w -1*gap-5;

		win = new Fl_Window(x,y,w,h,messages[4].c_str());
		win->color(back_color);
		win->box(FL_FLAT_BOX);
		Fl_Group *par = new Fl_Group(0,0,win->w(),win->h());
	    par->box(FL_FLAT_BOX);
	    par->color(back_color);
		win->set_modal();
        win->begin();
		built=1;

		message_box = new Fl_Box(20,20,win->w()-40,h-70,messages[0].c_str());
		message_box->align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE|FL_ALIGN_WRAP);
    
    	if (messages[3].size()>0){
    
	    	but1 = new button_extend(but_x1,h-40,but_w,25,messages[1].c_str());
	    	but1->callback(one_cb,this);
	    	but1->color(button_color);
	    	but2 = new button_extend(but_x2,h-40,but_w,25,messages[2].c_str());
	    	but2->callback(two_cb,this);
	    	but2->color(button_color);
	    	but3 = new button_extend(but_x3,h-40,but_w,25,messages[3].c_str());
	    	but3->callback(three_cb,this);
	    	but3->color(button_color);
    	}
    	else if(messages[2].size()>0){
    		but1 = new button_extend(but_x2,h-40,but_w,25,messages[1].c_str());
	    	but1->callback(one_cb,this);
	    	but1->color(button_color);
	    	but2 = new button_extend(but_x3,h-40,but_w,25,messages[2].c_str());
	    	but2->callback(two_cb,this);
	    	but2->color(button_color);

    	}
    	else{
    		but1 = new button_extend(but_x3,h-40,but_w,25,messages[1].c_str());
	    	but1->callback(one_cb,this);
	    	but1->color(button_color);
    	}

    	par->end();
    	win->end();
    	win->callback(null_cb,this);
		win->show();
		while (win->shown()) Fl::wait();

		delete_self();
	}

};

#endif /*CHOICEWIN_H */