/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef DRAW_WINDOW_H
#define DRAW_WINDOW_H


#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <ctime>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/glut.H>

#define BOOST_DATE_TIME_NO_LIB
#include <boost/date_time/posix_time/posix_time.hpp>

#include "container.hpp"
#include "text.hpp"
#include "fltk_extend.hpp"
#include "colors.hpp"

#undef min
#undef max

class base;

class draw_window : public Fl_Gl_Window {
    
    //MSVC compliance
    static const GLfloat cursor1[15*3*3];
    static const GLfloat cursor2[15*3*3];

    // Fl_RGB_Image *cursorA;
    // Fl_Pixmap *pxm;
    // const char **xpm;

    //rnd_mt rand; // in case we need pseudo random numbers
    int loop_select;

    int old_plan_color;
    bool plan_color;
    int x_mouse, y_mouse;
    double x_mouse_coord,y_mouse_coord;

    public:
    void do_plan_colors(int val){
        if (val){
            plan_color = true;
        }
        else{
            plan_color = false;
        }
    }

    int get_plan_colors(){
        return plan_color;
    }

    bool use_splines;

    public:
    void do_splines(int val){
        if (val){
            use_splines = true;
        }
        else{
            use_splines = false;
        }
    }

    void capture_screen();
    
    private:
    //methods

    void move_time_check();
    void detect_movement_presses(const int& );
    void draw_rot_axis(const double&,const double&);
    void projection(const double&,const double&);
    void determine_floor();
    void lighting();
    void reset_perspective(const double&,const double&);
    
    inline void set_color(int index,int block_color){

    if ((index==0)&&(block_color > 0)&&(block_color<256)){
        index = block_color;
    }
    else if ((index<0)||(index>255)){
        index = 0;
    }
    if (old_plan_color!=index){
        glColor3ub(color_DXF[index][0],color_DXF[index][1],color_DXF[index][2]);
        old_plan_color = index;
    }
}

    void draw();
    int handle(int);
    void drawcube(double,double,double,double);
    void drawcube_no_begin_end(double,double,double,double);
    void drawcube(double,double,double,double,GLuint);
    void drawcubeedge(double,double,double,double,double,int);
    void drawcubeedge_no_begin_end(double,double,double,double,int);
    void showpoints();
    void showsegments();
    void showlocations();
    void showscale(double);
    void showcolorscale(double);
    void showrotatinglegend(double);
    void drawaxialmap(const std::vector<double>&,int);
    void drawroute();
    void drawpoints(const std::vector<double>&);
    void draw_background(double);
    void set_rotation(double);
    void setzoom();
    void draw_desks_model(int);
    void draw_locations(const std::vector<double>&,int);//all the overloading....
    void draw_locations(const std::vector<location>*,int,int,int,int);
    void draw_location_links(const std::vector<location>*,int,int,int);
    void draw_locations(const std::vector<location>*,const std::vector<double>*,double,double,int);
    void draw_walls(const std::vector<wall>&,const std::vector<wall>&);
    void draw_visibility();
    void drawpyramid(double , double , double , double);
    void showcoords(double);
    void drawcursor(double);
    void draw_floorplan(double);
    void draw_floorplan_fast(double);
    void draw_floorplan_floor(double,int);
    void draw_floorplan_floor_fast(double,int);
    void draw_floorplan_gen(double);
    
    void draw_block_nested(const plan *const,const block_entry&,const double&,int,double&, double&);//nested only
    void DrawCircle(const circle&,const double&,const double&,const double&,const int);
    void DrawEllipse(const ellipse&,const double&,const double&,const double&,const int);
    void DrawArc(const arc&, const double&,const double&,const double&,const int);
    void DrawLines(const line_in&,const double&,const double&,const double&,const int); //nested only
    void DrawSpline(const spline&,const double&,const double&,const double&,const int); //nested only


    void find_plan_snap_point(const plan &,double &,const double&,const double&,const double&,bool);
    void find_plan_snap_point_spline(const spline &,double &,const double&,const double&,const double&,const matrix_2d &,const double&,const double&,const double &,const double &,bool);
    void find_plan_snap_point_line(const line_in &,double &,const double&,const double&,const double&,const matrix_2d &,const double&,const double&,const double &,const double &,bool);
    void find_plan_snap_point_circle(const circle &,double &,const double&,const double&,const double&,const matrix_2d &,const double&,const double&,const double &,const double &,bool);
    void find_plan_snap_point_arc(const arc &,double &,const double&,const double&,const double&,const matrix_2d &,const double&,const double&,const double &,const double &,bool);
    void find_plan_snap_point_ellipse(const ellipse &,double &,const double&,const double&,const double&,const matrix_2d &,const double&,const double&,const double &,const double &,bool);
    void find_plan_snap_point_block(const block_entry &,const plan &,double &,const double&,const double&,const double&,const matrix_2d &,const double&,const double&,const double &,const double &,bool);

