/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H


#if __APPLE__
	#define sysoffset 20
	#define SYS_ZOOM 1.0
#else
	#define sysoffset 0
	#define SYS_ZOOM 3.0
#endif

#define THEME 0 //set theme - 0: PLASTIC or 1: GTK

#if THEME == 0 
	#define BUTTON_R 155
	#define BUTTON_G 155
	#define BUTTON_B 170
	
	#define MENU_R 237
	#define MENU_G 237
	#define MENU_B 237
#elif THEME == 1
	#define BUTTON_R 215
	#define BUTTON_G 215
	#define BUTTON_B 217

	#define MENU_R 60
	#define MENU_G 60
	#define MENU_B 60
#endif

#define DEFAULT_MEMENTO_DEPTH 15//max number of frames in undo/redo stack on startup
#define NUM_BUILDINGS 4 //number of building slots
#define EPSILON 1e-6 //floating point equality
#define MIN_THREAD 2  //minimum number of threads in multi-threaded computations
#define MAX_DISPLAY_LINKS 1000000 //when selected, maximum number of edges to draw
#define LOCATION_SIZE 0.4 //size of location marker cube  in metres
#define LOCATION_SIZE_METRIC 0.6 //size of location marker cube when illustrated metric in metres
#define MAX_PLAN_LINES 2000000 //2500000  //maximum number of lines in floorplan before preprocessing into lines is abandoned.
#define LINE_DENSITY 100.0  //nunmber of lines per metre in preprocessing of floorplan
#define LINE_MINIMUM 10  //minimum nunmber of lines in a non-trivial floorplan object

//index of menu items
#define DESKMODEL_INDEX 60
#define FLOOR_LOCK_INDEX 61
#define FLOOR_CEIL_INDEX 62
#define FLOORPLAN_INDEX 68
#define FLOORPLAN_COLORS_INDEX 69
#define SHOW_ROUTE_INDEX 72
#define SHOW_METRIC_INDEX 73
#define LIMIT_EDGE_INDEX 78
#define MOVE_MODE_INDEX 81

#endif //PREPROCESSOR_H