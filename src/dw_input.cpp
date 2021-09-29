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

void draw_window::mouseless_zoom(int direction){
 
    this->frac_x = 0.5;
    this->frac_y = 0.5;
    this->zoom_mag=2*direction;
    this->zoom=1;
    this->redraw();
}


void draw_window::mouseless_zoom_out_entry(Fl_Widget*, void* data){
    
    base* program =static_cast<base*>(data);
    program->draw->mouseless_zoom(1);
}

void draw_window::mouseless_zoom_in_entry(Fl_Widget*, void* data){
    
    base* program =static_cast<base*>(data);
    program->draw->mouseless_zoom(-1);
}

void draw_window::deselect_ex_floor(){
    
    group_selection_list.clear();

    selected=-1;
    selector=-1;
    drag_point=-1;
    selected_matrix[0]=-1;
    selected_matrix[1]=-1;
    end_link=-1;
    set_first=0;
    program->soft_reset_fields();
}
    
void draw_window::deselect(){
    
    group_selection_list.clear();

    selected=-1;
    selector=-1;
    drag_point=-1;
    selected_matrix[0]=-1;
    selected_matrix[1]=-1;
    start_link=-1;
    end_link=-1;
    set_first=0;
    program->soft_reset_fields();
    
}


void draw_window::find_selection_objects(int select_all){


    deselect();

    program->metrics_mutex.lock();
    program->root.current->plot_metric.clear();
    program->root.current->route.clear();
    program->metrics_mutex.unlock();


    group_selection_list.clear();

    double x_mouse_coord2 = (static_cast<double>(xdown)  /  static_cast<double> (this->w()) )*(maxx-minx)+minx;
    double y_mouse_coord2 = (static_cast<double>(ydown)  /  static_cast<double> (this->h()) )*(miny-maxy)+maxy;

    double xmin = std::min(x_mouse_coord,x_mouse_coord2);
    double xmax = std::max(x_mouse_coord,x_mouse_coord2);

    double ymin = std::min(y_mouse_coord,y_mouse_coord2);
    double ymax = std::max(y_mouse_coord,y_mouse_coord2);

    int c = program->mark_locations_choice->value();
    int f = program->floor_choice->value()-1;

    if ((c>0)&&(c<10)){

        program->locations_mutex.lock();
        for (unsigned int i=0; i < current->locations[c-1].size(); i++){

            location &p = current->locations[c-1][i];

            if (p.loc.floor == f){
                if (select_all){
                    group_selection_list.push_back(i);
                }
                else if ((p.loc.x>xmin)&&(p.loc.x<xmax)&&(p.loc.y>ymin)&&(p.loc.y<ymax)){
                    group_selection_list.push_back(i);
                }
            }
        }
        program->locations_mutex.unlock();

        if (group_selection_list.size()){
            selector=c;
        }
        else{
            selector=-1;
        }
    }
    else if (c==12){
        
        program->points_mutex.lock();
        for (unsigned int i=0; i < current->points.size(); i++){

            vec &p = current->points[i];

            if (p.floor == f){
                if (select_all){
                    group_selection_list.push_back(i);
                }
                else if ((p.x>xmin)&&(p.x<xmax)&&(p.y>ymin)&&(p.y<ymax)){
                    group_selection_list.push_back(i);
                }
            }
        }
        program->points_mutex.unlock();

        if (group_selection_list.size()){
            selector=12;
        }
        else{
            selector=-1;
        }
    }
    else if (c==13){
        
        program->adj_mutex.lock();
        for (unsigned int i=0; i < current->adj_list.size(); i++){

            vertex &p = current->adj_list[i];

            vec &p1 = current->points[p.start];
            vec &p2 = current->points[p.end];

            if ((p1.floor == f)&&(p2.floor == f)){
                if (select_all){
                    group_selection_list.push_back(i);
                }
                else if ((p1.x>xmin)&&(p1.x<xmax)&&(p1.y>ymin)&&(p1.y<ymax)){
                    if ((p2.x>xmin)&&(p2.x<xmax)&&(p2.y>ymin)&&(p2.y<ymax)){
                        group_selection_list.push_back(i);
                    }
                }
            }
        }
        program->adj_mutex.unlock();

        if (group_selection_list.size()){
            if ((root->limit_edge)&&(current->adj_list.size()>MAX_DISPLAY_LINKS)){
                program->push_time();
                program->push_output("Warning: edge visualisation is limited - not all selected edges shown.\n");
            }
            selector=13;
        }
        else{
            selector=-1;
        }
    }
    else if (c==10){
        
        program->walls_mutex.lock();
        for (unsigned int i=0; i < current->walls_sitting.size(); i++){

            wall &p = current->walls_sitting[i];

            vec &p1 = p.start;
            vec &p2 = p.end;

            if (select_all){
                group_selection_list.push_back(i);
            }
            else if ((p1.floor == f)&&(p2.floor == f)){

                if ((p1.x>xmin)&&(p1.x<xmax)&&(p1.y>ymin)&&(p1.y<ymax)){ //if at least one end of the barrier is contained
                    group_selection_list.push_back(i);
                }
                else if ((p2.x>xmin)&&(p2.x<xmax)&&(p2.y>ymin)&&(p2.y<ymax)){ 
                    group_selection_list.push_back(i);
                }
            }
        }
        program->walls_mutex.unlock();

        if (group_selection_list.size()){
            selector=10;
        }
        else{
            selector=-1;
        }
    }
    else if (c==11){
        
        program->walls_mutex.lock();
        for (unsigned int i=0; i < current->walls_standing.size(); i++){

            wall &p = current->walls_standing[i];

            vec &p1 = p.start;
            vec &p2 = p.end;

            if ((p1.floor == f)&&(p2.floor == f)){

                if (select_all){
                    group_selection_list.push_back(i);
                }
                else if ((p1.x>xmin)&&(p1.x<xmax)&&(p1.y>ymin)&&(p1.y<ymax)){ //if at least one end of the barrier is contained
                    group_selection_list.push_back(i);
                }
                else if ((p2.x>xmin)&&(p2.x<xmax)&&(p2.y>ymin)&&(p2.y<ymax)){ 
                    group_selection_list.push_back(i);
                }
            }
        }
        program->walls_mutex.unlock();

        if (group_selection_list.size()){
            selector=11;
        }
        else{
            selector=-1;
        }
    }
    else if (c==14){
        
        program->areas_mutex.lock();
        for (unsigned int i=0; i < current->floorarea_polygons.size()-1; i++){

            std::vector<vec> &p = current->floorarea_polygons[i];

            int count = 0;

            if (p[0].floor == f){

                if (select_all){
                    count++;
                }
                else{
                    for (unsigned int j=0; j<p.size(); j++){

                        vec &q = p[j];

                        if ((q.x>xmin)&&(q.x<xmax)&&(q.y>ymin)&&(q.y<ymax)){
                            count++;
                        }
                    }
                }

            }
            
            if (count){
                group_selection_list.push_back(i);
            }

        }
        program->areas_mutex.unlock();

        if (group_selection_list.size()){
            selector=14;
        }
        else{
            selector=-1;
        }
    }

    this->redraw();
    
}

void draw_window::set_x_down(){

    if(this->x_down){
        this->x_down=0;
    }
    else{
        this->x_down=1;
    }
    program->push_move_status(this->x_down);
}

void draw_window::set_x_down(int set){
    if (set!=this->x_down){
        program->push_move_status(this->x_down);
    }
    this->x_down=set;
    
}

void draw_window::idle_draw(void* data){
    
    base* program = static_cast<base*>(data);
    
    if  (program->three_d_choice->value()>1){
        program->draw->redraw();
    }
}


