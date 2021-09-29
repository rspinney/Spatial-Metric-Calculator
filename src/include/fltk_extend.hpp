/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef WIN_EXTEND_H
#define WIN_EXTEND_H

#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <stdlib.h>

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Simple_Counter.H>
#include <FL/Fl_Check_Button.H>

#include "./preprocessor.hpp"
#include "./tabs_fltk_1-4-0.hpp"

class base;


#undef min
#undef max

#define BORDER 2
#define EXTRASPACE 10
#define SELECTION_BORDER 5


class cursor_text : public Fl_Text_Display{
    
    base* program;
    int handle(int);
    
public:
    cursor_text(int X, int Y, int W, int H, const char * L,base* program_):Fl_Text_Display(X,Y,W, H, L),program(program_)
    {}
};


class double_win_extend: public Fl_Double_Window{
		base* program;
		int handle(int);
		
	public:
		int xtemp,ytemp,wtemp,htemp;
		void resize(int,int,int,int);
		double_win_extend(int X, int Y, int W, int H, const char* l, base* program_):Fl_Double_Window(X,Y,W,H,l),program(program_){};
};


class win_extend: public Fl_Window{
		int handle(int);
		
	public:
		
		win_extend(int X, int Y, int W, int H, const char* l):Fl_Window(X,Y,W,H,l){};
};


class win_extend2: public Fl_Window{
		int handle(int);
		
	public:
		
		win_extend2(int X, int Y, int W, int H, const char* l):Fl_Window(X,Y,W,H,l){};
};

class win_extend_angle: public Fl_Window{
		int handle(int);
		
	public:
		
		win_extend_angle(int X, int Y, int W, int H, const char* l):Fl_Window(X,Y,W,H,l){};
};

class win_extend_edit: public Fl_Window{
		int handle(int);
		
	public:
		
		win_extend_edit(int X, int Y, int W, int H, const char* l):Fl_Window(X,Y,W,H,l){};
};

class menu_extend: public Fl_Sys_Menu_Bar{
	int handle(int);
public:
	menu_extend(int X,int Y, int W, int H):Fl_Sys_Menu_Bar(X,Y,W,H){};
};