    void find_plan_snap_point_fast(const plan &,double &,const double&,const double&,const double&,bool);
    void find_plan_snap_point_line_fast(const floor_line &,double &,const double&,const double&,const double&,bool);

    void draw_snapon(double);
    void test_snapon();
    void show_FOV(double);
    void draw_areas(double);
public:
    void ear_clipping(std::vector<vec>,std::vector<triangle>*);
    void find_selection_objects(int);
private:
    int intersect_test(double,double,double,double, double,double,double,double);
    int test_in_triangle(double, double,triangle);
    void every_frame_calc_alt();
    
    void translate_frustum();
    
    void draw_floor();
    void draw_ceiling();

    void draw_group_select(double);
    
    bool point_in_polygon(double, double,const std::vector<vec>&);
    int pnpoly(double, double,const std::vector<vec>&);
    
    void mouseless_zoom(int);

    void find_snap_point();

    int map_color[3];    

    //MSVC compliance
    static const int locR[8];
    static const int locG[8];
    static const int locB[8];
    
    int was_dragging;
    int pressed_mouse;
    int group_select;

    int idle_on;
    int pressed_direction,last_pressed_direction;
    
    boost::posix_time::ptime move_time,last_step_time;
    boost::posix_time::ptime move_start_time,old_move_start_time,last_press_time,last_release_time;
    
    double xfr,yfr,zfr,axx,azz;
    double convert;
    
    //data
    double frac_x,frac_y;
    
    double minx_overall,maxx_overall,miny_overall,maxy_overall;
    int h_initial,w_initial;
    double zoom_level;
    int drag;
    int zoom_mag;
    double xdown,ydown,xdragdown,ydragdown,xdragdown_old,ydragdown_old;
    double minx_down,maxx_down,miny_down,maxy_down;
    
    double midx,midy,midz;
    double ang,ang_old,rot[3],old_rot[3],cv[3],ov[3];
    double min_zoom,max_zoom;
    double image_x,image_y;
    double image_width,image_height;
    int snapon,snap_point,snap_point_plus,snap_wall;
    double snapx,snapy;
    
    
    double xfp_move,yfp_move,zfp_move;
    double xh,yh,zh,xh_offset,yh_offset,zh_offset,xh_offset_cp,yh_offset_cp,zh_offset_cp;
    double z_dist;
    double step_length;
    

    int metric_type;
    int count;
    int shift_on;
    int alt_on;
    
    wall temp_wall;
    
    const char *pathPtr;
    std::string bundle_address;
    
    int load_wall,load_ceiling,load_floor;
    
    int x_down;
    
    //GLuint textureObject;
    
    
    GLuint wall_texture;
    GLuint floor_texture;
    GLuint ceiling_texture;
    
    container* root;
    base* program;
    
    choice_extend*& mark_locations_choice;
    
    //END REFERENCES TO ACCESS MAIN DATA
    
public:
    
    static void idle_draw(void* data);
    
    building* current;
    
    double minx,maxx,miny,maxy,minz;
    double ang_x,ang_y,ang_z,ang_xfp,ang_zfp;
    double xfp,yfp,zfp;
    int first_draw;
    double z_gap;
    int flr_choice;
    int all_flrs;
    int selector;
    int selected;
    int selected_matrix[2];
    double new_zoom;
    int zoom;
    int start_link,end_link;
    int drag_point;
    int set_first;
    double ang_snap;
    
    std::vector<int> group_selection_list;

    int loaded_textures;
    
    GLubyte* pixels;  //for image export

    std::vector<std::vector<triangle> > floorarea_triangles;
    
    void deselect();
    void deselect_ex_floor();
    void calculate_areas(std::vector<std::vector<vec> >* polygons);
    void load_textures();

    void set_x_down();

    int is_x_down(){
        return x_down;
    }

    void snapoff(){snapon = 0;}
    void set_x_down(int);
    
    
    draw_window(int , int , int , int , const char *, container*,base*);
    
    void set_x_down_off(){x_down=0;}

    static void mouseless_zoom_in_entry(Fl_Widget*, void*);
    static void mouseless_zoom_out_entry(Fl_Widget*, void*);
    
    //void show_warning_visibility();  
    //void show_warning_map();
        
    int is_valid(){
        if (valid()){
            return 1;
        }
        else{
            return 0;
        }
    }
        
    GLuint TexID;
    const char *texturefilename;
    
    void TextureFilename(const char *val) {
        texturefilename = val;
    }
    const char *TextureFilename() const {
        return(texturefilename);
    }
    
    
    
};



#endif /*DRAW_WINDOW_H*/

