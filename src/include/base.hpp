/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef BASE_H
#define BASE_H

#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <vector>
#include <map>
#include <deque>
#include <ctime>
#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <thread> 
#include <mutex>  
#include <chrono> 
#include <sstream>
#include <stdexcept>
#include <memory>
#include <stdlib.h>

#include <FL/Fl_Tooltip.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/Fl_Sys_Menu_Bar.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Simple_Counter.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Pixmap.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Tabs.H>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#define BOOST_DATE_TIME_NO_LIB
#include <boost/date_time/posix_time/posix_time.hpp>

#include "thread_utils.hpp"
#include "enums.hpp"
#include "errors.hpp"
#include "preprocessor.hpp"
#include "graph.hpp"
#include "building.hpp"
#include "draw_window.hpp"
#include "locations.hpp"
#include "plot.hpp"
#include "dxf_parser.hpp"
#include "container.hpp"
#include "text.hpp"
#include "fltk_extend.hpp"
#include "matrix.hpp"
#include "graph.hpp"
#include "building.hpp"
#include "draw_window.hpp"
#include "locations.hpp"
#include "r_text.hpp"
#include "undo_blocked_input.hpp"

#undef min
#undef max


class base{

	friend class double_win_extend;

	Fl_Box *out_read_box, *draw_box,*out_data_box,*graph_box;

	int MEMENTO_DEPTH;

	int wtemp,htemp,xtemp,ytemp; 
    int wtemp1,htemp1,xtemp1,ytemp1;

	int loaded;

	std::string desks_file;
	std::string points_file;
	std::string matrix_file;
	
	int wall_floor;
	int wall_type;
	

	Fl_Window* route_display;
	choice_extend* route_type_c;
	Fl_Int_Input *start_route_input,*end_route_input;

	win_extend_edit* add_link_win;
	win_extend_angle* set_ang_win;
	Fl_Int_Input *start_add_link,*end_add_link,*memento_depth_input;
	Fl_Float_Input *ang_input;

	check_extend* plan_colors_button;
	check_extend* splines_button;

	button_extend* layers_button;
	button_extend* blocks_button;

	Fl_Window* memento_depth_win;




	public:

	text_output* data_text;
	
	choice_extend* building_choice;
	choice_extend* building_data_choice;

	choice_extend* show_visibility;

	choice_extend* floor_choice;
	button_extend* add_floor;
	button_extend* remove_floor; 
	choice_extend* three_d_choice;
	choice_extend* data_choice;
	choice_extend* data_format;
	choice_extend* show_desks_choice;
	choice_extend* mark_locations_choice;


	choice_extend* desk_metric_floor_choice;
	choice_extend* desk_metric_visibility_choice;
	choice_extend* desk_metric_stat_choice;
	choice_extend* desk_metric_type_choice;
	choice_extend* desk_metric_basis_choice;
	choice_extend* desk_metric_destination_choice;


	choice_extend* metric_slot_choice;

	choice_extend *custom_metric_choice,*route_type_choice,*custom_metric_floor_choice,*custom_metric_visibility_choice,*custom_metric_stat_choice,*custom_metric_destination_choice,*destination_criteria_choice,*custom_metric_basis,*custom_metric_self_count,*route_stat_choice;

	check_extend* inverse_option;
	check_extend* cache_metric_data;

	Fl_Float_Input*  eig_tol;

	choice_extend* custom_metric_range_choice;

	Fl_Value_Input* custom_metric_range;
	Fl_Value_Input* custom_metric_range2;
	Fl_Int_Input* max_iterations;

	button_extend *ask1,*ask2,*ask3,*ask4,*ask5,*ask6,*ask7,*ask8,*ask9,*ask10,*ask11,*ask12,*ask13,*ask14;

	Fl_Group* hist_group;

	choice_extend* plot_choice;
	choice_extend* hist_interp_choice;
	choice_extend* show_regression;
	choice_extend* yaxis_choice;
	Fl_Counter* num_bin_count;


	std::vector<Fl_Box*> hist_box;
	std::vector<choice_extend*> hist_building;
	std::vector<choice_extend*> hist_metric;
	std::vector<Fl_Counter*> bin_count;

	std::vector<check_extend*> show_locations;
	check_extend *show_walls_sitting,*show_walls_standing,*show_floor_areas,*show_map;
	check_extend *snap_to_point, *transparent_walls;


	button_ex* point_in;
	button_ex* location_label_in;

	button_extend* clear_selected_location;
	button_extend* clear_last_location;

	button_extend* clear_locations;


	undo_blocked_Fl_Int_Input* overwrite_location_point;
	undo_blocked_Fl_Input* location_label;

	undo_blocked_Fl_Float_Input *zfloor;
	Fl_Box* zfloor_label;
	button_ex* zfloor_ok;

