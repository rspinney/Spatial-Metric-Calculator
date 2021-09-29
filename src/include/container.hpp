/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef CONTAINER_H
#define CONTAINER_H

#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <ctime>
#include <math.h>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <atomic>
#include <stdlib.h>

#include <FL/gl.h>

#include "preprocessor.hpp"
#include "building.hpp"
#include "text.hpp"
#include "enums.hpp"
#include "thread_utils.hpp"

#undef min
#undef max

class draw_window;
class plot_window;
class base;

class container{
    base* program;

    public:

    int w,h;
  
    building* current;
    building* current_data;
  
    std::vector<GLfloat> screen_cap;


    THREAD_STATE calculating;
    int undo_test;
    int draw_floorplan,draw_grid,draw_points_label,draw_segments_label,draw_locations_label,snap_to_point,transparent_walls,color_scale,show_scale,draw_route,draw_metric,draw_map,mark_transparent_walls,autoconnect,desk_model,limit_edge,floor_lock,floor_ceil,show_rot_axis;
    int hide_info_bar,hide_side_bar,fullscreen,resized,show_floor_win,graphic_cursor,cursor;
    int capture_screen,screen_w,screen_h;
    std::vector<building*> comp_buildings;
  
    int bow;

    kill_switch killthread;
    

    container(){};
    void re_container(base* program_){

        program = program_;
        
        current = NULL;
        current_data = NULL;
        

        calculating=FREE;
        undo_test=0;
        killthread = kill_switch();
        w=0;h=0;
        draw_floorplan=0;draw_grid=0;draw_points_label=0;draw_segments_label=0;snap_to_point=0;transparent_walls=0;color_scale=0;show_scale=0;
        draw_route=0;draw_metric=0;draw_map=0,desk_model=0,limit_edge=0,floor_lock=1,floor_ceil=1,show_rot_axis=0;
        mark_transparent_walls=0;hide_info_bar=0;hide_side_bar=0;show_floor_win=0;draw_locations_label=0;graphic_cursor=0;
        bow=0;
        capture_screen = 0;
        fullscreen=0;
        resized=0;
        cursor = 1;
        
        for (int i=0;i<NUM_BUILDINGS;i++){            
            building* temp = NULL;
            comp_buildings.push_back(temp);
            comp_buildings[i] = new building(program);            
        }        

        current =comp_buildings[0];
        current_data =comp_buildings[0];

        
    }
    ~container(){
        
        for (std::size_t i=0;i<comp_buildings.size();i++){
            delete comp_buildings[i];
        } 
        
    }
};


#endif //CONTAINER_H