class choice_extend: public Fl_Choice{
	void draw() {
		Fl_Boxtype btype = Fl::scheme() ? FL_UP_BOX           // non-default uses up box
		                              : FL_DOWN_BOX;        // default scheme uses down box
		int dx = Fl::box_dx(btype);
		int dy = Fl::box_dy(btype);

		// Arrow area
		int H = h() - 2 * dy;
		int W = Fl::is_scheme("gtk+")    ? 20 :                       // gtk+  -- fixed size
		      Fl::is_scheme("gleam")   ? 20 :                       // gleam -- fixed size
		      Fl::is_scheme("plastic") ? ((H > 20) ? 20 : H)        // plastic: shrink if H<20
		                               : ((H > 20) ? 20 : H);       // default: shrink if H<20



		int X = x() + w() - W - dx;
		int Y = y() + dy;

		// Arrow object
		int w1 = (W - 4) / 3; if (w1 < 1) w1 = 1;
		int x1 = X + (W - 2 * w1 - 1) / 2;
		int y1 = Y + (H - w1 - 1) / 2;

		if (Fl::scheme()) {
			// NON-DEFAULT SCHEME

			// Draw widget box
			Fl_Color col = color();
			if (Fl::focus() == this){			
#if THEME == 0			
				col = fl_darker(fl_darker(color()));	
#elif THEME ==1
				col = fl_color_average(fl_darker(color()), color(), 0.25f);
#endif
			}
			draw_box(btype, col);

			// Draw arrow area
			fl_color(active_r() ? labelcolor() : fl_inactive(labelcolor()));
			if (Fl::is_scheme("plastic")) {
			  // Show larger up/down arrows...
				fl_polygon(x1, y1 + 3, x1 + w1, y1 + w1 + 3, x1 + 2 * w1, y1 + 3);
				fl_polygon(x1, y1 + 1, x1 + w1, y1 - w1 + 1, x1 + 2 * w1, y1 + 1);
			} else {
			  // Show smaller up/down arrows with a divider...
				x1 = x() + w() - 13 - dx;
				y1 = y() + h() / 2;
				fl_polygon(x1, y1 - 2, x1 + 3, y1 - 5, x1 + 6, y1 - 2);
				fl_polygon(x1, y1 + 2, x1 + 3, y1 + 5, x1 + 6, y1 + 2);

				fl_color(fl_darker(color()));
				fl_yxline(x1 - 7, y1 - 8, y1 + 8);

				fl_color(fl_lighter(color()));
				fl_yxline(x1 - 6, y1 - 8, y1 + 8);
			}
		} else {
			// DEFAULT SCHEME

			// Draw widget box
			if (fl_contrast(textcolor(), FL_BACKGROUND2_COLOR) == textcolor()) {
			  draw_box(btype, FL_BACKGROUND2_COLOR);
			} else {
			  draw_box(btype, fl_lighter(color()));
			}

			// Draw arrow area
			draw_box(FL_UP_BOX,X,Y,W,H,color());
			fl_color(active_r() ? labelcolor() : fl_inactive(labelcolor()));
			fl_polygon(x1, y1, x1 + w1, y1 + w1, x1 + 2 * w1, y1);
		}

		W += 2 * dx;

		// Draw menu item's label
		if (mvalue()) {
			Fl_Menu_Item m = *mvalue();
			if (active_r()) m.activate(); else m.deactivate();

			// Clip
			int xx = x() + dx, yy = y() + dy + 1, ww = w() - W, hh = H - 2;
			fl_push_clip(xx, yy, ww, hh);

			if ( Fl::scheme()) {
				Fl_Label l;
				l.value = m.text;
				l.image = 0;
				l.deimage = 0;
				l.type = m.labeltype_;
				l.font = m.labelsize_ || m.labelfont_ ? m.labelfont_ : textfont();
				l.size = m.labelsize_ ? m.labelsize_ : textsize();
				l.color= m.labelcolor_ ? m.labelcolor_ : textcolor();
				if (!m.active()) l.color = fl_inactive((Fl_Color)l.color);
				fl_draw_shortcut = 2; // hack value to make '&' disappear
				l.draw(xx+3, yy, ww>6 ? ww-6 : 0, hh, FL_ALIGN_LEFT);
				fl_draw_shortcut = 0;

				// if ( Fl::focus() == this ){ 
				// 	Fl_Color tc = color(); // hack to get it to draw a black dotted outline
				// 	color(FL_WHITE);
				// 	draw_focus(box(), xx, yy-1, ww, hh+2);
				// 	color(tc);
				// }
			}
			else {
				fl_draw_shortcut = 2; // hack value to make '&' disappear
				m.draw(xx, yy, ww, hh, this, Fl::focus() == this);
				fl_draw_shortcut = 0;
			}

			fl_pop_clip();
		}

		// Widget's label
		draw_label();
	}



public:
	choice_extend(int X,int Y, int W, int H, const char* l):Fl_Choice(X,Y,W,H,l){
		//this->box(_FL_PLASTIC_THIN_UP_BOX);
	};
};



class check_extend: public Fl_Check_Button{