void draw_window::every_frame_calc_alt(){
    

    if(program->three_d_choice->value()==3){ //from handle e==12 swithc statemtn
    

        if (pressed_direction>-1){//W,A,S,D,R,F
            
            
            double time_to_max,max_step_length;
            double step_length_overall;
            
            boost::posix_time::ptime time =boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration dur = time-move_start_time;
            double time_since_press=dur.total_milliseconds()*0.001;
            
            time_to_max=1;
            max_step_length=15.0;

            step_length_overall= (time_since_press<time_to_max) ? (0.0+1.0*time_since_press*(1.0/time_to_max))*max_step_length : max_step_length;

            boost::posix_time::time_duration dur_step = time - last_step_time;

            step_length=step_length_overall*std::min((dur_step.total_milliseconds()*0.001),1.0); //first step length will be large/weird - just dont allow steps bigger than 1fps

            last_step_time=time;
            
            if (step_length>max_step_length)step_length=max_step_length; //just for safety: shouldnt need it with initialisation of move_time
            
            double ang_zz;
    
            ang_zz=ang_zfp*convert;
            
            double zz_z=1.0,xx_z=1.0,yy_z=1.0;
            
            if (root->floor_lock){
                zz_z = 0;
            }
            else{

                double ang_xx;
                ang_xx = ang_xfp*convert;
                double sax = sin(ang_xx);
                zz_z*=-cos(ang_xx); //how much use z is described in terms of model z            
                xx_z*=-sax;
                yy_z*=-sax;
            }

            if (pressed_direction==2){ //pressed left 0,1,2,3,4,5 for,back,left,right,up,down
                ang_zz-=M_PI/2;
                zz_z = 0;
            }
            else if(pressed_direction==3){
                ang_zz+=M_PI/2;
                zz_z = 0;   
            }
            else if(pressed_direction==1){
                ang_zz+=M_PI;   
                zz_z *=  -1.0;
            }
            else if (pressed_direction>3){
                xx_z=0.0;
                yy_z=0.0;
                if (!root->floor_lock){
                    if (pressed_direction==4) zz_z=1.0;
                    if (pressed_direction==5) zz_z=-1.0;
                }
            }

            xx_z *= sin(ang_zz);
            yy_z *= cos(ang_zz);

            xfp+=step_length*xx_z;
            yfp+=step_length*yy_z;
            zfp+=step_length*zz_z;            
            
        }

            
    }
    else if(program->three_d_choice->value()==2){
        
        if (pressed_direction>-1){//W,A,S,D,R,F
            
            
            double time_to_max,max_step_length;
            double step_length_overall;
            
            boost::posix_time::ptime time =boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration dur = time-move_start_time;
            double time_since_press=dur.total_milliseconds()*0.001;
            
            time_to_max=1;
            max_step_length= fabs(maxx_overall-minx_overall);

            step_length_overall= (time_since_press<time_to_max) ? (0.0+1.0*time_since_press*time_since_press*(1.0/(time_to_max*time_to_max)))*max_step_length : max_step_length;

            boost::posix_time::time_duration dur_step = time - last_step_time;

            step_length=step_length_overall*std::min((dur_step.total_milliseconds()*0.001),1.0);

            last_step_time=time;
            
            if (step_length>max_step_length)step_length=max_step_length; 


            if (pressed_direction==0){
                z_dist-=step_length;
                if (z_dist<0){
                    z_dist = 0.0;
                }
            }
            else if(pressed_direction==1){
                z_dist+=step_length;
            }
        }
    }
}


void draw_window::move_time_check(){
    boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration dur  = t - last_press_time;
    boost::posix_time::time_duration dur2 = t - last_release_time;
    last_press_time = t;
    double dif  = dur.total_milliseconds()*0.001;
    double dif2  = dur2.total_milliseconds()*0.001;
    if ((dif>0.1)&&(dif2>0.1)) move_start_time = t;
}



void draw_window::detect_movement_presses(const int& e){

    if ((e==12)&&(program->three_d_choice->value()>1)&&(!Fl::event_ctrl())&&(!alt_on)){
            

        if ((Fl::event_key()=='w')&&(!idle_on)){ 
            Fl::add_idle(idle_draw,program);
            idle_on=1;
            if (last_pressed_direction!=0){
                boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
                move_start_time = t;
                last_press_time = t;
            }
            else{
                move_time_check();  
            }
            move_time_check(); 
            pressed_direction=0;              

        }
        else if ((Fl::event_key()=='w')&&(pressed_direction==0)){ 
            move_time_check();
        }
        else if (Fl::event_key()=='w'){ 
            pressed_direction=0;
            boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
            move_start_time = t;
            last_press_time = t;
        }

        if ((Fl::event_key()=='s')&&(!idle_on)){ 
            Fl::add_idle(idle_draw,program);
            idle_on=1;
            if (last_pressed_direction!=1){
                boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
                move_start_time = t;
                last_press_time = t;
            }
            else{
                move_time_check();  
            }
            pressed_direction=1;              

        }
        else if ((Fl::event_key()=='s')&&(pressed_direction==1)){ 
            move_time_check();
        }
        else if (Fl::event_key()=='s'){ 
            pressed_direction=1;
            boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
            move_start_time = t;
            last_press_time = t;
        }

        if ((Fl::event_key()=='a')&&(!idle_on)){ 
            Fl::add_idle(idle_draw,program);
            idle_on=1;
            if (last_pressed_direction!=2){
                boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
                move_start_time = t;
                last_press_time = t;
            }
            else{
                move_time_check();  
            }
            pressed_direction=2; 

        }
        else if ((Fl::event_key()=='a')&&(pressed_direction==2)){ 
            move_time_check();
        }
        else if (Fl::event_key()=='a'){ 
            pressed_direction=2;
            boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
            move_start_time = t;
            last_press_time = t;
        }

        if ((Fl::event_key()=='d')&&(!idle_on)){ 
            Fl::add_idle(idle_draw,program);
            idle_on=1;
            if (last_pressed_direction!=3){
                boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
                move_start_time = t;
                last_press_time = t;
            }
            else{
                move_time_check();  
            }
            pressed_direction=3; 

        }
        else if ((Fl::event_key()=='d')&&(pressed_direction==3)){
            move_time_check();
        }
        else if (Fl::event_key()=='d'){ 
            pressed_direction=3;
            boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
            move_start_time = t;
            last_press_time = t;
        }

        if ((Fl::event_key()=='r')&&(!idle_on)){ 
            Fl::add_idle(idle_draw,program);
            idle_on=1;
            if (last_pressed_direction!=4){
                boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
                move_start_time = t;
                last_press_time = t;
            }
            else{
                move_time_check();  
            }
            pressed_direction=4; 

        }
        else if ((Fl::event_key()=='r')&&(pressed_direction==4)){ 
            move_time_check();
        }
        else if (Fl::event_key()=='r'){ 
            pressed_direction=4;
            boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
            move_start_time = t;
            last_press_time = t;
        }

        if ((Fl::event_key()=='f')&&(!idle_on)){ 
            Fl::add_idle(idle_draw,program);
            idle_on=1;
            if (last_pressed_direction!=5){
                boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
                move_start_time = t;
                last_press_time = t;
            }
            else{
                move_time_check();  
            }
            pressed_direction=5; 

        }
        else if ((Fl::event_key()=='f')&&(pressed_direction==5)){ 
            move_time_check();
        }
        else if (Fl::event_key('f')){ 
            pressed_direction=5;
            boost::posix_time::ptime t = boost::posix_time::microsec_clock::local_time();
            move_start_time = t;
            last_press_time = t;
        }
        
    }
    else if (e==9){

        if (Fl::event_key()=='w'){ 
            pressed_direction=-1;
            last_pressed_direction=0;
            last_release_time = boost::posix_time::microsec_clock::local_time();
            Fl::remove_idle(idle_draw,program);

            idle_on=0;
        }

        if (Fl::event_key()=='s'){ 
            pressed_direction=-1;
            last_pressed_direction=1;
            last_release_time = boost::posix_time::microsec_clock::local_time();
            Fl::remove_idle(idle_draw,program);
            idle_on=0;
        }

        if (Fl::event_key()=='a'){ 
            pressed_direction=-1;
            last_pressed_direction=2;
            last_release_time = boost::posix_time::microsec_clock::local_time();
            Fl::remove_idle(idle_draw,program);
            idle_on=0;
        }

        if (Fl::event_key()=='d'){
            pressed_direction=-1;
            last_pressed_direction=3;
            last_release_time = boost::posix_time::microsec_clock::local_time();
            Fl::remove_idle(idle_draw,program);
            idle_on=0;
        }

        if (Fl::event_key()=='r'){ 
            pressed_direction=-1;
            last_pressed_direction=4;
            last_release_time = boost::posix_time::microsec_clock::local_time();
            Fl::remove_idle(idle_draw,program);
            idle_on=0;
        }

        if (Fl::event_key()=='f'){ 
            pressed_direction=-1;
            last_pressed_direction=5;
            last_release_time = boost::posix_time::microsec_clock::local_time();
            Fl::remove_idle(idle_draw,program);
            idle_on=0;
        }
   }
}


