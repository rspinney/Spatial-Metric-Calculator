/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/


#include "./include/draw_window.hpp"
#include "./include/base.hpp"

#ifndef M_PI
    #define M_PI 3.141592653589793238
#endif

#undef min
#undef max


//msvc compliance
const GLfloat draw_window::cursor1[15*3*3] = {0,0,0, 0,0,0, 0,0,0,    0,0,0, 1,1,1, 0,0,0,    0,0,0, 1,1,1, 0,0,0,    0,0,0, 1,1,1, 0,0,0,    0,0,0, 1,1,1, 0,0,0,    0,0,0, 1,1,1, 0,0,0,    0,0,0, 1,1,1, 0,0,0,    1,1,1, 1,1,1, 1,1,1,    0,0,0, 1,1,1, 0,0,0,    0,0,0, 1,1,1, 0,0,0,    0,0,0, 1,1,1, 0,0,0,    0,0,0, 1,1,1, 0,0,0,    0,0,0, 1,1,1, 0,0,0,    0,0,0, 1,1,1, 0,0,0,    0,0,0,  0,0,0,  0,0,0};
const GLfloat draw_window::cursor2[15*3*3] = {0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   1,1,1, 0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0,   0,0,0, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 1,1,1, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 1,1,1,  0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0};

const int draw_window::locR[8] = {200,100,100,200,100,200,190,130};
const int draw_window::locG[8] = {100,200,100,200,200,100,150,130};
const int draw_window::locB[8] = {100,100,200,100,200,200,170,0};

draw_window::draw_window(int X, int Y, int W, int H, const char *L, container* root_,base* program_): Fl_Gl_Window(X, Y, W, H, L),mark_locations_choice(program_->mark_locations_choice){//CTOR

    group_select = 0;
    pressed_mouse = -1;
    x_mouse = 0;
    y_mouse = 0;

    map_color[0]=51+5;map_color[1]=102+5;map_color[2]=201+5;

    plan_color = false;
    use_splines = true;

    axx=0;azz=0;frac_x=0;frac_y=0;
    h_initial=0;w_initial=0;
    xdown=0;ydown=0;
    xdragdown=0;ydragdown=0;
    xdragdown_old=0;ydragdown_old=0;
    minx_down=0;maxx_down=0;
    miny_down=0;maxy_down=0;
    midx=0;midy=0;midz=0;
    cv[0]=0;cv[1]=0;cv[2]=0;
    ov[0]=0;ov[1]=0;ov[2]=0;
    snapx=0;snapy=0;
    xh=0;yh=0;zh=0;
    xh_offset=0;yh_offset=0;zh_offset=0;
    xh_offset_cp=0;yh_offset_cp=0;zh_offset_cp=0;
    z_dist=0;
    metric_type=0;
    pathPtr=NULL;
    flr_choice=0;
    all_flrs=0;
    pixels = NULL;
    texturefilename = NULL;
    
    loop_select = 0;

    Fl_Gl_Window::mode(FL_RGB|FL_DOUBLE|FL_DEPTH);
    
    root=root_;
    program=program_;
    current = root->current;

    was_dragging=0;    

    x_down=0;

    pressed_direction = -1;
    last_pressed_direction = -1;
    
    xfr=0;yfr=0;zfr=0;
    
    loaded_textures=0;
    
    load_ceiling=0;
    load_wall=0;
    load_floor=0;
    
    set_first=0;
    shift_on=0;alt_on=0;
    first_draw=1;
    zoom=0;
    drag=0;
    zoom_mag=0;
    zoom_level=1.0;
    ang_x=0;ang_y=0;ang_z=0;
    
    ang=0;ang_old=0;
    rot[0]=0;rot[1]=0;rot[2]=0;
    old_rot[0]=0;old_rot[1]=0;old_rot[2]=0;
    min_zoom=0.2;
    max_zoom=100/0.01;
    
    image_x=-1.25;//-101;
    image_y=-6.5;//-74;
    image_width=136;
    image_height=36;

    z_gap=0.5;
    selector=-1;
    selected=-1;
    snapon=0;
    snap_point=-1;
    xfp=0;yfp=0;zfp=1.8;
    xfp_move=0;
    yfp_move=0;
    zfp_move=0;
    ang_xfp=-90;
    ang_zfp=0;
    step_length=0.05;
    new_zoom=0;
    start_link=-1;
    end_link=-1;
    selected_matrix[0]=-1;
    selected_matrix[1]=-1;
    minx_overall=-52;
    maxx_overall=52;
    miny_overall=-52;
    maxy_overall=52;
    minx=-52;
    maxx=52;
    miny=-52;
    maxy=52;
    minz=0;
    current->mid_x=0;
    current->mid_y=0;
    current->mid_z=0;
    drag_point=-1;
    ang_snap=0;
    count=0;
    
    idle_on=0;

    convert=2.0*M_PI*0.0027777777777777777777777777; // 2pi/360

    last_press_time = boost::posix_time::microsec_clock::local_time();
    last_release_time = boost::posix_time::microsec_clock::local_time();
    last_step_time = boost::posix_time::microsec_clock::local_time();

}