	void draw() {

	  if (box()) draw_box(this==Fl::pushed() ? fl_down(box()) : box(), color());
	  Fl_Color col = value() ? (active_r() ? selection_color() :
	                            fl_inactive(selection_color())) : color();

	  int W  = labelsize();
	  int bx = Fl::box_dx(box());   // box frame width
	  int dx = bx + 2;              // relative position of check mark etc.
	  int dy = (h() - W) / 2;       // neg. offset o.k. for vertical centering
	  int lx = 0;                   // relative label position (STR #3237)

#if THEME == 0 //different amounts of color averaging
	  if (Fl::focus() == this) draw_box(FL_FLAT_BOX,x()+lx, y()+2, w()-lx-bx, h()-4,fl_color_average(FL_GRAY, color(), 0.85f));
#elif THEME == 1
	  if (Fl::focus() == this) draw_box(FL_FLAT_BOX,x()+lx, y()+2, w()-lx-bx, h()-4,fl_color_average(FL_GRAY, color(), 0.45f));	  
#endif

	  if (down_box()) {
	    // draw other down_box() styles:
	    switch (down_box()) {
	      case FL_DOWN_BOX :
	      case FL_UP_BOX :
	      case _FL_PLASTIC_DOWN_BOX :
	      case _FL_PLASTIC_UP_BOX :
	      	
	        // Check box...
	        //draw_box(down_box(), x()+dx, y()+dy, W, W, FL_BACKGROUND2_COLOR);
	      	//would like to have black outline even in plastic scheme
	      	draw_box(FL_GTK_DOWN_BOX, x()+dx, y()+dy, W, W, FL_BACKGROUND2_COLOR);
	        if (value()) {
	          if (Fl::is_scheme("gtk+")) {
	            fl_color(FL_SELECTION_COLOR);
	          } else {
	            fl_color(col);
	          }
	          int tx = x() + dx + 3;
	          int tw = W - 6;
	          int d1 = tw/3;
	          int d2 = tw-d1;
	          int ty = y() + dy + (W+d2)/2-d1-2;
	          for (int n = 0; n < 3; n++, ty++) {
	            fl_line(tx, ty, tx+d1, ty+d1);
	            fl_line(tx+d1, ty+d1, tx+tw-1, ty+d1-d2+1);
	          }
	        }
	        break;
	      case _FL_ROUND_DOWN_BOX :
	      case _FL_ROUND_UP_BOX :
	        // Radio button...
	        draw_box(down_box(), x()+dx, y()+dy, W, W, FL_BACKGROUND2_COLOR);
	        if (value()) {
	          int tW = (W - Fl::box_dw(down_box())) / 2 + 1;
	          if ((W - tW) & 1) tW++; // Make sure difference is even to center
	          int tdx = dx + (W - tW) / 2;
	          int tdy = dy + (W - tW) / 2;

	          if (Fl::is_scheme("gtk+")) {
	            fl_color(FL_SELECTION_COLOR);
	            tW --;
	            fl_pie(x() + tdx - 1, y() + tdy - 1, tW + 3, tW + 3, 0.0, 360.0);
	            fl_color(fl_color_average(FL_WHITE, FL_SELECTION_COLOR, 0.2f));
	          } else fl_color(col);

	          switch (tW) {
	            // Larger circles draw fine...
	            default :
	              fl_pie(x() + tdx, y() + tdy, tW, tW, 0.0, 360.0);
	              break;

	            // Small circles don't draw well on many systems...
	            case 6 :
	              fl_rectf(x() + tdx + 2, y() + tdy, tW - 4, tW);
	              fl_rectf(x() + tdx + 1, y() + tdy + 1, tW - 2, tW - 2);
	              fl_rectf(x() + tdx, y() + tdy + 2, tW, tW - 4);
	              break;

	            case 5 :
	            case 4 :
	            case 3 :
	              fl_rectf(x() + tdx + 1, y() + tdy, tW - 2, tW);
	              fl_rectf(x() + tdx, y() + tdy + 1, tW, tW - 2);
	              break;

	            case 2 :
	            case 1 :
	              fl_rectf(x() + tdx, y() + tdy, tW, tW);
	              break;
	          }

	          if (Fl::is_scheme("gtk+")) {
	            fl_color(fl_color_average(FL_WHITE, FL_SELECTION_COLOR, 0.5));
	            fl_arc(x() + tdx, y() + tdy, tW + 1, tW + 1, 60.0, 180.0);
	          }
	        }
	        break;
	      default :
	        draw_box(down_box(), x()+dx, y()+dy, W, W, col);
	        break;
	    }
	    lx = dx + W + 2;
	  } else {
	    // if down_box() is zero, draw light button style:
	    int hh = h()-2*dy - 2;
	    int ww = W/2+1;
	    int xx = dx;
	    if (w()<ww+2*xx) xx = (w()-ww)/2;
	    if (Fl::is_scheme("plastic")) {
	    	if (Fl::focus() == this){
	    	  fl_color(fl_rgb_color(255,0,0));
		      fl_pie(x()+xx, y()+dy+1, ww, hh, 0, 360);
	    	}
	    	else{
		      col = active_r() ? selection_color() : fl_inactive(selection_color());
		      fl_color(value() ? col : fl_color_average(col, FL_BLACK, 0.5f));
		      fl_pie(x()+xx, y()+dy+1, ww, hh, 0, 360);
		  	}
	    } else {
	      draw_box(FL_THIN_DOWN_BOX, x()+xx, y()+dy+1, ww, hh, col);
	    }
	    lx = dx + ww + 2;
	  }
	  draw_label(x()+lx, y(), w()-lx-bx, h());
	  
	}

public:
	check_extend(int X,int Y, int W, int H, const char* l):Fl_Check_Button(X,Y,W,H,l){
		//this->box(_FL_PLASTIC_THIN_UP_BOX);
	};
};