	counter_extend*  point_size;
	Fl_Counter*  line_thickness;
	
	double_win_extend* win;
	Fl_Window* win2;
	Fl_Window* about_win;
	Fl_Window* calc_win;
	Fl_Window* mesh_win;
	Fl_Window* quit_win;

	win_extend2* floorplan_win;
	Fl_Window *units_win;
	choice_extend *unit_choice;

	undo_blocked_Fl_Float_Input* mesh_density;


	check_extend* use_stand_walls_opaque,*use_stand_walls_transparent,*use_sit_walls_opaque,*use_sit_walls_transparent;
	check_extend* detect_boundaries;
	choice_extend* movement_vis_choice;
	choice_extend *lattice_choice;

	check_extend *edges_button,*turns_button;
	check_extend *force_create_button,*force_visibility_button;


	undo_blocked_Fl_Float_Input* xshift;
	undo_blocked_Fl_Float_Input* yshift;

	button_ex* xshift_ok;
	button_ex* yshift_ok;

	button_extend *load_dxf_button, *clear_dxf_button;

	undo_blocked_Fl_Float_Input* scale_floorplan;
	button_extend* scale_floorplan_ok;

	Fl_Window *guide;
	win_extend* custom_metric_calc;

	std::thread test_thread,test_thread2;
	std::mutex test_mutex,test_mutex2;


	clock_t start_time;   //non-boost

	Fl_Text_Buffer* buff;
    Fl_Text_Buffer* data_buff;
   
    cursor_text *out_read;
    reporting_text_display* out_data;
    
    container root; // main access structure


    button_extend* desk_metric_button;
    
    button_extend* save;
    
    
 	private:
    int menu_height;
    Fl_Box* splash;
    Fl_Group* ooo;
    tabs_extend* oo;
    Fl_Group *o1,*o2,*o3;
    Fl_Group *p1,*p2,*p3;
    

    Fl_Text_Display *out_guide;
    
   
    Fl_Float_Input *x_vert, *y_vert, *x_loc, *y_loc;
    choice_extend* loc_choice;
    Fl_Int_Input *remove_vert, *loc_remove_index;
    std::string bundle_address;
    check_extend* limit_mesh_edge;
    Fl_Float_Input* mesh_edge_max;

    

    #include "callbacks.hpp" // big list of  functions and class prototypes - clearly bad style but...
    
    

public:

	menu_extend* menu;

	draw_window *draw, *fs_draw;
    plot_window* plot;
	Fl_Float_Input* turn_ang;
	button_ex* turn_ang_ok;
	text_output* text;
	Fl_Box* warning_visibility;
	Fl_Box* warning_map;
	Fl_Box *calc_message;

	std::string output_string;
	int buffer_point;
	std::mutex output_mutex, points_mutex, adj_mutex, locations_mutex, walls_mutex, areas_mutex,floors_mutex,plans_mutex,metrics_mutex;
	std::mutex map_mutex;

	int finished_console_output;
	progress calc_progress;
	double old_calc_progress,old_time_left;
	int continue_console_output;
	boost::posix_time::ptime calc_start_time,last_calc_time;
	std::deque<double>  progress_history;
	std::deque<boost::posix_time::ptime>  time_history;

	/*lock and unlock in this order to prevent dead locks
	
	Fl::lock();
	points_mutex.lock();
    adj_mutex.lock();
    walls_mutex.lock();
    locations_mutex.lock();
    areas_mutex.lock();
    floors_mutex.lock();
    plans_mutex.lock();
    output_mutex.lock();
    metrics_mutex.lock();

	
	do stuff
	
	metrics_mutex.unlock();
	output_mutex.unlock();
	plans_mutex.unlock();
    floors_mutex.unlock();
	areas_mutex.unlock();
    locations_mutex.unlock();
    walls_mutex.unlock();
    adj_mutex.unlock();
    points_mutex.unlock();
    Fl::unlock();

	*/


	// map_mutex should only be used for cache writing between worker threads and never interact with the other mutexes

	base();

	void build_and_run();


	~base(){ //not really required as will only reach here on program close where the OS will release everything anyway
		delete win;

		delete out_guide;
		delete win2;
		delete splash;
		delete text;

		delete buff;
    	delete data_buff;
    
    	delete out_read;
	    delete out_data;
	    
	    
	    delete save;

	    delete route_display;
		delete memento_depth_win;

		delete data_text;



		delete  about_win;
		delete  mesh_win;
		delete  quit_win;

		delete  xshift;
		delete  yshift;

		delete mesh_density;
		
		delete detect_boundaries;
		

		delete  scale_floorplan;
		delete  scale_floorplan_ok;

		delete guide;
		
		delete custom_metric_calc;


	};

};


struct packaged_base_ptr{
	base *ptr;
	int option;
};

#endif //BASE_H