//These need to be folded into base class at some point
void draw_window::show_warning_visibility(){
        
    if (root->current->vis_valid){
        program->push_time();
        program->push_output("WARNING: Visibility constructs may have changed.\n------------------- >> Displaying/using visibility in metrics will incur additional computation steps.\n");
    }

    root->current->vis_valid=0;
    program->show_visibility->value(0);    
    if (!program->calc_message->visible()){
        program->warning_visibility->show();
    }
    program->warning_visibility->redraw();
    program->warning_visibility->parent()->redraw();
    program->win->redraw();


    program->metrics_mutex.lock();

    program->root.current->plot_metric.clear();
    root->current->max_plot_metric = 0.0;
    root->current->min_plot_metric = 0.0;

    program->metrics_mutex.unlock();
    
}

void draw_window::show_warning_map(){
    
    
    
    if ((root->current->map_valid)||(root->current->lines_valid)){
    
        program->push_time();    
        program->push_output("WARNING: Spatial graph may have changed.\n------------------- >> Subsequent route/metric calculation will incur additional computation steps.\n");
        
    }

    root->current->map_valid=0;
    root->current->lines_valid=0;
    root->current->segs_valid=0;
    root->current->turns_valid=0;
   
    if (!program->calc_message->visible()){
        program->warning_map->show();
    }
    program->warning_map->redraw();
    program->warning_map->parent()->redraw();
    program->win->redraw();

    program->metrics_mutex.lock();

    program->root.current->plot_metric.clear();
    root->current->max_plot_metric = 0.0;
    root->current->min_plot_metric = 0.0;
    program->root.current->route.clear();

    program->metrics_mutex.unlock();

    this->redraw();

}