class button_extend: public Fl_Button{

	void draw() {
		if (type() == FL_HIDDEN_BUTTON) return;
		Fl_Color col = value() ? selection_color() : color();
		if (Fl::focus() == this){
#if THEME == 0			
			col = fl_darker(fl_darker(color()));	
#elif THEME ==1
			col = fl_color_average(fl_darker(color()), color(), 0.25f);
#endif
		}
		draw_box(value() ? (down_box()?down_box():fl_down(box())) : box(), col);
		//draw_backdrop();
		if (labeltype() == FL_NORMAL_LABEL && value()) {
		Fl_Color c = labelcolor();
		labelcolor(fl_contrast(c, col));
		draw_label();
		labelcolor(c);
		} else draw_label();
		// if (Fl::focus() == this){ 
		// 	Fl_Color tc = color(); // hack to get it to draw a black dotted outline
		// 	color(FL_WHITE);
		// 	draw_focus();
		// 	color(tc);
		// }
	}

public:
	button_extend(int X,int Y, int W, int H, const char* l):Fl_Button(X,Y,W,H,l){
		//this->box(_FL_PLASTIC_THIN_UP_BOX);
	};
};



#if (FL_MAJOR_VERSION == 1) && (FL_MINOR_VERSION > 3)
	#define EXTEND_TABS 1 //extend but just use native fltk tabs widget
	#define TAB_CLASS Fl_Tabs
#elif (FL_MAJOR_VERSION == 1) &&(FL_MINOR_VERSION == 3) && (FL_PATCH_VERSION > 5)
	#define EXTEND_TABS 1  //extend and use the code for the fltk 1.4.0 tabs widget which allow for draw adjustments
	#define TAB_CLASS fl_tabs_1_4_0  //tested on fltk v 1.3.7 + v 1.3.6 only
#else
	#define EXTEND_TABS 0 //dont extend, just use native fl_tabs widget without draw adjustments
	#define TAB_CLASS Fl_Tabs //not tested on these releases
#endif
	

class tabs_extend: public TAB_CLASS{


#if EXTEND_TABS //not tested on anything before fltk v1.3.6