int draw_window::handle(int e) {
    
    int ret = Fl_Gl_Window::handle(e);


    if(snapon){        
        if (program->root.cursor!=0){
            cursor(FL_CURSOR_NONE);
            program->root.cursor = 0;
        }
    }
    else{
        if (root->graphic_cursor){
            if (program->root.cursor!=0){
                cursor(FL_CURSOR_NONE);
                program->root.cursor = 0;
            }
        }
        else{
            if (program->root.cursor!=2){
                if ((e!=FL_LEAVE)&&(e!=FL_NO_EVENT)){
                    cursor(FL_CURSOR_CROSS);
                    program->root.cursor = 2;
                }
            }
        }
    }

    int to_redraw=0;

    if ((e==12)&&(Fl::event_key()==65535)){ //reject delete
        return 0;
    }

    if ((e==12)&&(Fl::event_key()==65288)){ //reject backspace
        return 0;
    }
    
    if ((ret==FL_FOCUS)||(ret==FL_UNFOCUS)){
        return 1;
    }
    
    shift_on=Fl::event_shift();

    alt_on = Fl::event_alt();
    
    
    if (e==12){ // if a button has gone down
    
        if (Fl::event_key()==65513){ //if alt went down
            alt_on=1;
            to_redraw=1;
        }

        
        if (Fl::event_key()==65505){ //if shift went down
            shift_on=1;
            to_redraw=1;
        }
    }

    if (e==9){ // if a button has been released
    
        if (Fl::event_key()==65513){ //if alt
            alt_on=0;
            snap_point=-1;
            to_redraw=1;
        }
        

        if (Fl::event_key()==65505){ //if shift
            shift_on=0;
            to_redraw=1;
        }
    }

    if (!shift_on){
        snap_point=-1;
    }
    
    if (x_down){
        alt_on = 1;
    }

    find_snap_point();
    
    int old_snapon =snapon;
    program->plans_mutex.lock();
    test_snapon();
    program->plans_mutex.unlock();
    //end snap_on function test

    
    if (e==12){ // if a button has gone downn

        if (Fl::event_key()==65513){ //if alt went down
            this->redraw();
            return 1;
        }
        
        if (Fl::event_key()==65505){ //if shift went down
            this->redraw();            
            return 1;
        }
        
        //apparently shift is only counted as a button press on first press, other keys continuously press: who knew
     
        if ((program->three_d_choice->value()==0)&&((selected>-1)||(group_selection_list.size()>0))){
            
            if ((selector==10)||(selector==11)){

                int change = 0;

                if ((selector==10)&&(Fl::event_key()=='o')){
                    if (  (selected>=0)   &&   (selected < static_cast<int>(current->walls_sitting.size() ) ) ){
                        if (current->walls_sitting[selected].transparent){
                            program->walls_mutex.lock();
                            current->walls_sitting[selected].transparent=0;
                            program->walls_mutex.unlock();
                            program->push_time();
                            program->push_output("Selected barrier set as opaque.\n");
                            program->prepare_undo();
                            change = 1;
                        }
                        else{
                            program->push_time();
                            program->push_output("Selected barrier already set as opaque.\n");
                            change = 1;
                        }
                    }
                    else if (group_selection_list.size()>0){
                        int count =0;
                        program->walls_mutex.lock();
                        for (unsigned int i=0; i< group_selection_list.size();i++){
                            wall &w = current->walls_sitting[group_selection_list[i]];
                            if (w.transparent){
                                w.transparent=0;
                                change = 1;
                                count++;
                            }
                        }
                        program->walls_mutex.unlock();
                        if (count){
                            program->push_time();
                            std::stringstream s;
                            s<<count<<" out of "<<group_selection_list.size()<<" selected barriers set as opaque."<<std::endl;
                            program->push_output(s.str());
                            program->prepare_undo();
                        }
                        else{
                            program->push_time();
                            program->push_output("All selected barriers already opaque.\n");
                        }
                    }
                }
                else if ((selector==10)&&(Fl::event_key()=='t')){
                    if ((selected>=0)&&(selected < static_cast<int>(current->walls_sitting.size()))){
                        if (!current->walls_sitting[selected].transparent){
                            program->walls_mutex.lock();
                            current->walls_sitting[selected].transparent=1;
                            program->walls_mutex.unlock();
                            program->push_time();
                            program->push_output("Selected barrier set as transparent.\n");
                            program->prepare_undo();
                            change = 1;
                        }
                        else{
                            program->push_time();
                            program->push_output("Selected barrier already set as transparent.\n");
                            change = 1;
                        }
                        
                    }
                    else if (group_selection_list.size()>0){
                        int count = 0;
                        program->walls_mutex.lock();
                        for (unsigned int i=0; i< group_selection_list.size();i++){
                            wall &w = current->walls_sitting[group_selection_list[i]];
                            if (!w.transparent){
                                w.transparent=1;
                                change = 1;
                                count++;
                            }
                        }
                        program->walls_mutex.unlock();
                        if (count){
                            program->push_time();
                            std::stringstream s;
                            s<<count<<" out of "<<group_selection_list.size()<<" selected barriers set as transparent."<<std::endl;
                            program->push_output(s.str());
                            program->prepare_undo();
                        }
                        else{
                            program->push_time();
                            program->push_output("All selected barriers already transparent.\n");
                        }
                    }
                }
                else if ((selector==11)&&(Fl::event_key()=='o')){
                    if ((selected>=0)&&(selected < static_cast<int>(current->walls_standing.size()))){
                        if (current->walls_standing[selected].transparent){
                            program->walls_mutex.lock();
                            current->walls_standing[selected].transparent=0;
                            program->walls_mutex.unlock();
                            program->push_time();
                            program->push_output("Selected barrier set as opaque.\n");
                            program->prepare_undo();
                            change = 1;
                        }
                        else{
                            program->push_time();
                            program->push_output("Selected barrier already set as opaque.\n");
                            change = 1;
                        }
                    }
                    else if (group_selection_list.size()>0){
                        int count = 0;
                        program->walls_mutex.lock();
                        for (unsigned int i=0; i< group_selection_list.size();i++){
                            wall &w = current->walls_standing[group_selection_list[i]];
                            if (w.transparent){
                                w.transparent=0;
                                change = 1;
                                count++;
                            }
                        }
                        program->walls_mutex.unlock();
                        if (count){
                            program->push_time();
                            std::stringstream s;
                            s<<count<<" out of "<<group_selection_list.size()<<" selected barriers set as opaque."<<std::endl;
                            program->push_output(s.str());
                            program->prepare_undo();
                        }
                        else{
                            program->push_time();
                            program->push_output("All selected barriers already opaque.\n");
                        }
                    }
                    
                }
                else if ((selector==11)&&(Fl::event_key()=='t')){
                    if ((selected>=0)&&(selected < static_cast<int>(current->walls_standing.size()))){
                        if (!current->walls_standing[selected].transparent){
                            program->walls_mutex.lock();
                            current->walls_standing[selected].transparent=1;
                            program->walls_mutex.unlock();
                            program->push_time();
                            program->push_output("Selected barrier set as transparent.\n");
                            program->prepare_undo();
                            change = 1;
                        }
                        else{
                            program->push_time();
                            program->push_output("Selected barrier already set as transparent.\n");
                            change = 1;
                        }
                    }
                    else if (group_selection_list.size()>0){
                        int count =0;
                        program->walls_mutex.lock();
                        for (unsigned int i=0; i< group_selection_list.size();i++){
                            wall &w = current->walls_standing[group_selection_list[i]];
                            if (!w.transparent){
                                w.transparent=1;
                                change = 1;
                                count++;
                            }
                        }
                        program->walls_mutex.unlock();
                        if (count){
                            program->push_time();
                            std::stringstream s;
                            s<<count<<" out of "<<group_selection_list.size()<<" selected barriers set as transparent."<<std::endl;
                            program->push_output(s.str());
                            program->prepare_undo();
                        }
                        else{
                            program->push_time();
                            program->push_output("All selected barriers already transparent.\n");
                        }
                    }

                }

                if (change){
                    this->redraw();
                    return 1;
                }
                
            }

            if (selector==14){

                int change = 0;

                if (Fl::event_key()=='p'){
                    if (  (selected>=0)   &&   (selected < static_cast<int>(current->floorarea_type.size() ) ) ){
                        if (current->floorarea_type[selected]){
                            program->areas_mutex.lock();
                            current->floorarea_type[selected]=0;
                            program->areas_mutex.unlock();
                            program->push_time();
                            program->push_output("Selected area set as positive.\n");
                            program->prepare_undo();
                            change = 1;
                        }
                        else{
                            program->push_time();
                            program->push_output("Selected area already set as positive.\n");
                            change = 1;
                        }
                    }
                    else if (group_selection_list.size()>0){
                        int count =0;
                        program->areas_mutex.lock();
                        for (unsigned int i=0; i< group_selection_list.size();i++){
                            int &a = current->floorarea_type[group_selection_list[i]];
                            if (a){
                                a=0;
                                change = 1;
                                count++;
                            }
                        }
                        program->areas_mutex.unlock();
                        if (count){
                            program->push_time();
                            std::stringstream s;
                            s<<count<<" out of "<<group_selection_list.size()<<" selected areas set as positive."<<std::endl;
                            program->push_output(s.str());
                            program->prepare_undo();
                        }
                        else{
                            program->push_time();
                            program->push_output("All selected areas already set as positive.\n");
                        }
                    }
                }
                else if (Fl::event_key()=='n'){
                    if ((selected>=0)&&(selected < static_cast<int>(current->floorarea_type.size()))){
                        if (!current->floorarea_type[selected]){
                            program->areas_mutex.lock();
                            current->floorarea_type[selected]=1;
                            program->areas_mutex.unlock();
                            program->push_time();
                            program->push_output("Selected area set as negative.\n");
                            program->prepare_undo();
                            change = 1;
                        }
                        else{
                            program->push_time();
                            program->push_output("Selected area already set as negative.\n");
                            change = 1;
                        }
                    }
                    else if (group_selection_list.size()>0){
                        int count =0;
                        program->areas_mutex.lock();
                        for (unsigned int i=0; i< group_selection_list.size();i++){
                            int &a = current->floorarea_type[group_selection_list[i]];
                            if (!a){
                                a=1;
                                change = 1;
                                count++;
                            }
                        }
                        program->areas_mutex.unlock();
                        if (count){
                            program->push_time();
                            std::stringstream s;
                            s<<count<<" out of "<<group_selection_list.size()<<" selected areas set as negative."<<std::endl;
                            program->push_output(s.str());
                            program->prepare_undo();
                        }
                        else{
                            program->push_time();
                            program->push_output("All selected areas already set as negative.\n");
                        }
                    }
                }

                
                if (change){
                    this->redraw();
                    return 1;
                }
            }
            
        }
        
    }
        //button presses for movement
        
    detect_movement_presses(e);

    switch (e) {
        case FL_ENTER:        
            return 1;
            break;

        case FL_LEAVE:

            this->redraw();            
            return 1;
            break;

        case FL_MOUSEWHEEL:
            if (Fl::event_inside(this)){
                
                if ((program->three_d_choice->value()==2)&&(Fl::event_ctrl())){
                 
                    int zooming = Fl::event_dy();
                    
                    double scale = 0.05*fabs(maxx_overall-minx_overall);

                    if (zooming>0){
                        z_dist+=scale*zooming;
                    }
                    else{
                        if (z_dist+scale*zooming>0){
                            z_dist+=scale*zooming;
                        }
                    }
                    
                }
                else{
                this->frac_x = (static_cast<double>(x_mouse-program->win->x()-this->x())/(  static_cast<double>(this->w())   ));
                this->frac_y = 1.0-(static_cast<double>(y_mouse-program->win->y()-this->y())/(  static_cast<double>(this->h())   ));

                
                this->zoom = 1;                
                
                this->zoom_mag = SYS_ZOOM * Fl::event_dy();
                    
                }
                
                this->redraw();
                return 1;
                
            }
            break;
        case FL_RELEASE:            
            
            //this conditional on mouse button? 
            if (was_dragging){
                was_dragging=0;
                program->prepare_undo();
            }

            if (group_select){
                find_selection_objects(0);
                group_select = 0;
            }
            
            return 1;
            break;
        case FL_KEYUP:
            
            break;
   
        case FL_PUSH:
            

            if ((program->root.calculating)&&(mark_locations_choice->value()>0)){
                program->no_editing();
                return 1;
            }

            group_selection_list.clear();

            this->pressed_mouse = Fl::event_button();
            this->xdown=Fl::event_x();
            this->ydown=Fl::event_y();
            this->xdragdown_old=this->xdown;
            this->ydragdown_old=this->ydown;
            this->miny_down=this->miny;
            this->maxy_down=this->maxy;
            this->minx_down=this->minx;
            this->maxx_down=this->maxx;
            
                    
            if (( Fl::event_button() == FL_RIGHT_MOUSE )&&(!program->three_d_choice->value())) { //right mouse: selection event or close geometry
                
                deselect();
                selector =mark_locations_choice->value();
                        
                std::vector<location> *temp_ptr = &current->locations[0]; //must initialise to soemthing for later reference
                std::vector<wall> *temp_wall_ptr = &current->walls_standing;
                const std::vector<vec>& temp_points = current->points;
                const std::vector<vertex>& temp_adj_list = current->adj_list;
                
                
                int loc_or_wall=-1;
                
                // -1:default
                // 0: location
                // 1: wall
                // 2: spatial graph point
                // 3: spatial graph link
                
                switch (mark_locations_choice->value()) {
                        
                    case 0:
                        loc_or_wall=-1; //just selecting point in space!
                        break;
                    case 1:
                        temp_ptr=&current->locations[0];
                        loc_or_wall=0;
                        break;
                    case 2:
                        temp_ptr=&current->locations[1];
                        loc_or_wall=0;
                        break;
                    case 3:
                        temp_ptr=&current->locations[2];
                        loc_or_wall=0;
                        break;
                    case 4:
                        temp_ptr=&current->locations[3];
                        loc_or_wall=0;
                        break;
                    case 5:
                        temp_ptr=&current->locations[4];
                        loc_or_wall=0;
                        break;
                    case 6:
                        temp_ptr=&current->locations[5];
                        loc_or_wall=0;
                        break;
                    case 7:
                        temp_ptr=&current->locations[6];
                        loc_or_wall=0;
                        break;
                    case 8:
                        temp_ptr=&current->locations[7];
                        loc_or_wall=0;
                        break;
                    case 9:
                        temp_ptr=&current->locations[8];
                        loc_or_wall=0;
                        break;
                    case 10:
                        temp_wall_ptr=&current->walls_sitting;
                        loc_or_wall=1;
                        break;
                    case 11:
                        temp_wall_ptr=&current->walls_standing;
                        loc_or_wall=1;
                        break;
                    case 12:
                        loc_or_wall=2;
                        break;
                    case 13:
                        loc_or_wall=3;
                        break;
                    case 14:
                    {
                        
                        selector=0;
                        
                        int ii=current->floorarea_polygons.size();
                        int jj=current->floorarea_polygons[ii-1].size();
                                
                        if ((!Fl::event_ctrl())&&(jj>0)){ //cancel floor area
                            program->floorpoly_reset();
                            //reverting back to no undo functionality for last polygon until I can figure out a better way
                            this->redraw();
                        }
                        else if (jj>2){ //close geometry
                            int intersect=0;// only close if shape won't intersect itself
                            for (int i=1;i<jj-2;i++){ //no point in testing first or last segment: gets confused anyway.. think about it....
                                
                                intersect+=intersect_test(current->floorarea_polygons[ii-1][0].x,current->floorarea_polygons[ii-1][jj-1].x,current->floorarea_polygons[ii-1][0].y,current->floorarea_polygons[ii-1][jj-1].y,current->floorarea_polygons[ii-1][i].x,current->floorarea_polygons[ii-1][i+1].x,current->floorarea_polygons[ii-1][i].y,current->floorarea_polygons[ii-1][i+1].y);
                                if (intersect>0){
                                    break;
                                }
                                
                            }
                            if (intersect==0){
                                
                                program->areas_mutex.lock();

                                current->floorarea_triangles.push_back(std::vector<triangle>(0));
                                ear_clipping(current->floorarea_polygons.back(),&current->floorarea_triangles.back());
                                current->floorarea_polygons.push_back(std::vector<vec>(0));
                                current->floorarea_type.push_back(root->mark_transparent_walls);
                                calculate_areas(&current->floorarea_polygons);

                                program->areas_mutex.unlock();

                                program->prepare_undo();
                            }
                            
                        }
                        else if (jj==0){  //selecting
                            
                            std::vector<int> selected_vec;
                            
                            for (unsigned int i=0;i<current->floorarea_polygons.size()-1;i++){

                                if (current->floorarea_polygons[i][0].floor == program->floor_choice->value()-1){
                                
                                    double x=(static_cast<double>(Fl::event_x())  /  static_cast<double>(this->w())  )*(maxx-minx)+minx;
                                    double y=(static_cast<double>(Fl::event_y())  /  static_cast<double>(this->h())  )*(miny-maxy)+maxy;
                                    
                                    if (pnpoly(x,y,current->floorarea_polygons[i])){
                                        selector=mark_locations_choice->value();
                                        selected_vec.push_back(i);
                                        
                                    }
                                }
                                
                            }

                            if (selected_vec.size()){

                                loop_select++;
                                if (loop_select>=static_cast<int>(selected_vec.size())){
                                    loop_select =0;
                                }

                                selected = selected_vec[loop_select];
                                
                            }
                            else{
                                loop_select=0;
                            }                            
                        }
                    }
                        
                        break;
                    default:
                        break;
                }


                const std::vector<location>& temp = *temp_ptr;
                const std::vector<wall>& temp_wall = *temp_wall_ptr;

                double x=(static_cast<double>(Fl::event_x())  /  static_cast<double> (this->w()) )*(maxx-minx)+minx;
                double y=(static_cast<double>(Fl::event_y())  /  static_cast<double> (this->h()) )*(miny-maxy)+maxy;
                double min=999999;
                double cutoff=0.04*(maxx-minx);
                
                this->xfp=x;
                this->yfp=y;
                this->zfp=1.8;
                    
                
                if ((loc_or_wall==0)&&(temp.size())){ //if selecting location
                    
                    
                    for (unsigned int i=0;i<temp.size();i++){
                        if ((all_flrs)||(temp[i].loc.floor==flr_choice)){
                            double dist=sqrt((temp[i].loc.x-x)*(temp[i].loc.x-x)+(temp[i].loc.y-y)*(temp[i].loc.y-y));
                            
                            if ((dist<min)&&(dist<cutoff)&&(temp[i].loc.floor==(program->floor_choice->value()-1))){
                                min=dist;
                                selected=i;
                                this->xfp=temp[i].loc.x;
                                this->yfp=temp[i].loc.y;
                                this->zfp=temp[i].loc.z+1.8;
                            }
                        }
                    }
                    
                    if ((selected>-1)&&(selected < static_cast<int>(temp.size()))){
                        program->location_label->value(temp[selected].label.c_str());
                        std::stringstream s;
                        s<<temp[selected].point;
                        program->overwrite_location_point->value(s.str().c_str());
                    }
                    
                }
                else if ((loc_or_wall==1)&&(temp_wall.size())){ // if selecting wall
                    
                    
                    
                    for (unsigned int i=0;i<temp_wall.size();i++){
                        if ((all_flrs)||(temp_wall[i].start.floor==flr_choice)){
                            
                            //start distance to segement
                            
                            double x1=temp_wall[i].start.x;
                            double x2=temp_wall[i].end.x;
                            double y1=temp_wall[i].start.y;
                            double y2=temp_wall[i].end.y;
                            
                            double px=x2-x1;
                            double py=y2-y1;
                            double s=px*px+py*py;
                            double u=((x-x1)*px+(y-y1)*py)/s;
                            if (u>1)u=1;
                            if (u<0)u=0;
                            double dx=x1+u*px-x;
                            double dy=y1+u*py-y;
                            double dist=sqrt(dx*dx+dy*dy);
                            
                            //end distance to segement
                            if ((dist<min)&&(dist<cutoff)&&(temp_wall[i].start.floor==(program->floor_choice->value()-1))){
                                min=dist;
                                selected=i;
                            }
                        }
                    }
                }
                else if ((loc_or_wall==2)&&(temp_points.size())){ // if selecting spatial graph point
                    
                    
                    for (unsigned int i=0;i<temp_points.size();i++){
                        if ((all_flrs)||(temp_points[i].floor==flr_choice)){
                            double dist=sqrt((temp_points[i].x-x)*(temp_points[i].x-x)+(temp_points[i].y-y)*(temp_points[i].y-y));
                            
                            if ((dist<min)&&(dist<cutoff)){
                                min=dist;
                                selected=i;
                                this->xfp=temp_points[i].x;
                                this->yfp=temp_points[i].y;
                                this->zfp=temp_points[i].z+1.8;
                            }
                        }
                    }
                    
                    
                }
                else if ((loc_or_wall==3)&&(temp_adj_list.size())){ // if selecting spatial graph link
                    
                    
                    for (unsigned int ii=0;ii<temp_adj_list.size();ii++){
                        int i = temp_adj_list[ii].start;
                        int j = temp_adj_list[ii].end;
                        
                        if ((all_flrs)||((current->points[i].floor==flr_choice)&&(current->points[j].floor==flr_choice))){
                            
                            double x1=current->points[i].x;
                            double x2=current->points[j].x;
                            double y1=current->points[i].y;
                            double y2=current->points[j].y;
                            
                            double px=x2-x1;
                            double py=y2-y1;
                            double s=px*px+py*py;
                            double u=((x-x1)*px+(y-y1)*py)/s;
                            if (u>1)u=1;
                            if (u<0)u=0;
                            double dx=x1+u*px-x;
                            double dy=y1+u*py-y;
                            double dist=sqrt(dx*dx+dy*dy);
                            
                            //end distance to segement
                            
                            if ((dist<min)&&(dist<cutoff)){
                                min=dist;
                                selected = ii;
                                
                            }
                            
                        }
                        
                    }                                    
                    
                }
                
               
               this->redraw();
            } // end of selection/beginning of marking
            else if ((Fl::event_button() == FL_LEFT_MOUSE)&&(!program->three_d_choice->value())&&(!Fl::event_ctrl())){ //left mouse addition event : has to be 2D and not cancelled with cntrl
                
                if ((((mark_locations_choice->value()==10)||(mark_locations_choice->value()==11)))){ //drawing walls

                    
                    int temp_first=set_first;
                    deselect();
                    set_first=temp_first;
                    
                    wall temp;
                    temp.start.x=(  static_cast<double>(Fl::event_x())  /  static_cast<double> (this->w() ) )*(maxx-minx)+minx;
                    temp.start.y=(  static_cast<double>(Fl::event_y())  /  static_cast<double> (this->h() ) )*(miny-maxy)+maxy;
                    
                    
                    if (snapon){    //snap on overwrite
                        temp.start.x=snapx;
                        temp.start.y=snapy;
                    }
                    
                    
                    
                    temp.start.floor=program->floor_choice->value()-1;
                    temp.start.z=current->floor_index[program->floor_choice->value()-1];
                    
                    
                    if (set_first==0){
                        temp_wall.start=temp.start;
                        set_first=1;
                    }
                    else{
                        temp_wall.end=temp.start;
                        

                                            
                        if (program->root.mark_transparent_walls){
                            temp_wall.transparent=1;
                        }
                        else{
                            temp_wall.transparent=0;
                        }

                        if (mark_locations_choice->value()==10){
                            program->walls_mutex.lock();
                            current->walls_sitting.push_back(temp_wall);
                            program->walls_mutex.unlock();
                            program->prepare_undo();
                        }
                        else{
                            program->walls_mutex.lock();
                            current->walls_standing.push_back(temp_wall);
                            program->walls_mutex.unlock();
                            program->prepare_undo();
                        }
                        set_first=0;
                        show_warning_visibility();
                    }
                    
                    snap_point = -1;

                    this->redraw();
                }
                else if((mark_locations_choice->value()==12)){ //marking/editing spatial graph current->points
                    
                    
                    deselect();
                    selector=mark_locations_choice->value();
                                                            
                    
                    if ((Fl::event_alt())||(x_down)){ //start dragging a point around
                        
                        double x=(static_cast<double>(Fl::event_x())/static_cast<double>(this->w()))*(maxx-minx)+minx;
                        double y=(static_cast<double>(Fl::event_y())/static_cast<double>(this->h()))*(miny-maxy)+maxy;
                        
                        double min=9e99;
                        double dist_crit =0.5;
                        if (program->point_size->value()>dist_crit){
                            dist_crit=program->point_size->value();
                        }
                        
                        for (unsigned int i=0;i<current->points.size();i++){
                            if ((all_flrs)||(current->points[i].floor==flr_choice)){
                                double dist=sqrt((current->points[i].x-x)*(current->points[i].x-x)+(current->points[i].y-y)*(current->points[i].y-y));
                                
                                if ((dist<min)&&(dist<dist_crit)){
                                    min=dist;
                                    drag_point=i;
                                    was_dragging=1;
                                }
                            }
                        }

                    }
                    else{ //normal point adding
                        
                        vec temp;
                        temp.x=(static_cast<double>(Fl::event_x())/static_cast<double>(this->w()))*(maxx-minx)+minx;
                        temp.y=(static_cast<double>(Fl::event_y())/static_cast<double>(this->h()))*(miny-maxy)+maxy;
                        temp.floor=program->floor_choice->value()-1;
                        temp.z=current->floor_index[program->floor_choice->value()-1];
                        
                        if (snapon){   //snap on overwrite
                            temp.x=snapx;
                            temp.y=snapy;
                        }
                        
                        program->points_mutex.lock();
                        current->points.push_back(temp);
                        program->points_mutex.unlock();

                        if (root->autoconnect){
                            if (current->points.size()>1){
                                vertex temp;
                                int i =  current->points.size()-1;
                                temp.start= i ;
                                temp.end = i-1;
                        
                                program->adj_mutex.lock();
                                current->adj_list.push_back(temp);
                                program->adj_mutex.unlock();

                                int floor1 = current->points.at(temp.start).floor;
                                int floor2 = current->points.at(temp.end).floor;

                                if (floor1!=floor2){
                                    std::stringstream s;
                                    s<<"WARNING: Auto-connecting points "<<temp.start<<" and "<<temp.end<<" across different floors ("<<floor1<<" and "<<floor2<<")."<<std::endl;

                                    program->push_time();
                                    program->push_output(s.str());
                                }
                                
                            }
                        }
                                       
                        show_warning_map();

                        program->prepare_undo();
                        
                    }
                    
                }
                else if ((mark_locations_choice->value()==13)){ //selecting current->matrix link
                    
                    
                    selected=-1; //can't use deselect() here
                    selector=-1;
                    selected_matrix[0]=-1;
                    selected_matrix[1]=-1;
                    drag_point=-1;
                    
                    double x=(static_cast<double>(Fl::event_x())/static_cast<double>(this->w()))*(maxx-minx)+minx;
                    double y=(static_cast<double>(Fl::event_y())/static_cast<double>(this->h()))*(miny-maxy)+maxy;

                    if (snapon){
                        x=snapx;
                        y=snapy;
                    }
                    
                    double min=9e99;
                    
                    int finish_link=0;
                    
                    if ((start_link>-1)&&(end_link<0)){
                        finish_link=1;
                    }                                       
                    
                    double dist_crit =0.5*program->point_size->value();
                    
                    for (unsigned int i=0;i<current->points.size();i++){
                        if ((all_flrs)||(current->points[i].floor==flr_choice)){
                            double dist=sqrt((current->points[i].x-x)*(current->points[i].x-x)+(current->points[i].y-y)*(current->points[i].y-y));                                                        
                            
                            if ((dist<min)&&(dist<dist_crit)){
                                min=dist;
                                if (start_link>-1){
                                    end_link=i;
                                                                        
                                    if (start_link!=end_link){
                                        
                                        int found=0;
                                        for (unsigned int ii=0;ii<current->adj_list.size();ii++){
                                            
                                            if (((current->adj_list[ii].start==start_link)&&(current->adj_list[ii].end==end_link))||((current->adj_list[ii].start==end_link)&&(current->adj_list[ii].end==start_link))){
                                                found=1;
                                            }                                                                                        
                                        }
                                        
                                        if (!found){
                                            vertex temp;
                                            temp.start=start_link;
                                            temp.end=end_link;

                                            program->adj_mutex.lock();
                                            current->adj_list.push_back(temp);
                                            program->adj_mutex.unlock();

                                            show_warning_map();
                                            program->prepare_undo();
                                        }
                                        
                                    }
                                    start_link=-1;
                                    end_link=-1;
                                    
                                }
                                else{
                                    start_link=i;                                    
                                }
                            }
                        }
                    }
                    
                    if (finish_link){ //selected first but failed with second
                        if ((start_link>-1)&&(end_link<0)){
                            start_link=-1;
                        }
                    }
                    
                    
                }
                
                else if ((mark_locations_choice->value()==14)){ //floor areas

                    deselect();
                    
                    double x=(static_cast<double>(Fl::event_x())/static_cast<double>(this->w()))*(maxx-minx)+minx;
                    double y=(static_cast<double>(Fl::event_y())/static_cast<double>(this->h()))*(miny-maxy)+maxy;
                    
                    vec temp;
                    temp.x=x;
                    temp.y=y;
                    temp.z=0;
                    temp.floor=program->floor_choice->value()-1;
                    
                    if (snapon){   //snap on overwrite
                        temp.x=snapx;
                        temp.y=snapy;
                    }
                    
                    
                    int ii=current->floorarea_polygons.size();
                    int jj=current->floorarea_polygons[ii-1].size();
                    
                    //add point
                    
                    int intersect=0;// only add point if created segment doesn't overlap
                    for (int i=0;i<jj-2;i++){ //no point in testing last segment: gets confused anyway
                        intersect+=intersect_test(temp.x,current->floorarea_polygons[ii-1][jj-1].x,temp.y,current->floorarea_polygons[ii-1][jj-1].y,current->floorarea_polygons[ii-1][i].x,current->floorarea_polygons[ii-1][i+1].x,current->floorarea_polygons[ii-1][i].y,current->floorarea_polygons[ii-1][i+1].y);
                    }
                    if (intersect==0){
                        
                        program->areas_mutex.lock();

                        current->floorarea_polygons[current->floorarea_polygons.size()-1].push_back(temp);
                        snap_point=-1; //so that new guidelines are drawn relative to most recent point

                        program->areas_mutex.unlock();
                        
                    }
                                                            
                }
                
                else if (mark_locations_choice->value()){ //marking normal locations
                    
                    deselect();
                                      
                    selector=mark_locations_choice->value();
  

                    if ((Fl::event_alt())||(x_down)){ //start dragging a point around
                        
                        
                        std::vector<location> *temp_location_ptr = &current->locations[0];
                        
                        switch (mark_locations_choice->value()) {
                                
                            case 0:
                                
                                break;
                            case 1:
                                temp_location_ptr=&current->locations[0];
                                
                                break;
                            case 2:
                                temp_location_ptr=&current->locations[1];
                                
                                break;
                            case 3:
                                temp_location_ptr=&current->locations[2];
                                
                                break;
                            case 4:
                                temp_location_ptr=&current->locations[3];
                                
                                break;
                            case 5:
                                temp_location_ptr=&current->locations[4];
                                
                                break;
                            case 6:
                                temp_location_ptr=&current->locations[5];
                                
                                break;
                            case 7:
                                temp_location_ptr=&current->locations[6];
                                
                                break;
                            case 8:
                                temp_location_ptr=&current->locations[7];
                                
                                break;
                            case 9:
                                temp_location_ptr=&current->locations[8];
                                
                                break;
                        }
                        
                        const std::vector<location>& temp_location = *temp_location_ptr;

                                                                
                        double x=(static_cast<double>(Fl::event_x())/static_cast<double>(this->w()))*(maxx-minx)+minx;
                        double y=(static_cast<double>(Fl::event_y())/static_cast<double>(this->h()))*(miny-maxy)+maxy;
                        
                        double min=9e99;
                        for (unsigned int i=0;i<temp_location.size();i++){
                            if ((all_flrs)||(temp_location[i].loc.floor==flr_choice)){
                                double dist=sqrt((temp_location[i].loc.x-x)*(temp_location[i].loc.x-x)+(temp_location[i].loc.y-y)*(temp_location[i].loc.y-y));
                                
                                if ((dist<min)&&(dist<0.5)){
                                    min=dist;
                                    drag_point=i;
                                    was_dragging=1;
                                }
                            }
                        }                                               
                        
                    }
                    else{ //actual adding of locations here
                        
                        
                        int found=0;
                        for (unsigned int i=0;i<current->points.size();i++){
                            if (current->points[i].floor==program->floor_choice->value()-1){
                                found=1;
                                break;
                            }
                        }
                        if (found){
                            
                            location temp;                        

                            temp.loc.x=(static_cast<double>(Fl::event_x())/static_cast<double>(this->w()))*(maxx-minx)+minx;
                            temp.loc.y=(static_cast<double>(Fl::event_y())/static_cast<double>(this->h()))*(miny-maxy)+maxy;
                            
                            
                            if (snapon){   //snap on overwrite
                                temp.loc.x=snapx;
                                temp.loc.y=snapy;
                            }
                            
                            for (unsigned int i=0;i<current->points.size();i++){
                                if (current->points[i].floor==(program->floor_choice->value()-1)){                                                                      
                                    temp.loc.floor=program->floor_choice->value()-1;
                                    temp.loc.z=current->floor_index[program->floor_choice->value()-1];
                                    
                                }
                            }
                                                                                    
                            double min=9e99; //point set
                            int ii=0;
                            
                            for (unsigned int i=0;i<current->points.size();i++){
                                
                                if (current->points[i].floor==temp.loc.floor){
                                    double r= sqrt((current->points[i].x-temp.loc.x)*(current->points[i].x-temp.loc.x)+(current->points[i].y-temp.loc.y)*(current->points[i].y-temp.loc.y));
                                    if (r<min){
                                        ii=i;
                                        min=r;
                                    }
                                }
                            }
                            
                            temp.point=ii;                            
                            
                            temp.segment=0;
                            temp.is_desk=0;
                            
                            temp.label="";
                            
                            switch (mark_locations_choice->value()){
                                case 1:
                                    temp.is_desk=1;
                                    program->locations_mutex.lock();
                                    current->locations[0].push_back(temp);
                                    program->locations_mutex.unlock();
                                    show_warning_visibility();
                                    program->prepare_undo();
                                    break;
                                case 2:
                                    program->locations_mutex.lock();
                                    current->locations[1].push_back(temp);
                                    program->locations_mutex.unlock();
                                    show_warning_visibility();
                                    program->prepare_undo();
                                    break;
                                case 3:
                                    program->locations_mutex.lock();
                                    current->locations[2].push_back(temp);
                                    program->locations_mutex.unlock();
                                    show_warning_visibility();
                                    program->prepare_undo();
                                    break;
                                case 4:
                                    program->locations_mutex.lock();
                                    current->locations[3].push_back(temp);
                                    program->locations_mutex.unlock();
                                    show_warning_visibility();
                                    program->prepare_undo();
                                    break;
                                case 5:
                                    program->locations_mutex.lock();
                                    current->locations[4].push_back(temp);
                                    program->locations_mutex.unlock();
                                    show_warning_visibility();
                                    program->prepare_undo();
                                    break;
                                case 6:
                                    program->locations_mutex.lock();
                                    current->locations[5].push_back(temp);
                                    program->locations_mutex.unlock();
                                    show_warning_visibility();
                                    program->prepare_undo();
                                    break;
                                case 7:
                                    program->locations_mutex.lock();
                                    current->locations[6].push_back(temp);
                                    program->locations_mutex.unlock();
                                    show_warning_visibility();
                                    program->prepare_undo();
                                    break;
                                case 8:
                                    program->locations_mutex.lock();
                                    current->locations[7].push_back(temp);
                                    program->locations_mutex.unlock();
                                    show_warning_visibility();
                                    program->prepare_undo();
                                    break;
                                case 9:
                                    program->locations_mutex.lock();
                                    current->locations[8].push_back(temp);
                                    program->locations_mutex.unlock();
                                    show_warning_visibility();
                                    program->prepare_undo();
                                    break;
                            }
                            std::stringstream s;
                            s<<ii;
                            program->overwrite_location_point->value(s.str().c_str());
                        }
                        else{
                            program->push_time();
                            program->push_output("No spatial graph vertices found on this floor! Complete the spatial graph to continue.\n");
                        }
                    }
                    this->redraw();
                    
                }
                
            }
            return 1;
            
        case FL_DRAG:

            if (!program->three_d_choice->value()&&(this->pressed_mouse == FL_RIGHT_MOUSE)){    
                if (program->floor_choice->value()){
                    deselect();
                    group_select = 1;
                    this->drag = 0;
                    this->redraw();
                    return 1;
                }
            }
            else{
            
                if ((drag_point>-1)&&((Fl::event_alt())||(x_down))){
                
                                        
                    double x=(static_cast<double>(Fl::event_x())/static_cast<double>(this->w()))*(maxx-minx)+minx;
                    double y=(static_cast<double>(Fl::event_y())/static_cast<double>(this->h()))*(miny-maxy)+maxy;
                    
                    if (snapon){   //snap on overwrite
                        x=snapx;
                        y=snapy;
                    }
                    
                    
                    program->points_mutex.lock();
                    program->locations_mutex.lock();
                    
                    int moved=0;

                    switch (selector){
                            
                        case 0:
                            break;
                        case 1:
                            current->locations[0][drag_point].loc.x=x;
                            current->locations[0][drag_point].loc.y=y;
                            moved=1;                    
                            break;
                        case 2:
                            current->locations[1][drag_point].loc.x=x;
                            current->locations[1][drag_point].loc.y=y;
                            moved=1;
                            break;
                        case 3:
                            current->locations[2][drag_point].loc.x=x;
                            current->locations[2][drag_point].loc.y=y;
                            moved=1;
                            break;
                        case 4:
                            current->locations[3][drag_point].loc.x=x;
                            current->locations[3][drag_point].loc.y=y;
                            moved=1;
                            break;
                        case 5:
                            current->locations[4][drag_point].loc.x=x;
                            current->locations[4][drag_point].loc.y=y;
                            moved=1;
                            break;
                        case 6:
                            current->locations[5][drag_point].loc.x=x;
                            current->locations[5][drag_point].loc.y=y;
                            moved=1;
                            break;
                        case 7:
                            current->locations[6][drag_point].loc.x=x;
                            current->locations[6][drag_point].loc.y=y;
                            moved=1;
                            break;
                        case 8:
                            current->locations[7][drag_point].loc.x=x;
                            current->locations[7][drag_point].loc.y=y;
                            moved=1;
                            break;
                        case 9:
                            current->locations[8][drag_point].loc.x=x;
                            current->locations[8][drag_point].loc.y=y;
                            moved=1;
                            break;
                        case 10:
                            current->walls_sitting[drag_point].start.x=x;
                            current->walls_sitting[drag_point].start.y=y;
                            moved=1;
                            break;
                        case 11:
                            current->walls_standing[drag_point].start.x=x;
                            current->walls_standing[drag_point].start.y=y;
                            moved=1;
                            break;
                        case 12:
                            current->points[drag_point].x=x;
                            current->points[drag_point].y=y;
                            moved=2;
                            break;
                        case 13:
                            break;
                            
                    }
                    
                    program->locations_mutex.unlock();                    
                    program->points_mutex.unlock();
                    
                    
                    if (moved==1){
                        show_warning_visibility();
                    }
                    else if(moved==2){
                        show_warning_map();
                    }

                }                
                
                this->xdragdown=Fl::event_x();
                this->ydragdown=Fl::event_y();
                this->drag=1;  
                
                if ((program->three_d_choice->value())&&(!Fl::event_ctrl())&&(Fl::event_button()!=FL_MIDDLE_MOUSE)){ //only rotate if dragging in 3D!!
                    double ydrag=(this->ydragdown-this->ydragdown_old)/2.0;
                    double xdrag=(this->xdragdown-this->xdragdown_old)/2.0;
                    
                    if (program->three_d_choice->value()==3){ //more manageable rotation in first person                                                
                        
                        double FOV = 180.0*(2.0*(atan(0.5*(maxx-minx)/(maxx_overall-minx_overall)))/M_PI);
                        
                        xdrag*=FOV/120.0;
                        ydrag*=FOV/120.0;
                                               
                        if ((ang_xfp+ydrag<0)&&(ang_xfp+ydrag>-180)){
                            this->ang_xfp+=ydrag;
                        }
                        this->ang_zfp+=xdrag;
                            
                        this->redraw();
                        
                    }
                    else{
                        
                        this->ang_x+=ydrag;
                        this->ang_z+=xdrag;
                    }
                
                    this->drag=0;//only rotate or drag at once
                }
                else if(Fl::event_ctrl()){
                    this->drag=1;
                }
                
        

                if ((mark_locations_choice->value())&&(!Fl::event_ctrl())&&(Fl::event_button()==FL_LEFT_MOUSE)){ //stop dragging whilst marking unless holding cntrl;
                    this->drag=0;
                }
                
                if ((snap_point>-1)&&((mark_locations_choice->value()==12)||(mark_locations_choice->value()==1)||(mark_locations_choice->value()==14))&&(Fl::event_ctrl())){//current->origins or current->points
                    
                                      
                    double x=(static_cast<double>(Fl::event_x())/static_cast<double>(this->w()))*(maxx-minx)+minx;
                    double y=(static_cast<double>(Fl::event_y())/static_cast<double>(this->h()))*(miny-maxy)+maxy;
                    
                    double  xx=0,yy=0; 
                    
                    switch (mark_locations_choice->value()){
                        case 1:

                            if (snap_point>static_cast<int>(current->locations[0].size())){
                                break;
                            }
                            xx=current->locations[0][snap_point].loc.x;
                            yy=current->locations[0][snap_point].loc.y;
                            break;
                        case 12:
                            if (snap_point>static_cast<int>(current->points.size())){
                                break;
                            }
                            xx=current->points[snap_point].x;
                            yy=current->points[snap_point].y;
                            break;
                        case 14:
                            if (snap_point>static_cast<int>(current->floorarea_polygons.size())){
                                break;
                            }
                            else if (snap_point_plus>static_cast<int>(current->floorarea_polygons[snap_point].size())){
                                break;
                            }
                            xx=current->floorarea_polygons[snap_point][snap_point_plus].x;
                            yy=current->floorarea_polygons[snap_point][snap_point_plus].y;
                            break;
                    }
                                       
                    ang_snap=atan2(y-yy,x-xx);
                    this->drag=0;
                    
                }                
                
                if ((Fl::event_alt())||(x_down)){ // no dragging floorplan whilst dragging a point
                    this->drag=0;
                }

                if (Fl::event_button()==FL_MIDDLE_MOUSE){

                    this->drag=1;
                }
                  
                this->redraw();
                this->xdragdown_old=this->xdragdown;
                this->ydragdown_old=this->ydragdown;
            }
        
            return 1;
            break;
            
            
        case FL_MOVE:
                        
            this->redraw();
            break;
        default:
            break;
    }
  
    
    
    
    if ((snapon!=old_snapon)||(to_redraw)){
        this->redraw();
    }
    
    return ret;

}