void draw_window::draw() { 

    int do_capture=0;
    if(root->capture_screen) do_capture = 1;

    if ((minx>maxx)||(miny>maxy)){ //if either of these something has gone horribly wrong: start again.
        first_draw=1;
        this->redraw();
        return;
    }    
    
    every_frame_calc_alt();
    
    if (!valid()){        
        load_textures();
    }
    
    determine_floor();
    
    if (!do_capture){
        glViewport(0,0,w(),h());
    }
    else{
        glViewport(0,0,w(),h());
    }

    glLoadIdentity();
    
    if (root->bow){
        glClearColor(0.98,0.98,0.98,0);//background colour
    }
    else{
        glClearColor(0.22,0.22,0.22,0);
    }
        
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    double back_z = root->current->minimum_z-10.0-(maxx_overall-minx_overall)/min_zoom;
    double fore_z = root->current->maximum_z+10.0+(maxx_overall-minx_overall)/min_zoom;
        
    double dif = fore_z-back_z;
    double back_z_ext = back_z - 10*dif;
    double fore_z_ext = fore_z + 10*dif;        
    
    //SET UP PROJECTION
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    projection(back_z_ext,fore_z_ext);
        
    //START DRAWING THINGS
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    translate_frustum();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    
    double zval = minz;

    if (all_flrs){
        zval = 0.5*(root->current->maximum_z+root->current->minimum_z); //rotation point for z
    }

    set_rotation(zval);
    
    lighting();    
   
    glEnable(GL_DEPTH_TEST);

    glDepthMask(GL_TRUE);

    if (root->draw_grid){
        draw_background(back_z);   
    }
    
    draw_rot_axis(back_z,fore_z);

    glColor3ub(255,255,255);
    if (root->floor_ceil){
        draw_floor();
        draw_ceiling();
    }

    //capture mouse position
    
    if (program->three_d_choice->value()<3){
        program->plans_mutex.lock();
        draw_floorplan_gen(back_z+std::max(10.0,0.001*fabs(maxx_overall-minx_overall)));
        program->plans_mutex.unlock();
    }
    else if (!root->floor_ceil){
        program->plans_mutex.lock();
        draw_floorplan_gen(back_z+std::max(10.0,0.002*fabs(maxx_overall-minx_overall)));
        program->plans_mutex.unlock();
    }
            

    if (program->show_floor_areas->value()){
        program->areas_mutex.lock();
        draw_areas(back_z+std::max(0.01,0.003*fabs(maxx_overall-minx_overall)));
        program->areas_mutex.unlock();
    }
  
    metric_type=0;

    program->metrics_mutex.lock();
    if ((root->draw_metric)&&(current->plot_metric.size()>0)){
        metric_type = root->current->plot_metric_des.basis+1;
    }
    program->metrics_mutex.unlock();
    
    Fl::get_mouse(x_mouse,y_mouse);
    x_mouse_coord = (static_cast<double>(x_mouse-program->win->x()-this->x())/(  static_cast<double>(this->w())   ))*(maxx-minx)+minx;
    y_mouse_coord = (static_cast<double>(y_mouse-program->win->y()-this->y())/(  static_cast<double>(this->h())   ))*(miny-maxy)+maxy;

    //x_mouse_coord=(static_cast<double>(Fl::event_x())/static_cast<double>(this->w()))*(maxx-minx)+minx;
    //y_mouse_coord=(static_cast<double>(Fl::event_y())/static_cast<double>(this->h()))*(miny-maxy)+maxy;

    if (program->show_map->value()){

        program->points_mutex.lock();
        program->adj_mutex.lock();
        program->metrics_mutex.lock();

        drawaxialmap(current->plot_metric,metric_type);
        showpoints();
        showsegments();

        program->metrics_mutex.unlock();
        program->adj_mutex.unlock();
        program->points_mutex.unlock();

    }

    if (root->draw_route){
        drawroute();
    }
        
    program->locations_mutex.lock(); 
    program->metrics_mutex.lock();

    draw_locations(current->plot_metric,metric_type);

    program->metrics_mutex.unlock();
    showlocations();
    draw_visibility();
    program->locations_mutex.unlock();

    program->walls_mutex.lock();        
    draw_walls(current->walls_sitting,current->walls_standing);
    program->walls_mutex.unlock();
    
    if (!program->three_d_choice->value()){
        if (group_select){
            draw_group_select(fore_z);
        }
    }

    glPopMatrix();
    
    gl_color(FL_GRAY);
    glEnable(GL_DEPTH_TEST);
                        
    reset_perspective(back_z_ext,fore_z_ext);
            
    showrotatinglegend(fore_z);
    showscale(fore_z);
    show_FOV(fore_z);
    showcolorscale(fore_z);
    
    showcoords(fore_z);

    glEnable(GL_DEPTH_TEST);
    if ((!do_capture)&&(!snapon)) drawcursor(fore_z_ext);
            
    if (do_capture) capture_screen();

    
    if (first_draw){
        first_draw=0; //finished first draw
    }

}