	enum {LEFT, RIGHT, SELECTED};
	void draw() {
		Fl_Widget *v = value();
		int H = tab_height();

		if (damage() & FL_DAMAGE_ALL) { // redraw the entire thing:
		Fl_Color c = v ? v->color() : color();

		draw_box(box(), x(), y()+(H>=0?H:0), w(), h()-(H>=0?H:-H), c);

		if (selection_color() != c) {
			// Draw the top or bottom SELECTION_BORDER lines of the tab pane in the
			// selection color so that the user knows which tab is selected...
			int clip_y = (H >= 0) ? y() + H : y() + h() + H - SELECTION_BORDER;
			fl_push_clip(x(), clip_y, w(), SELECTION_BORDER);
			draw_box(box(), x(), clip_y, w(), SELECTION_BORDER, selection_color());
			fl_pop_clip();
		}
		if (v) draw_child(*v);
		} else { // redraw the child
			if (v) update_child(*v);
		}
		if (damage() & (FL_DAMAGE_SCROLL|FL_DAMAGE_ALL)) {
			const int nc = children();
			int selected = tab_positions();
			int i;
			Fl_Widget*const* a = array();
			for (i=0; i<selected; i++)
			draw_tab(x()+tab_pos[i], x()+tab_pos[i+1],
			tab_width[i], H, a[i], LEFT);
			for (i=nc-1; i > selected; i--)
			draw_tab(x()+tab_pos[i], x()+tab_pos[i+1],
			tab_width[i], H, a[i], RIGHT);
			if (v) {
				i = selected;
				draw_tab(x()+tab_pos[i], x()+tab_pos[i+1],
				tab_width[i], H, a[i], SELECTED);
			}
		}
	}


void draw_tab(int x1, int x2, int W, int H, Fl_Widget* o, int what) {
	int sel = (what == SELECTED);
	int dh = Fl::box_dh(box());
	int dy = Fl::box_dy(box());
	char prev_draw_shortcut = fl_draw_shortcut;
	fl_draw_shortcut = 1;

	Fl_Boxtype bt = box();//= (o == push_ && !sel) ? fl_down(box()) : box();
	//Fl_Color bc = sel ? selection_color() : o->selection_color();
	//Fl_Color bc = sel ? fl_darker(fl_darker(color())) : color(); //<<------------selected tab is darker

	//Fl_Boxtype bt = FL_FRAME_BOX;// _FL_PLASTIC_DOWN_BOX;

#if THEME == 0			
	Fl_Color bc = sel ? fl_darker(fl_darker(color())) : color(); //<<------------selected tab is darker
#elif THEME ==1
	Fl_Color bc = sel ? fl_color_average(fl_darker(color()), color(), 0.35f) : color(); //<<------------selected tab is darker
#endif

	//bc = color();

	// Save the label color and label type
	Fl_Color oc = o->labelcolor();
	Fl_Labeltype ot = o->labeltype();

	// Set a labeltype that really draws a label
	if (ot == FL_NO_LABEL)
	o->labeltype(FL_NORMAL_LABEL);

	// compute offsets to make selected tab look bigger
	int yofs = sel ? 0 : BORDER;

	if ((x2 < x1+W) && what == RIGHT) x1 = x2 - W;

		if (H >= 0) {
			if (sel) fl_push_clip(x1, y(), x2 - x1, H + dh - dy);
			else fl_push_clip(x1, y(), x2 - x1, H);

			H += dh;

			draw_box(bt, x1, y() + yofs, W, H + 10 - yofs, bc);

			// Draw the label using the current color...
			o->labelcolor(sel ? labelcolor() : o->labelcolor());
			o->draw_label(x1, y() + yofs, W, H - yofs, tab_align());

			//if (Fl::focus() == this && o->visible()) //remove focus box
			//	draw_focus(box(), x1, y(), W, H);

			fl_pop_clip();
		} else {
			H = -H;

			if (sel) fl_push_clip(x1, y() + h() - H - dy, x2 - x1, H + dy);
			else fl_push_clip(x1, y() + h() - H, x2 - x1, H);

			H += dh;

			draw_box(bt, x1, y() + h() - H - 10, W, H + 10 - yofs, bc);

			// Draw the label using the current color...
			o->labelcolor(sel ? labelcolor() : o->labelcolor());
			o->draw_label(x1, y() + h() - H, W, H - yofs, tab_align());

			//if (Fl::focus() == this && o->visible())   //remove focus box
			//draw_focus(box(), x1, y() + h() - H, W, H);

			fl_pop_clip();
		}
		fl_draw_shortcut = prev_draw_shortcut;

		// Restore the original label color and label type
		o->labelcolor(oc);
		o->labeltype(ot);
	}

#endif

	public:
	tabs_extend(int X,int Y, int W, int H):TAB_CLASS(X,Y,W,H){};
};

#undef TAB_CLASS
#undef EXTEND_TABS

class counter_extend: public Fl_Counter{


	int mouseobj;
	int calc_mouseobj() {
	  if (type() == FL_NORMAL_COUNTER) {
	    int W = w()*15/100;
	    if (Fl::event_inside(x(), y(), W, h())) return 1;
	    if (Fl::event_inside(x()+W, y(), W, h())) return 2;
	    if (Fl::event_inside(x()+w()-2*W, y(), W, h())) return 3;
	    if (Fl::event_inside(x()+w()-W, y(), W, h())) return 4;
	  } else {
	    int W = w()*20/100;
	    if (Fl::event_inside(x(), y(), W, h())) return 2;
	    if (Fl::event_inside(x()+w()-W, y(), W, h())) return 3;
	  }
	  return -1;
	}

	int handle(int msg){
		int ret = Fl_Counter::handle(msg);
		if (msg == FL_PUSH){
			mouseobj = calc_mouseobj();
			return 1;
		}
		else{
			return ret;
		}
	}

	void draw() {
	  int i; Fl_Boxtype boxtype[5];
	  Fl_Color selcolor;


	  boxtype[0] = box();
	  if (boxtype[0] == FL_UP_BOX) boxtype[0] = FL_DOWN_BOX;
	  if (boxtype[0] == FL_THIN_UP_BOX) boxtype[0] = FL_THIN_DOWN_BOX;
	  for (i=1; i<5; i++){
	      if (mouseobj == i){
	        	boxtype[i] = fl_down(FL_UP_BOX);//fl_down(box());
	    		mouseobj=-1;
	      }
	      else{
	       boxtype[i] = FL_FLAT_BOX;//FL_THIN_UP_BOX;//box();
	   	  }
	  }
	  int xx[5], ww[5];
	  if (type() == FL_NORMAL_COUNTER) {
	    int W = w()*15/100;
	    xx[1] = x();         ww[1] = W;
	    xx[2] = x()+1*W;     ww[2] = W;
	    xx[0] = x()+2*W;     ww[0] = w()-4*W;
	    xx[3] = x()+w()-2*W; ww[3] = W;
	    xx[4] = x()+w()-1*W; ww[4] = W;
	  } else {
	    int W = w()*20/100;
	    xx[1] = 0;           ww[1] = 0;
	    xx[2] = x();         ww[2] = W;
	    xx[0] = x()+W;       ww[0] = w()-2*W;
	    xx[3] = x()+w()-1*W; ww[3] = W;
	    xx[4] = 0;           ww[4] = 0;
	  }

	  draw_box(boxtype[0], xx[0], y(), ww[0], h(), FL_BACKGROUND2_COLOR);
	  fl_font(textfont(), textsize());
	  fl_color(active_r() ? textcolor() : fl_inactive(textcolor()));
	  char str[128]; format(str);
	  fl_draw(str, xx[0], y(), ww[0], h(), FL_ALIGN_CENTER);
	  //if (Fl::focus() == this) draw_focus(boxtype[0], xx[0], y(), ww[0], h());
	  if (!(damage()&FL_DAMAGE_ALL)) return; // only need to redraw text

	  if (active_r())
	    selcolor = labelcolor();
	  else
	    selcolor = fl_inactive(labelcolor());


		//Fl_Color col = color();
		Fl_Color col = fl_color_average(FL_GRAY, color(), 0.75f);
		if (Fl::focus() == this){
			//col = fl_darker(fl_darker(color())); 
			col = fl_color_average(FL_GRAY, fl_darker(fl_darker(color())), 0.75f);	
		}


	  if (type() == FL_NORMAL_COUNTER) {
	    draw_box(boxtype[1], xx[1], y(), ww[1], h(), col);
	    fl_draw_symbol("@-5<<", xx[1], y()+2, ww[1], h()-4, selcolor);
	  }
	  draw_box(boxtype[2], xx[2], y(), ww[2], h(), col);
	  fl_draw_symbol("@-5<",  xx[2], y()+2, ww[2], h()-4, selcolor);
	  draw_box(boxtype[3], xx[3], y(), ww[3], h(), col);
	  fl_draw_symbol("@-5>",  xx[3], y()+2, ww[3], h()-4, selcolor);
	  if (type() == FL_NORMAL_COUNTER) {
	    draw_box(boxtype[4], xx[4], y(), ww[4], h(), col);
	    fl_draw_symbol("@-5>>", xx[4], y()+2, ww[4], h()-4, selcolor);
	  }
	}
	public:
	counter_extend(int X,int Y, int W, int H, const char* l):Fl_Counter(X,Y,W,H,l),mouseobj(-1){};
};

class simple_counter_extend: public counter_extend{
	public:
		simple_counter_extend(int X,int Y,int W,int H, const char *L):counter_extend(X,Y,W,H,L) {
  		type(FL_SIMPLE_COUNTER);
	}
};

#undef BORDER
#undef EXTRASPACE
#undef SELECTION_BORDER


class button_ex : public button_extend{
    
    base* program;
    int handle (int e){
        
        
        
        if (e==8){
            if (Fl::event_key(FL_Enter)){

                this->do_callback(this,program);
               
                return 1;
                
            }
        }
        
        return button_extend::handle(e);
    }
    
public:
    button_ex(int X, int Y, int W, int H, const char * L,base* program_):button_extend(X,Y,W, H, L),program(program_)
    {}
};



#endif //WIN_EXTEND_H

