/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/base.hpp"
#include "./include/choicewin.hpp"

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
    #define STB_IMAGE_WRITE_IMPLEMENTATION
#endif

#include "./include/stb_image_write.h"



int base::choice(const char *m,const char *a,const char *b,const char *c){//replicate fl_choice signature

    int state=-1;
    choicewin *A;
    if (!c){
        A = new choicewin(this,state,m,a,b,"","");
    }
    else{
        A = new choicewin(this,state,m,a,b,c,"");   
    }
    A->show();
    return state;

}

int base::choice(const char *m,const char *a,const char *b,const char *c,const char *d){//extend fl_choice signature

    int state=-1;
    choicewin *A;
    if ((!c)&&(!b)){
        A = new choicewin(this,state,m,a,"","",d);
    }
    else if (!c){
        A = new choicewin(this,state,m,a,b,"",d);
    }
    else{
        A = new choicewin(this,state,m,a,b,c,d);   
    }
    A->show();
    return state;

}


void base::select_all_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->draw->find_selection_objects(1);
}

void base::lattice_choice_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->lattice_choice_cb();
}


void base::lattice_choice_cb(){

    if (lattice_choice->value()){
        movement_vis_choice->value(1);
        Fl_Menu_Item  *temp;
        temp =  (Fl_Menu_Item*)(&movement_vis_choice->menu()[0]);
        temp->deactivate(); 
    }
    else{
        Fl_Menu_Item  *temp;
        temp =  (Fl_Menu_Item*)(&movement_vis_choice->menu()[0]);
        temp->activate(); 
    }
}

void base::limit_mesh_edge_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->limit_mesh_edge_cb();
}


void base::limit_mesh_edge_cb(){

    if (limit_mesh_edge->value()){
        mesh_edge_max->activate();
    }
    else{
        mesh_edge_max->deactivate();
    }
}

void base::cursor_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->cursor_cb();
}


void base::cursor_cb(){

    if (!root.graphic_cursor){
        root.graphic_cursor=1;
        push_time();
        push_output("Switching to rendering cursor graphically in program.\n");
    }
    else{
        root.graphic_cursor=0;
        push_time();
        push_output("Switching to system cursor.\n");
    }

    draw->redraw();
}


void base::floorplan_win_cb(Fl_Widget*,void* data){
    base* program = static_cast<base*> (data);
    program->floorplan_win->hide();
    //Fl::focus(program->win);
    program->root.show_floor_win=0;
}

void base::floorplan_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->floorplan_cb();
}


void base::floorplan_cb(){

    if (!root.show_floor_win){


        if (!root.draw_floorplan){
            
            Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[FLOORPLAN_INDEX]);   
            root.draw_floorplan=1;
            temp->set();
            
        }

        floorplan_win->set_non_modal();
        floorplan_win->show();
        floorplan_win->resize(win->x()+draw->x()+draw->w()-250,win->y()+draw->y()+55,250,190+30);
        root.show_floor_win=1;
    }
    else{

#if defined __APPLE__ || defined WIN32 // Avoid hide/show input spamming leading to breaking keyboard input on Linux/X11
        floorplan_win->hide();
        root.show_floor_win=0;
        //Fl::focus(this->win);
#endif

    }
    draw->redraw();
}

void base::floorplan_cb_button_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->floorplan_cb_button();
}


void base::floorplan_cb_button(){

    if (!root.show_floor_win){


        if (!root.draw_floorplan){
            
            Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[FLOORPLAN_INDEX]);   
            root.draw_floorplan=1;
            temp->set();
            
        }

        floorplan_win->set_non_modal();
        floorplan_win->show();
        floorplan_win->resize(win->x()+draw->x()+draw->w()-250,win->y()+draw->y()+55,250,190+30);
        root.show_floor_win=1;
    }
    else{

        floorplan_win->hide();
        root.show_floor_win=0;
        //Fl::focus(this->win);

    }
    draw->redraw();
}



void base::panel_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->panel_cb();
}


void base::panel_cb(){

    int dx=1,ddx=0;
    if (!root.bow){
        dx=0;ddx=0;
    }

    if (!root.fullscreen){

        if (root.hide_side_bar){
            
            draw->resize(10+dx,22+menu_height-sysoffset+dx,o1->w()-255-15-ddx-2*dx,draw->h());
            draw_box->resize(draw->x()-dx,draw->y()-dx,draw->w()+2*dx,draw->h()+2*dx);
            out_data->resize(10+dx,22+menu_height-sysoffset+dx,o2->w()-255-15-2*dx,out_data->h());
            out_data_box->resize(out_data->x()-dx,out_data->y()-dx,out_data->w()+2*dx,out_data->h()+2*dx);
            plot->resize(10+dx,22+menu_height-sysoffset+dx,o3->w()-255-15-2*dx,plot->h());
            graph_box->resize(plot->x()-dx,plot->y()-dx,plot->w()+2*dx,plot->h()+2*dx);
            o1->add(p1);
            p1->resize(draw_box->x()+draw_box->w(),22+menu_height-sysoffset,260,o1->h()-2);
            o2->add(p2);
            p2->resize(out_data_box->x()+out_data_box->w(),22+menu_height-sysoffset,260,o2->h()-2);
            o3->add(p3);
            p3->resize(graph_box->x()+graph_box->w(),22+menu_height-sysoffset,260,o3->h()-2);
            root.hide_side_bar=0;

        }
        else{
            
            draw->resize(10+dx,22+menu_height-sysoffset+dx,o1->w()-2*dx,draw->h());
            draw_box->resize(draw->x()-dx,draw->y()-dx,draw->w()+2*dx,draw->h()+2*dx);
            out_data->resize(10+dx,22+menu_height-sysoffset+dx,o2->w()-2*dx,out_data->h());
            out_data_box->resize(out_data->x()-dx,out_data->y()-dx,out_data->w()+2*dx,out_data->h()+2*dx);
            plot->resize(10+dx,22+menu_height-sysoffset+dx,o3->w()-2*dx,plot->h());
            graph_box->resize(plot->x()-dx,plot->y()-dx,plot->w()+2*dx,plot->h()+2*dx);
            o1->remove(p1);
            o2->remove(p2);
            o3->remove(p3);
            root.hide_side_bar=1;
        }
     
        //if(oo->value()==o2){ //total hack - cant make it redraw otherwise...       
            // win->resize(win->x(),win->y(),win->w()+1,win->h());   
            // win->resize(win->x(),win->y(),win->w()-1,win->h());   
        //}
    }
   
    draw_box->redraw();
    out_data_box->redraw();
    graph_box->redraw();
    oo->redraw();
    win->redraw();
   
}


void base::info_bar_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->info_bar_cb();
}


void base::info_bar_cb(){

    int dx = 1,ddx=0;

    if (!root.bow){
        dx=0;ddx=0;
    }

    if (!root.fullscreen){

        if (root.hide_info_bar){
            
            out_read->show();
            win->add(out_read);
            win->add(out_read_box);
            out_read->resize(10,win->h()-120,win->w()-20,110);
            out_read_box->resize(out_read->x()-1,out_read->y()-1,out_read->w()+2,out_read->h()+2);

            int temp = out_read->h()+30;
            root.hide_info_bar=0;
            ooo->resize(10,menu_height-sysoffset,win->w()-20,win->h()-temp+sysoffset);
            oo->resize(10,menu_height-sysoffset,win->w()-20,win->h()-temp+sysoffset);
            o1->resize(10, 20+menu_height-sysoffset,  oo->w(),oo->h()-10-menu_height+10);
            o2->resize(10, 20+menu_height-sysoffset,  oo->w(),oo->h()-10-menu_height+10);
            o3->resize(10, 20+menu_height-sysoffset,  oo->w(),oo->h()-10-menu_height+10);

            if (root.hide_side_bar){
                draw->resize(10+dx,22+menu_height-sysoffset+dx,o1->w()-2*dx,oo->h()-12-menu_height-2*dx);
                draw_box->resize(draw->x()-dx,draw->y()-dx,draw->w()+2*dx,draw->h()+2*dx);
                out_data->resize(10+dx,22+menu_height-sysoffset+dx,o2->w()-2*dx,oo->h()-12-menu_height-2*dx);
                out_data_box->resize(out_data->x()-dx,out_data->y()-dx,out_data->w()+2*dx,out_data->h()+2*dx);
                plot->resize(10+dx,22+menu_height-sysoffset-dx,o3->w()-2*dx,oo->h()-12-menu_height-2*dx);
                graph_box->resize(plot->x()-dx,plot->y()-dx,plot->w()+2*dx,plot->h()+2*dx);
            }
            else{
                draw->resize(10+dx,22+menu_height-sysoffset+dx,o1->w()-255-15-ddx-2*dx,oo->h()-12-menu_height-2*dx);
                draw_box->resize(draw->x()-dx,draw->y()-dx,draw->w()+2*dx,draw->h()+2*dx);
                out_data->resize(10+dx,22+menu_height-sysoffset+dx,o2->w()-250-2*dx,oo->h()-12-menu_height-2*dx);
                out_data_box->resize(out_data->x()-dx,out_data->y()-dx,out_data->w()+2*dx,out_data->h()+2*dx);
                plot->resize(10+dx,22+menu_height-sysoffset+dx,o3->w()-255-15-2*dx,oo->h()-12-menu_height-2*dx);
                graph_box->resize(plot->x()-dx,plot->y()-dx,plot->w()+2*dx,plot->h()+2*dx);
            }
            
        }
        else{
            
            out_read->hide();
            root.hide_info_bar=1;
            ooo->resize(10,menu_height-sysoffset,win->w()-20,win->h()-20+sysoffset);
            oo->resize(10,menu_height-sysoffset,win->w()-20,win->h()-20+sysoffset);
            o1->resize(10, 20+menu_height-sysoffset,  oo->w(),oo->h()-10-menu_height+10);
            o2->resize(10, 20+menu_height-sysoffset,  oo->w(),oo->h()-10-menu_height+10);
            o3->resize(10, 20+menu_height-sysoffset,  oo->w(),oo->h()-10-menu_height+10);
            if (root.hide_side_bar){
                draw->resize(10+dx,22+menu_height-sysoffset+dx,o1->w()-2*dx,oo->h()-12-menu_height-2*dx-5);
                draw_box->resize(draw->x()-dx,draw->y()-dx,draw->w()+2*dx,draw->h()+2*dx);
                out_data->resize(10+dx,22+menu_height-sysoffset+dx,o2->w()-2*dx,oo->h()-12-menu_height-2*dx-5);
                out_data_box->resize(out_data->x()-dx,out_data->y()-dx,out_data->w()+2*dx,out_data->h()+2*dx);
                plot->resize(10+dx,22+menu_height-sysoffset+dx,o3->w()-2*dx,oo->h()-12-menu_height-2*dx-5);
                graph_box->resize(plot->x()-dx,plot->y()-dx,plot->w()+2*dx,plot->h()+2*dx);
            }
            else{
                draw->resize(10+dx,22+menu_height-sysoffset+dx,o1->w()-255-15-ddx-2*dx,oo->h()-12-menu_height-2*dx-5);
                draw_box->resize(draw->x()-dx,draw->y()-dx,draw->w()+2*dx,draw->h()+2*dx);
                out_data->resize(10+dx,22+menu_height-sysoffset+dx,o2->w()-250-2*dx,oo->h()-12-menu_height-2*dx-5);
                out_data_box->resize(out_data->x()-dx,out_data->y()-dx,out_data->w()+2*dx,out_data->h()+2*dx);
                plot->resize(10+dx,22+menu_height-sysoffset+dx,o3->w()-255-15-2*dx,oo->h()-12-menu_height-2*dx-5);
                graph_box->resize(plot->x()-dx,plot->y()-dx,plot->w()+2*dx,plot->h()+2*dx);
            }

            win->remove(out_read);
            win->remove(out_read_box);

        }

    }

    draw_box->redraw();
    out_data_box->redraw();
    graph_box->redraw();

    oo->redraw();
    win->redraw();
}



void base::save_screen_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->save_screen_cb();
}


void base::save_screen_cb(){

    root.capture_screen = 1;

    root.calculating=LOADING;
    Fl::add_idle(draw->idle_draw,this);
    calc_message_on();
    test_thread = std::thread(&base::save_screen_wait,this);
    test_thread.detach();
}


void* base::save_screen_wait(void *data){
     
    base* program = static_cast<base*> (data);

    while(program->root.capture_screen==1){
       std::this_thread::sleep_for (std::chrono::milliseconds(10)); // need something to stop compiler optimising empty while loop away...
    }

    if (program->root.capture_screen==0){

        Fl::awake(program->save_screen,program);
    }
    else{

        program->push_time();
        std::stringstream s;
        s<<"Error during screenshot.";
        s<<"\n";
        program->push_output(s.str().c_str());  
    }

    Fl::remove_idle(program->draw->idle_draw,program);
    program->root.calculating=FREE;
    program->calc_message_off();
  
   
    return NULL;
}


void base::save_screen(void *data){
    

    base* program = static_cast<base*> (data);

    Fl_Native_File_Chooser fnfc;
    fnfc.title("Capture screen");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    fnfc.filter("*.png");
    fnfc.preset_file("screenshot.png");
    fnfc.options(Fl_Native_File_Chooser::SAVEAS_CONFIRM);
    if ( fnfc.show() ) return;
    
    std::string str=std::string(fnfc.filename());
    

    std::vector<unsigned char> output;

    for (unsigned int i=0;i<program->root.screen_cap.size();i++){
        output.push_back(static_cast<unsigned char>(255*program->root.screen_cap[i]));
    }

    stbi_flip_vertically_on_write(1);
    int success = stbi_write_png(str.c_str(), program->root.screen_w, program->root.screen_h, 3, output.data(),0);

    if (success){

        program->push_time();
        std::stringstream s;
        s<<"Screen captured and exported to ";
        s<<str.c_str();
        s<<"\n";
        program->push_output(s.str().c_str());
    }
    else{
        
        program->push_time();
        std::stringstream s;
        s<<"Error during screenshot.";
        s<<"\n";
        program->push_output(s.str().c_str());
    }

    
}


void base::calc_message_on(){
    Fl::lock();
    warning_visibility->hide();
    warning_map->hide();
    calc_message->show();
    Fl::unlock();
    //
}

void base::calc_message_off_cb(void* data){
    base* program = static_cast<base*> (data);
    program->calc_message_off();
}

void base::calc_message_on_cb(void* data){
    base* program = static_cast<base*> (data);
    program->calc_message_on();
}

void base::calc_message_off(){
    Fl::lock();
    calc_message->hide();
    if (!root.current->vis_valid){ 
        warning_visibility->show();
        show_visibility->value(0);
    }
    if (!root.current->map_valid) warning_map->show();
    //Fl::awake();
    Fl::unlock();   
   // 
}


void base::toggle_deskmodel_cb_static(Fl_Widget*, void* data){
    base* program = static_cast<base*> (data);
    container* root = &(program->root);
    if (root->desk_model){
        root->desk_model=0;
        program->push_time();
        program->push_output("Drawing origin markers as 3D desk model off.\n");
    }
    else{
        root->desk_model=1;
        program->push_time();
        program->push_output("Drawing origin markers as 3D desk model on.\n");
    }

    program->draw->redraw();
}


void base::toggle_segments_cb_static(Fl_Widget*, void* data){
    base* program = static_cast<base*> (data);
    int &toggle = program->root.current->use_segments;    
    if (!program->edges_button->value()){
        toggle=0;
        program->push_time();
        program->push_output("Disabling edge based analysis.\n");        
    }
    else{
        toggle=1;
        program->push_time();
        program->push_output("Enabling edge based analysis.\n");        
    }
    program->metric_cb();
}

void base::toggle_turns_cb_static(Fl_Widget*, void* data){
    base* program = static_cast<base*> (data);
    int &toggle = program->root.current->use_turns;    
    if (!program->turns_button->value()){
        toggle=0;
        program->push_time();
        program->push_output("Disabling angular deviation/turn based analysis.\n");
        program->turn_ang->deactivate();
        program->turn_ang_ok->deactivate();
    }
    else{
        toggle=1;
        program->push_time();
        program->push_output("Enabling angular deviation/turn based analysis.\n");
        program->turn_ang->activate();
        program->turn_ang_ok->activate();
    }
    program->metric_cb();
}



void base::rebuild_widgets(){

    std::vector<double>& floor_index = root.current->floor_index;    
    //THREAD_STATE& calculating = root.calculating;

    minimum_reset_fields();

    if (floor_choice->value()>0){
        add_floor->activate();
        zfloor->activate();
        zfloor_ok->activate();
        activate_plan_edit();
        if (static_cast<int>(floor_index.size())>1){
            remove_floor->activate();
        }
        else{
            remove_floor->deactivate();
        }

        if (mark_locations_choice->value()>0){
            activate_main_edit();
            if (mark_locations_choice->value()<10){
                location_label->deactivate();
                location_label_in->deactivate();
                overwrite_location_point->deactivate();
                point_in->deactivate();
            }
        }
        else{
            deactivate_main_edit();
        }

    }
    else{
        add_floor->deactivate();
        remove_floor->deactivate();
        zfloor->deactivate();
        zfloor_ok->deactivate();
        deactivate_plan_edit();
        deactivate_main_edit();
    }


    if (plot_choice->value()){
        
        for (int i=0;i<4;i++){
            
            bin_count[i]->activate();
        }
        
        hist_building[3]->activate();
        hist_metric[3]->activate();
        
        hist_interp_choice->activate();
        yaxis_choice->activate();
        show_regression->deactivate();
        
    }
    else{
        for (int i=0;i<4;i++){
            
            bin_count[i]->deactivate();
        }
        
        hist_building[3]->deactivate();
        hist_metric[3]->deactivate();
        
        hist_interp_choice->deactivate();
        yaxis_choice->deactivate();
        show_regression->activate();
        
    }


}

void base::activate_main_edit(){
    location_label->activate();
    location_label_in->activate();
    overwrite_location_point->activate();
    point_in->activate();
    clear_selected_location->activate();
    clear_last_location->activate();
    clear_locations->activate();
}

void base::deactivate_main_edit(){
    location_label->deactivate();
    location_label_in->deactivate();
    overwrite_location_point->deactivate();
    point_in->deactivate();
    clear_selected_location->deactivate();
    clear_last_location->deactivate();
    clear_locations->deactivate();
}

void base::activate_plan_edit(){

    clear_dxf_button->activate();
    load_dxf_button->activate();
    xshift->activate();
    yshift->activate();
    xshift_ok->activate();
    yshift_ok->activate();
    scale_floorplan->activate();
    scale_floorplan_ok->activate();
    layers_button->activate();
    blocks_button->activate();

}

void base::deactivate_plan_edit(){
    
    clear_dxf_button->deactivate();
    load_dxf_button->deactivate();
    xshift->deactivate();
    yshift->deactivate();
    xshift_ok->deactivate();
    yshift_ok->deactivate();
    scale_floorplan->deactivate();
    scale_floorplan_ok->deactivate();
    layers_button->deactivate();
    blocks_button->deactivate();
}

void base::two_element_deselect(){

    draw->start_link=-1;
    draw->end_link=-1;
    draw->set_first=0;

}

void base::floorpoly_reset(){

    std::vector<std::vector<vec> >& floorarea_polygons = root.current->floorarea_polygons;
    if (floorarea_polygons.size()>0){
        floorarea_polygons.pop_back();
    }
    floorarea_polygons.push_back(std::vector<vec>(0));

}

void base::turn_def_cb_static(Fl_Widget*,void* data){
    base* program = static_cast<base*> (data);
    program->turn_def_cb();
}

void base::turn_def_cb(){

    if (root.calculating){
        calc_cb();
        return;
    }

    std::stringstream s;
    s<<turn_ang->value();
    std::string str1;
    str1 = s.str();
    char * e;
    errno = 0;
    double x = -1.0;

    try{
        x = std::strtod(str1.c_str(), &e);
    }
    catch(...){
        push_time();
        push_output("Error reading in turn definition.\n");      
        return;   
    }

    if (*e != '\0' ||  // error, we didn't consume the entire string
        errno != 0 )   // error, overflow or underflow
    {
        push_time();
        push_output("Error reading in turn definition.\n");      
        return;  
    }

    if ((x<0)||(x>360)){
        push_time();
        push_output("Error reading in turn definition. Value must be between 0 and 360.\n");      
        std::stringstream s;
        s<<std::fixed<<std::setprecision(2)<<root.current->turn_definition;
        turn_ang->value(s.str().c_str());
        return;
    }

    root.current->turn_definition = x;
    push_time();
    s.str("");
    s<<"Turn definition for project "<<building_choice->value()+1<<" set to "<<std::fixed<<std::setprecision(4)<<x<<" degrees"<<std::endl;
    push_output(s.str());

    base::show_warning_map_cb(this);
    
}

void base::push_move_status(int set){

    std::stringstream s;
    if (set){
        s<<"Move mode/Alt lock on."<<std::endl;
    }
    else{
        s<<"Move mode/Alt lock off."<<std::endl;   
    }
    push_time();
    push_output(s.str());
}

void base::push_output(std::string str){
    
    Fl::lock();
    output_mutex.lock();
    text->buff->append(str.c_str());
    text->display->buffer(text->buff);
    text->display->insert_position(text->buff->line_start(text->buff->length()));//moves insert position to the character corresponding to the start of the line that the last character is on
    text->display->show_insert_position(); //scrolls to that point
    output_mutex.unlock();
    text->display->redraw();
    Fl::unlock();
    Fl::awake();
    
}

void base::push_output_no_lock(std::string str){
    

    output_mutex.lock();
    text->buff->append(str.c_str());
    text->display->buffer(text->buff);
    text->display->insert_position(text->buff->line_start(text->buff->length()));//moves insert position to the character corresponding to the start of the line that the last character is on
    text->display->show_insert_position(); //scrolls to that point
    output_mutex.unlock();
    text->display->redraw();
    
}

void base::clear_output(){
    
    Fl::lock();    
    text->buff->remove(0,text->buff->length());
    text->display->buffer(text->buff);
    text->display->redraw();
    
    Fl::unlock();
    Fl::awake();
    
}

void base::push_time_no_lock(){

    time_t t = time(NULL);
    tm* tt=localtime(&t);
    std::stringstream s;
    s<< std::setfill('0')<<std::setw(2)<<tt->tm_mday<<"/" <<std::setw(2)<<tt->tm_mon+1<<"/"<< std::setw(4)<<tt->tm_year+1900<<" "<< std::setw(2)<<tt->tm_hour<<":"<<std::setw(2)<<tt->tm_min<<":"<<std::setw(2)<<tt->tm_sec<<" >> ";
    push_output_no_lock(s.str());

}

void base::push_time(){

    time_t t = time(NULL);
    tm* tt=localtime(&t);
    std::stringstream s;
    s<< std::setfill('0')<<std::setw(2)<<tt->tm_mday<<"/" <<std::setw(2)<<tt->tm_mon+1<<"/"<< std::setw(4)<<tt->tm_year+1900<<" "<< std::setw(2)<<tt->tm_hour<<":"<<std::setw(2)<<tt->tm_min<<":"<<std::setw(2)<<tt->tm_sec<<" >> ";
    push_output(s.str());

}

// void base::push_time(void* data){

//     base* program = static_cast<base*> (data);
//     time_t t = time(NULL);
//     tm* tt=localtime(&t);
//     std::stringstream s;
//     s<< std::setfill('0')<<std::setw(2)<<tt->tm_mday<<"/"<<
//         std::setw(2)<<tt->tm_mon+1<<"/"<< std::setw(4)<<tt->tm_year+1900<<" "<< 
//         std::setw(2)<<tt->tm_hour<<":"<<std::setw(2)<<tt->tm_min<<":"<<std::setw(2)<<tt->tm_sec<<" >> ";
//     program->output_mutex.lock();
//     program->output_string = s.str();
//     program->output_mutex.unlock();
//     program->push_output_awake(program);

// }

void base::push_output_awake(void* data){
    
    base* program = static_cast<base*> (data);
    program->output_mutex.lock();
    program->text->buff->append(program->output_string.c_str());
    program->text->display->buffer(program->text->buff);
    program->text->display->insert_position(program->text->buff->line_start(program->text->buff->length()));//moves insert position to the character corresponding to the start of the line that the last character is on
    program->text->display->show_insert_position(); //scrolls to that point
    program->output_mutex.unlock();
    program->text->display->redraw();
    Fl::check();
}

void base::reset_console_buffer(void* data){
    
    base* program = static_cast<base*> (data);
    program->text->buff->remove(program->buffer_point,program->text->buff->length());
    
}

void base::reset_console_buffer_and_push(void* data){
    
    base* program = static_cast<base*> (data);

    program->output_mutex.lock();
    program->text->buff->remove(program->buffer_point,program->text->buff->length()); //buffer_point and output_string are protected, everything else only used by main thread
    program->text->buff->append(program->output_string.c_str());
    program->output_mutex.unlock();

    program->text->display->buffer(program->text->buff);
    program->text->display->insert_position(program->text->buff->line_start(program->text->buff->length()));//moves insert position to the character corresponding to the start of the line that the last character is on
    program->text->display->show_insert_position(); //scrolls to that point
    program->text->display->redraw();

    program->output_mutex.lock();
    program->finished_console_output=1;
    program->output_mutex.unlock();

    Fl::check();
    
}

void base::push_time_static(void * data){

    base* program = static_cast<base*> (data);
    program->push_time();
    program->push_output("ok\n");
    Fl::repeat_timeout(1.0,push_time_static,program); 
}

void base::add_console_updater(void* data){
    base* program = static_cast<base*> (data);
    
    program->output_mutex.lock(); 
    program->finished_console_output = 0;
    program->continue_console_output = 1;
    program->buffer_point=program->text->buff->length();
    program->calc_progress.reset();
    program->old_calc_progress= 0.0;
    program->output_mutex.unlock();

    program->last_calc_time = boost::posix_time::microsec_clock::local_time();
    program->calc_start_time = program->last_calc_time;
    program->progress_history.clear();
    program->time_history.clear();
    Fl::add_timeout(0.0,program->reset_console_buffer_and_push2,program);    
}

void base::reset_console_buffer_and_push2(void* data){

    base* program = static_cast<base*> (data);
    
    double progress = 0;
    int continue_output = 0;
    
    if (program->calc_progress.size()){
        progress = program->calc_progress.get_min();
    }
    else{
        progress = 0.0;
    }

    program->output_mutex.lock();
    continue_output = program->continue_console_output;
    program->output_mutex.unlock();

    if (!continue_output){
        program->output_mutex.lock();
        program->finished_console_output = 1;
        program->output_mutex.unlock();
        return;
    }

    boost::posix_time::ptime calc_time = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration dur = calc_time - program->last_calc_time;
    double dt = dur.total_milliseconds();
    double time_left=0;
    

    if (dt>1000){

        double percent_left= (100-progress);
        program->progress_history.push_back(progress);
        program->time_history.push_back(calc_time);
        if (program->progress_history.size()>60){
            program->progress_history.pop_front();
            program->time_history.pop_front();
        }
        boost::posix_time::time_duration dur_av = program->time_history.back()-program->time_history.front();
        double dt_av = dur_av.total_milliseconds();
        double mean_rate = (program->progress_history.back()-program->progress_history.front())/(0.001*dt_av);
        
        time_left = percent_left/mean_rate;

        program->old_calc_progress=progress;
        program->old_time_left = time_left;
        program->last_calc_time = calc_time;

    }
    else{
        time_left = program->old_time_left;
    }

    boost::posix_time::time_duration dur2 = calc_time - program->calc_start_time;
    double dt2 = dur2.total_milliseconds();
    
    std::stringstream output;
    output.precision(3);
    output.setf( std::ios::fixed, std:: ios::floatfield );
    if (program->root.killthread.get()){
        output<<"------------------- >> Cancelling..."<<std::endl;
    }
    else if ((dt2 > 2500)&&(progress>1e-10)){
        boost::posix_time::time_duration td = boost::posix_time::seconds(static_cast<int>(std::round(0.001*dt2)));
        boost::posix_time::time_duration td1 = boost::posix_time::seconds(static_cast<int>(std::round(time_left)));
        output<<"------------------- >> "<<progress<<"% calculated"<<" - "<< td.hours()<<":"<<std::setw(2) << std::setfill('0') <<td.minutes()<<":"<<std::setw(2) << std::setfill('0') <<td.seconds() <<" elapsed. Estimating "<<td1.hours()<<":"<<std::setw(2) << std::setfill('0') <<td1.minutes()<<":"<<std::setw(2) << std::setfill('0') <<td1.seconds()<<" remaining."<<std::setfill(' ') <<std::endl;
    }
    else{
        output<<"------------------- >> "<<progress<<"% calculated"<<std::endl;   
    }

    program->text->buff->remove(program->buffer_point,program->text->buff->length()); //buffer_point and output_string are protected, everything else only used by main thread
    program->text->buff->append(output.str().c_str());
    program->text->display->buffer(program->text->buff);
    program->text->display->insert_position(program->text->buff->line_start(program->text->buff->length()));//moves insert position to the character corresponding to the start of the line that the last character is on
    program->text->display->show_insert_position(); //scrolls to that point
    program->text->display->redraw();

    
    Fl::add_timeout(0.12,reset_console_buffer_and_push2,program); 
    
    
}

void base::wait_for_console(){
    while (1){
        int i = 0;        
        output_mutex.lock();
        i = continue_console_output;
        output_mutex.unlock();
        if (i){
            break;
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(100)); // allow last timeout to finish
    }
}

void base::stop_console_output(){ //can be called multiple times without harm
    output_mutex.lock();
    continue_console_output=0;
    output_mutex.unlock();

    while (1){
        int i = 0;        
        output_mutex.lock();
        i = finished_console_output;
        output_mutex.unlock();
        if (i){
            break;
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(100)); // allow last timeout to finish
    }
}

void base::awake_map_warning_off(void* data){
    base* program = static_cast<base*> (data);
   if (program->warning_map) program->warning_map->hide();
}

void base::awake_vis_warning_off(void* data){
    base* program = static_cast<base*> (data);
    program->warning_visibility->hide();
}

void base::awake_map_warning_on(void* data){
    base* program = static_cast<base*> (data);
    program->warning_map->show();
}

void base::awake_vis_warning_on(void* data){
    base* program = static_cast<base*> (data);
    program->warning_visibility->show();
    program->show_visibility->value(0);
}


void base::close_route_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->close_route_cb();
}

void base::close_route_cb(){

    route_display->hide();
}


void base::show_ang_win_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->show_ang_win_cb();
}

void base::show_ang_win_cb(){
    set_ang_win->show();
    set_ang_win->resize(win->x()+0.5*win->w()-120,win->y()+0.5*win->h()-80,240,70);
    Fl::focus(ang_input);
}

void base::set_ang_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->set_ang_cb();
}

void base::set_ang_cb(){
            
    std::stringstream s;
    s<<ang_input->value();
    std::string str1;
    str1 = s.str();
    char * e;
    errno = 0;
    double x = std::strtod(str1.c_str(), &e);

    if (*e != '\0' ||  // error, we didn't consume the entire string
        errno != 0 )   // error, overflow or underflow
    {
        push_time();
        push_output("Error reading in angle.\n");
        return;
    }

    draw->ang_snap = x* (M_PI/180.0);
    s.str("");
    s<<"Guide angle set to "<<x<<" degrees."<<std::endl;
    push_time();
    push_output(s.str());
    set_ang_win->hide();
    //Fl::focus(this->win);
    draw->redraw();

}

void base::close_ang_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->close_ang_cb();
}

void base::close_ang_cb(){
    set_ang_win->hide();
    //Fl::focus(this->win);
    draw->redraw();
}


void base::kill_thread_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->kill_thread();
}

void base::kill_thread() {

    container* root = &(this->root);
    
    //if ((root->calculating)&&(root->killthread==0)){
    if (root->calculating){
        
        //thread states that actually are "cancellable"
        if ((root->calculating == FLOORPLAN_CONVERT)||(root->calculating == CALC_STRUCTURES)
            ||(root->calculating == CALC_VIS)||(root->calculating == CALC_METRIC)
            ||(root->calculating == CALC_LINES)||(root->calculating == CHECKING_MAP)
            ||(root->calculating == MAP_CHECK))
        {

            //root->killthread=1;  
            root->killthread.kill();
            push_time_no_lock();
            push_output_no_lock("Cancelling...\n");
        }
    }
}


void base::show_warning_map_start(void* data){
    
    base* program = static_cast<base*> (data);
    //program->draw->show_warning_map();
    program->show_warning_map();
}

void base::show_warning_vis_start(void* data){
    
    base* program = static_cast<base*> (data);
    
    //program->draw->show_warning_visibility();
    program->show_warning_visibility();
    program->show_visibility->value(0);
    
}

void base::show_warning_map_cb(void* data){
    
    base* program = static_cast<base*> (data);
    //program->draw->show_warning_map();
    program->show_warning_map();

}

void base::show_warning_vis_cb(void* data){
    
    base* program = static_cast<base*> (data);
    //program->draw->show_warning_visibility();
    program->show_warning_visibility();
}

void base::warning_map_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->warning_map_cb();
}


void base::warning_map_cb() {
    
    container* root = &(this->root);
    root->current->route.clear();
    show_warning_map();
}

void base::show_warning_visibility(){
        
    if (root.current->vis_valid){
        push_time();
        push_output("WARNING: Visibility constructs may have changed.\n------------------- >> Displaying/using visibility in metrics will incur additional computation steps.\n");
    }

    root.current->vis_valid=0;
    show_visibility->value(0);    
    if (!calc_message->visible()){
        warning_visibility->show();
    }
    warning_visibility->redraw();
    warning_visibility->parent()->redraw();
    win->redraw();

    metrics_mutex.lock();

    root.current->plot_metric.clear();
    root.current->max_plot_metric = 0.0;
    root.current->min_plot_metric = 0.0;

    metrics_mutex.unlock();
    
}

void base::show_warning_map(){
    
    if ((root.current->map_valid)||(root.current->lines_valid)){
    
        push_time();    
        push_output("WARNING: Spatial graph may have changed.\n------------------- >> Subsequent route/metric calculation will incur additional computation steps.\n");
        
    }

    root.current->map_valid=0;
    root.current->lines_valid=0;
    root.current->segs_valid=0;
    root.current->turns_valid=0;
   
    if (!calc_message->visible()){
        warning_map->show();
    }
    warning_map->redraw();
    warning_map->parent()->redraw();
    win->redraw();

    metrics_mutex.lock();

    root.current->plot_metric.clear();
    root.current->max_plot_metric = 0.0;
    root.current->min_plot_metric = 0.0;
    root.current->route.clear();

    metrics_mutex.unlock();

    draw->redraw();

}

void base::reset_view_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->reset_view();
}

void base::reset_view() {

    draw->first_draw=1;
    draw->redraw();
    plot->ang_x=0;
    plot->ang_y=0;
    push_time();
    push_output("Resetting view.\n");
    plot->redraw();
    
}

void base::colour_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->colour_cb();
}

void base::colour_cb() {
    
    container* root = &(this->root);
    

    Fl_Color dark_back = fl_rgb_color(56,56,56);
    Fl_Color dark_text = fl_rgb_color(235,235,235);
    
    
    dark_back = fl_rgb_color(70,70,70);


    if (root->bow){

        // out_read->textcolor(fl_rgb_color(210,210,210));
        // out_read->color(fl_rgb_color(50,50,50));
        // out_data->textcolor(fl_rgb_color(210,210,210));
        // out_data->color(fl_rgb_color(50,50,50));

        out_read->textcolor(dark_text);
        out_read->color(dark_back);
        out_data->textcolor(dark_text);
        out_data->color(dark_back);
        out_data->redraw();

        out_guide->textcolor(dark_text);
        out_guide->color(dark_back);
        out_guide->redraw();

        root->bow=0;
        
        push_time();
        push_output("Switching to dark mode.\n");
        
        out_read_box->hide();
        draw_box->hide();
        out_data_box->hide();
        graph_box->hide();

    }
    else{
        
        out_read->color(7);
        out_read->textcolor(0);
        out_read->redraw();
        out_data->color(7);
        out_data->textcolor(0);
        out_data->redraw();
        out_guide->color(7);
        out_guide->textcolor(0);
        out_guide->redraw();
        
        
        root->bow=1;
        
        push_time();
        push_output("Switching to light mode.\n");
        
        out_read_box->show();
        draw_box->show();
        out_data_box->show();
        graph_box->show();
     
    }
    
    win->resize(win->x(),win->y(),win->w(),win->h());
    win->redraw();

    draw->redraw();
    plot->redraw();
    out_data->redraw();
    
}

void base::show_metric_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->show_metric_cb();
}

void base::show_metric_cb() {
    
    container* root = &(this->root);
    
    if (root->draw_metric){
        root->draw_metric=0;
    }
    else{
        root->draw_metric=1;
        draw->deselect();
    }
    
    draw->redraw();
    
    
}

void base::floor_ceil_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->floor_ceil_cb();
}

void base::floor_ceil_cb() {
    
    container* root = &(this->root);
    
    if (root->floor_ceil){
        root->floor_ceil=0;
        push_time();
        push_output("Floor/ceiling visualisation (free view) turned off.\n");
    }
    else{
        root->floor_ceil=1;
        push_time();
        push_output("Floor/ceiling visualisation (free view) turned on.\n");
    }
    
    draw->redraw();
    
    
}

void base::show_rot_axis_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->show_rot_axis_cb();
}

void base::show_rot_axis_cb() {
    
    container* root = &(this->root);
    
    if (root->show_rot_axis){
        root->show_rot_axis=0;
        push_time();
        push_output("3D rotation axis hidden/locked.\n");
    }
    else{
        root->show_rot_axis=1;
        push_time();
        push_output("3D rotation axis displayed and editable through 2D view.\n");
    }
    
    draw->redraw();
    
    
}



void base::floor_lock_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->floor_lock_cb();
}

void base::floor_lock_cb() {
    
    container* root = &(this->root);
    
    if (root->floor_lock){
        root->floor_lock=0;
        push_time();
        push_output("Floor lock (free view) turned off.\n");
    }
    else{
        root->floor_lock=1;
        floor_choice_cb(0);
        push_time();
        push_output("Floor lock (free view) turned on.\n");
    }
    
    draw->redraw();
    
    
}



void base::limit_edge_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->limit_edge_cb();
}

void base::limit_edge_cb() {
    
    container* root = &(this->root);
    
    if (root->limit_edge){
        root->limit_edge=0;
        push_time();
        push_output("Edge visualisation limit turned off.\n");
    }
    else{
        root->limit_edge=1;
        push_time();
        push_output("Edge visualisation limit turned on.\n");
    }
    
    draw->redraw();
    
    
}


void base::hide(Fl_Widget* widget){
    widget->deactivate();
}

void base::building_data_choice_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->building_data_choice_cb();
}

void base::building_data_choice_cb() {
    
    container* root = &(this->root);
    root->current_data = root->comp_buildings[building_data_choice->value()];
    redraw_data();
       
}

void base::building_choice_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->building_choice_cb();
}

void base::building_choice_cb() {
    
    static int old_val=0; //bad style? works well enough...
    
    container* root = &(this->root);
    
    
    if (!root->calculating){
        
        reset_fields();
         
    }
    else{
        building_choice->value(old_val);
        calc_cb();
    }
    
    old_val = building_choice->value();
        
}

void base::quit_callback(Fl_Widget*, void*) {
    
    quit_win->show();
    
}

void base::main_callback_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->main_callback();
}

void base::main_callback() {
        
    container* root = &(this->root);
    
    if (Fl::event()==FL_SHORTCUT && Fl::event_key()==FL_Escape){        
    
        return;//escape key calls main window callback apparenty
    }
    
    int i=0;
    if (!(root->calculating==SAVING)){

        i = choice("Confirm quit?\nIf saving remember to save the contents of every project slot in use.","Save","Cancel","Quit","Quit?");
    }
    
    
    if (i==2){
        exit(0);
    }
    else if (i==0){
        save_binary_cb();
    }
    
}

void base::idle_load_static(void* data) {
    base* program = static_cast<base*> (data);
    program->idle_load();

}

void base::idle_load(){

    container* root = &(this->root);
    
    static int shown =0;
    
    clock_t timer;   //non-boost
    timer=clock();  //non-boost
    double s = (timer-start_time)/static_cast<double>(CLOCKS_PER_SEC);
    
    if ((!draw->loaded_textures)&&(draw->is_valid())){
        draw->load_textures();
        draw->loaded_textures=1;
    }
    
    
    if (s>2.5){
        if (shown==0){
            win->show();
            shown=1;
        }
    }
    

    if ((draw->loaded_textures)&&(s>4)){
        push_output("done.\n");
        win2->hide();

        Fl::remove_idle(idle_load_static,this);

        //Fl::focus(this->win);
        push_time();
        push_output("Ready.\n");
        root->calculating=FREE;
        calc_message_off();
    }
}



void* base::showmain(){
    
    
    win->show();
    
    return NULL;
}

void* base::showsecond(){
    
    win2->show();
    return NULL;
}


void base::redraw_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->redraw();
}

void base::redraw(){    
    draw->redraw();
}


void base::redraw_from_thread(void* data){
    base* program = static_cast<base*> (data);
    program->win->redraw();
    program->draw->redraw();
    program->plot->redraw();
}

void base::check_from_thread(void * data){
    base* program = static_cast<base*> (data);
    program->win->redraw();
    Fl::check();
    Fl::flush();
}



void base::idle_static(void* data) {
    base* program = static_cast<base*> (data);
    program->idle();
}


void base::idle(){
    
    if  (three_d_choice->value()==3){
        draw->redraw();
    }
}


void base::redraw_show_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->redraw_show();
}


void base::redraw_show(){
    
    
    int reset=0;
    

    switch (mark_locations_choice->value()){
        case 1:
            if (!show_locations[0]->value()){
                if (!show_visibility->value()){ // if displaying visibility need to be able to keep selecting origin locations
                    reset=1;
                }
            }
            break;
        case 2:
            if (!show_locations[1]->value()){
                reset=1;
            }
            break;
        case 3:
            if (!show_locations[2]->value()){
                reset=1;
            }
            break;
        case 4:
            if (!show_locations[3]->value()){
                reset=1;
            }
            break;
        case 5:
            if (!show_locations[4]->value()){
                reset=1;
            }
            break;
        case 6:
            if (!show_locations[5]->value()){
                reset=1;
            }
            break;
        case 7:
            if (!show_locations[6]->value()){
                reset=1;
            }
            break;
        case 8:
            if (!show_locations[7]->value()){
                reset=1;
            }
            break;
        case 9:
            if (!show_locations[8]->value()){
                reset=1;
            }
            break;
        case 10:
            if (!show_walls_sitting->value()){
                reset=1;
            }
            break;
        case 11:
            if (!show_walls_standing->value()){
                reset=1;
            }
            break;
        case 12:
            if (!show_map->value()){
                reset=1;
            }

            break;
        case 13:
            
            if (!show_map->value()){
                reset=1;
            }
            
            break;
        case 14:
            if (!show_floor_areas->value()){
                reset=1;
            }
            break;
    }
    
    
    if (reset){
        mark_locations_choice->value(0);
        redraw_mark(1);
    }

    draw->redraw();
}


void base::scheme0_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);

    program->scheme0();
}
void base::scheme1_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->scheme1();
}
void base::scheme2_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->scheme2();
}
void base::scheme3_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->scheme3();
}

void base::scheme0(){
    
    Fl::scheme("none");
    
}
void base::scheme1(){
    
    Fl::scheme("gtk+");    
    
}
void base::scheme2(){
    
    Fl::scheme("plastic");
    
}

void base::scheme3(){
    
    Fl::scheme("gleam");
    
}


void base::rebuild_hist_input_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->rebuild_hist_input();
}


void base::rebuild_hist_input(){
    
    container* root = &(this->root);
    
    for (unsigned i=0;i<4;i++){
        
        int bd = hist_building[i]->value();
        
        if (   static_cast<int>(  (hist_metric[i]->size()-1)  ) !=  static_cast<int>( (root->comp_buildings[bd]->metrics.size()+1)  )  ){
            
           
            
            hist_metric[i]->clear();
            hist_metric[i]->add("-----");
            
            for (unsigned j=0;j<root->comp_buildings[bd]->metrics.size();j++){
                std::stringstream s;
                s<<j+1;
                hist_metric[i]->add(s.str().c_str());
            }
            hist_metric[i]->value(0);
        }
    }
    
    plot_choice_cb();

    plot->histogram();
    plot->redraw();
    
}


void base::minimum_reset_fields(){
    std::stringstream s;
    s<<std::fixed<<std::setprecision(2)<<root.current->turn_definition;
    turn_ang->value(s.str().c_str());
    if (floor_choice->value()>0){
        s.str("");
        s<<std::fixed<<std::setprecision(2)<<root.current->floor_plans[floor_choice->value()-1].shift_x;
        xshift->value(s.str().c_str());
        s.str("");
        s<<std::fixed<<std::setprecision(2)<<root.current->floor_plans[floor_choice->value()-1].shift_y;
        yshift->value(s.str().c_str());
        draw->zfp=root.current->floor_index[floor_choice->value()-1]+1.8;
    }


}

void base::soft_reset_fields(){ //for floor change to anyting but "all"


    std::stringstream s;
    s.str("");
    location_label->value(s.str().c_str());
    overwrite_location_point->value(s.str().c_str());
    minimum_reset_fields();


}

void base::reset_fields(){  //rebuild everything

    draw->deselect();

    floorpoly_reset();
    
    root.current = root.comp_buildings[building_choice->value()];
    draw->current = root.current;


    //Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[TOGGLE_SEGMENTS_INDEX]);

    if (root.current->use_segments){
        //temp->set();
        //calc_lines->activate();
        //progress_lines->activate();
        edges_button->value(1);
    }
    else{
        //temp->clear();
        //calc_lines->deactivate();
        //progress_lines->deactivate();
        edges_button->value(0);
    }    

    if (root.current->use_turns){
        
        turns_button->value(1);
    }
    else{
        
        turns_button->value(0);
    }   


    //prepare_undo();//really? - in construction of each building slot?
  
    reset_floor_choice();
    floor_choice->value(0);
    //floor_choice->do_callback(floor_choice,this);
    floor_choice_cb(0);
    draw->deselect();
    two_element_deselect();
    draw->redraw();
 
    soft_reset_fields();


    if (root.current->vis_valid){
        warning_visibility->hide();
    }
    else{
        warning_visibility->show(); 
        show_visibility->value(0);
    }
    
    if (root.current->map_valid){
        
        warning_map->hide();
    }
    else{
        
        warning_map->show();
        
    }


    win->redraw();


}

void base::main_tab_callback_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->main_tab_callback();
}



void base::main_tab_callback(){

    rebuild_hist_input();
    data_choice->value(0);
    redraw_data();
    tab_callback();
}


void base::tab_callback_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->tab_callback();
}

void base::tab_callback(){
        
    
    if (mark_locations_choice->value()){
        mark_locations_choice->value(0);
        redraw_mark(1);

        draw->deselect();
        two_element_deselect();
        floorpoly_reset();
        draw->redraw();

    }

    soft_reset_fields();
}


void base::no_editing(){

    if (mark_locations_choice->value()>0){
        mark_locations_choice->value(0);
    }
    push_time();
    push_output("No editing of constructs with calculations running!\n");
}


void base::go_fullscreen_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->go_fullscreen();
}

void base::go_fullscreen(){    

    if ((root.fullscreen==0)&&(oo->value()==o1)){
        xtemp = draw->x();
        ytemp = draw->y();
        wtemp = draw->w();
        htemp = draw->h();
        
        xtemp1 = win->x();
        ytemp1 = win->y();
        wtemp1 = win->w();
        htemp1 = win->h();
        
        
        draw->resize(0,0,win->w(),win->h());
        
        o1->remove(draw);//put draw window "on top"
        win->add(draw);
        
        root.fullscreen=1;
    }
    else if (oo->value()==o1)
    {

        if (!root.resized){
            draw->resize(xtemp,ytemp,wtemp,htemp);
            win->resize(xtemp1,ytemp1,wtemp1,htemp1);
        }
        else{
            draw->resize(xtemp,ytemp,wtemp,htemp);
            win->resize(win->xtemp,win->ytemp,win->wtemp,win->htemp);

        }
        
        win->remove(draw); //put draw window "back in place"
        o1->add(draw);
                
        root.resized = 0;
        root.fullscreen=0;

        //total hack
        info_bar_cb();
        info_bar_cb();
        panel_cb();
        panel_cb();
    }
}


void base::about_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    
    program->about_cb();

}

void base::about_cb(){
    
    about_win->show();
    about_win->resize(win->x()+0.5*win->w()-165,win->y()+0.5*win->h()-125,330,250);

    
    Fl::flush();
}


void base::about_ok_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    
    program->about_ok_cb();
}


void base::about_ok_cb(){
    //Fl_Window* w = (Fl_Window*) data;
    
    about_win->hide();
    //Fl::focus(this->win);
    win->redraw();
    
    Fl::flush();
    
}


void base::calc_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    
    program->calc_cb();

}

void base::calc_cb(){
    
    calc_win->resize(win->x()+0.5*win->w()-150,win->y()+0.5*win->h()-125,300,150);
    calc_win->show();

    
    Fl::flush();
}



void base::calc_ok_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    
    program->calc_ok_cb();
}


void base::calc_ok_cb(){
    //Fl_Window* w = (Fl_Window*) data;
    
    calc_win->hide();
    //Fl::focus(this->win);
    win->redraw();
    
    Fl::flush();
    
}

void base::guide_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->guide_cb();
}
void base::guide_cb(){
    
    
    guide->show();
    guide->resize(win->x()+0.5*win->w()-300,win->y()+0.5*win->h()-265,600,530);
    
    
    
    Fl::flush();
}






void base::ok_guide_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->ok_guide_cb();
}

void base::ok_guide_cb(){

    guide->hide();
    //Fl::focus(this->win);
}



void base::show_route_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->show_route_cb();
}

void base::show_route_cb(){
    
    container* root = &(this->root);
    if (root->draw_route){
        root->draw_route=0;
    }
    else{
        root->draw_route=1;
    }
    draw->redraw();
    
}

void base::show_floorplan_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->show_floorplan_cb();
}

void base::show_floorplan_cb(){
    container* root = &(this->root);
    if (root->draw_floorplan){
        root->draw_floorplan=0;
        push_time();
        push_output("Hiding floorplans.\n");
    }
    else{
        root->draw_floorplan=1;
        push_time();
        push_output("Displaying floorplans.\n");
    }
    draw->redraw();
    
}

void base::show_grid_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->show_grid_cb();
}


void base::show_grid_cb(){
    container* root = &(this->root);
    if (root->draw_grid){
        root->draw_grid=0;
        push_time();
        push_output("Hiding grid.\n");
    }
    else{
        root->draw_grid=1;
        push_time();
        push_output("Displaying grid.\n");
    }
    draw->redraw();
    
}

void base::snap_to_point_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->snap_to_point_cb();
}


void base::snap_to_point_cb(){
    container* root = &(this->root);
    if (root->snap_to_point){
        root->snap_to_point=0;
        push_time();
        push_output("Snap to point priority off.\n");
    }
    else{
        root->snap_to_point=1;
        push_time();
        push_output("Snap to point priority on.\n");
    }
    draw->redraw();
    
}



void base::mark_transparent_walls_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->mark_transparent_walls_cb();
}


void base::mark_transparent_walls_cb(){
    container* root = &(this->root);
    if (root->mark_transparent_walls){
        root->mark_transparent_walls=0;
        push_time();
        push_output("Barriers will be added as opaque/floor areas will be added as positive areas.\n");
    }
    else{
        root->mark_transparent_walls=1;
        push_time();
        push_output("Barriers will be added as transparent/floor areas will be added as negative areas.\n");
    }
    draw->redraw();
        
}


void base::transparent_walls_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->transparent_walls_cb();
}


void base::transparent_walls_cb(){
    container* root = &(this->root);
    if (root->transparent_walls){
        root->transparent_walls=0;
        push_time();
        push_output("Displaying barriers according to their type (opaque vs. transparent, 3D only).\n");
    }
    else{
        root->transparent_walls=1;
        push_time();
        push_output("Displaying barriers as transparent (3D only).\n");
    }
    draw->redraw();
    
}

void base::points_label_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->points_label_cb();
}

void base::points_label_cb(){
    container* root = &(this->root);
    if (root->draw_points_label){
        root->draw_points_label=0;
        push_time();
        push_output("Hiding vertex indices.\n");
    }
    else{
        root->draw_points_label=1;
        push_time();
        push_output("Displaying vertex indices.\n");
    }
    draw->redraw();
    
}

void base::segments_label_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->segments_label_cb();
}

void base::segments_label_cb(){
    container* root = &(this->root);
    if (root->draw_segments_label){
        root->draw_segments_label=0;
        push_time();
        push_output("Hiding edge indices.\n");
    }
    else{
        root->draw_segments_label=1;
        push_time();
        push_output("Displaying edge indices.\n");
    }
    draw->redraw();
    
}

void base::locations_label_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->locations_label_cb();
}

void base::locations_label_cb(){
    container* root = &(this->root);
    if (root->draw_locations_label){
        root->draw_locations_label=0;
        push_time();
        push_output("Hiding location marker indices.\n");
    }
    else{
        root->draw_locations_label=1;
        push_time();
        push_output("Displaying location marker indices.\n");
    }
    draw->redraw();
    
}

void base::toggle_autoconnect_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->toggle_autoconnect_cb();
}

void base::toggle_autoconnect_cb(){
    
    push_time();
    

    container* root = &(this->root);
    if (root->autoconnect){
        push_output("Auto-connect for spatial graph vertices turned off.\n");
        root->autoconnect=0;
    }
    else{
        push_output("Auto-connect for spatial graph vertices turned on.\n");

        root->autoconnect=1;
    }
    
    draw->redraw();

}



void base::set_move_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->set_move_cb();
}

void base::set_move_cb(){
    
    draw->set_x_down();
    
    draw->redraw();
    
}

void base::color_scale_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->color_scale_cb();
}

void base::color_scale_cb(){
    container* root = &(this->root);
    
    if (root->color_scale){
        root->color_scale=0;
        push_time();
        push_output("Hiding metric colour scale.\n");
    }
    else{
        root->color_scale=1;
        push_time();
        push_output("Displaying metric colour scale.\n");
    }
    draw->redraw();
    
}

void base::show_scale_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->show_scale_cb();
}

void base::show_scale_cb(){
    container* root = &(this->root);
    if (root->show_scale){
        root->show_scale=0;
        push_time();
        push_output("Hiding length scale/orientation.\n");
    }
    else{
        root->show_scale=1;
        push_time();
        push_output("Displaying length scale/orientation.\n");
    }
    draw->redraw();
}





void base::plot_choice_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->plot_choice_cb();
}

void base::plot_choice_cb(){

    
    if (plot_choice->value()){
        
        for (int i=0;i<4;i++){
            
            bin_count[i]->activate();
            hist_building[i]->activate();
            hist_metric[i]->activate();
        }

        hist_interp_choice->activate();
        yaxis_choice->activate();
        show_regression->deactivate();
        
    }
    else{
        for (int i=0;i<4;i++){
            
            bin_count[i]->deactivate();
            hist_building[i]->deactivate();
            hist_metric[i]->deactivate();
        }
        
        hist_building[0]->activate();
        hist_metric[0]->activate();

        hist_building[1]->activate();
        hist_metric[1]->activate();

        if ((hist_metric[0]->value())&&(hist_metric[1]->value())){
            hist_building[2]->activate();
            hist_metric[2]->activate();
        }
        
        hist_interp_choice->deactivate();
        yaxis_choice->deactivate();
        show_regression->activate();
        
    }
    
    
    int count=0,count1=0;;
    
    for (unsigned int i=0;i<4;i++){
        
        if (hist_metric[i]->value()){ 
            count++;
            if (i<3){
                count1++;
            }
        }
    }
    
    if (((count<2)&&(plot_choice->value()))||((count1<3)&&(!plot_choice->value()))){
        plot->ang_x=0;
        plot->ang_y=0;
        plot->ang_z=0;
    }

    plot->histogram();
    
    plot->redraw();
    
}



void base::reset_floor_choice(){
    container* root = &(this->root);
    
    std::vector<double>& floor_index = root->current->floor_index;

    
    floor_choice->clear();
    floor_choice->add("All");
    
    std::stringstream s;
    for (unsigned int i=0;i<floor_index.size();i++){
        s.str("");
        s<<i;
        floor_choice->add(s.str().c_str());
    }
    

    {
        std::stringstream s;
        s<<std::fixed<<std::setprecision(2)<<floor_index[0];
        zfloor->value(s.str().c_str());
    }
    floor_choice->value(0);
    floor_choice_cb(0);
    
    zfloor->deactivate();
    zfloor_ok->deactivate();
}


void base::set_floor_choice(){ 
    
    container* root = &(this->root);
    std::vector<vec>* points = &(root->current->points);
    
    std::vector<plan>& floor_plans = root->current->floor_plans;
    std::vector<double>& floor_index = root->current->floor_index;
    std::vector<double>& floor_areas = root->current->floor_areas;
    
    
    
    floor_choice->clear();

    floors_mutex.lock();

    floor_index.clear();
    floor_plans.clear();

    floors_mutex.unlock();

    areas_mutex.lock();
    
    floor_areas.clear();

    areas_mutex.unlock();
    
    floor_choice->add("All");

    int max=0;
    for (unsigned int i=0;i<points->size();i++){
        if ((*points)[i].floor>max) max=(*points)[i].floor;
    }
    
    std::stringstream s;
    for (int i=0; i<=max;i++){
        s.str("");
        s<<i;
        floor_choice->add(s.str().c_str());
        
        plan temp;

        floors_mutex.lock();
        plans_mutex.lock();

        floor_plans.push_back(temp);
        double k;
        k=0;
        floor_index.push_back(k);
        floor_areas.push_back(0);
        
        
        plans_mutex.unlock();
        floors_mutex.unlock();

    }
    floor_choice->value(0);
    floor_choice_cb(0);
    
    zfloor->deactivate();
    zfloor_ok->deactivate();
    //zfloor_label->deactivate(); //floor change

    
    floors_mutex.lock();

    for (unsigned int i=0;i<points->size();i++){
        floor_index[(*points)[i].floor]=(*points)[i].z;
    }

    floors_mutex.unlock();
    
    for (unsigned int i=0;i<floor_index.size();i++){
        if (i==0){
            {
            std::stringstream s;
            s<<std::fixed<<std::setprecision(2)<<floor_index[i];
            zfloor->value(s.str().c_str());
            }
        }

    }        
    
}


void base::three_d_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->three_d_cb();
}
void base::three_d_cb(){
    
    container* root = &(this->root);
    std::vector<double>& floor_index = root->current->floor_index;
    
    
    if (three_d_choice->value()>1){
       // Fl::add_idle(idle,data);
    }
    else{
        Fl::remove_idle(draw->idle_draw,this); //just in case
    }
    
    
    draw->drag_point=-1;
    
    if(three_d_choice->value()){
        
        mark_locations_choice->value(0);
        redraw_mark(1);
        
        if (three_d_choice->value()==3){
            
            if (floor_choice->value()==0){
                floor_choice->value(1);
                floor_choice_cb(0);
            }
            
            draw->ang_xfp=-90;
            draw->ang_zfp=0;
            if (floor_choice->value()-1>=0){
                draw->zfp=floor_index[floor_choice->value()-1]+1.8;
            }
            else{
                draw->zfp=floor_index[0]+1.8;
            }

            draw->xfp = root->current->mid_x;
            draw->yfp = root->current->mid_y;

            draw->new_zoom=1;
            draw->zoom=1;

            if (!root->desk_model){

                Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[DESKMODEL_INDEX]);   
                temp->do_callback(menu,this);
                temp->set(); 

            }

        }

    }
    else{

        if (root->desk_model){
            Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[DESKMODEL_INDEX]);   
            temp->do_callback(menu,this);
            temp->clear(); 
        }
    }
    
    
    draw->redraw();
    
}

void base::floor_choice_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->floor_choice_cb(0);
}

void base::floor_choice_cb(int in_undo){
    
    container* root = &(this->root);
    std::vector<double>& floor_index = root->current->floor_index;
    
    if(!floor_choice->value()){

        draw->deselect();
        two_element_deselect();
        floorpoly_reset();

        zfloor->deactivate();
        zfloor_ok->deactivate();

        add_floor->deactivate();
        remove_floor->deactivate();
        mark_locations_choice->value(0);
        redraw_mark(1);
        minimum_reset_fields();

        deactivate_plan_edit();
        
    }
    else{

        draw->deselect_ex_floor(); // deselect minus first edge choice

        if(!in_undo){
            floorpoly_reset();
        }

        {
            std::stringstream s;
            s<<std::fixed<<std::setprecision(2)<<floor_index[floor_choice->value()-1];
            zfloor->value(s.str().c_str());
        }
        zfloor->activate();
        zfloor_ok->activate();
        if (mark_locations_choice->value()==0){// match in mark locations cb 


            add_floor->activate();
            
            if (static_cast<int>(floor_choice->size())<4){   
                remove_floor->deactivate();
            }
            else{                
                remove_floor->activate();
            }
        }
        minimum_reset_fields();
        activate_plan_edit();
    }
    

    soft_reset_fields();

    draw->snapoff();
    draw->redraw();
    
}

void base::redraw_mark_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->redraw_mark(0);
}

void base::redraw_mark(int from_calc){
    
    container* root = &(this->root);
    THREAD_STATE& calculating = root->calculating;
    
    static int old_val=0;
    
    floorpoly_reset();    
    
    if ((calculating==FREE)||((calculating==CALC_STRUCTURES)&&((mark_locations_choice->value()<=11)||(mark_locations_choice->value()==14)))||((calculating==CALC_VIS)&&(mark_locations_choice->value()>11))){
        

        if (mark_locations_choice->value()){
        
            if (root->draw_metric){

                Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[SHOW_METRIC_INDEX]);                
                temp->do_callback(menu,this);
                temp->clear();
                
            }
            
        }
        

        if ((!mark_locations_choice->value())||( (mark_locations_choice->value()>9) && (mark_locations_choice->value()<12) ) || (mark_locations_choice->value()>12)){

            if (draw->is_x_down()){

                Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[MOVE_MODE_INDEX]);
                temp->do_callback(menu,this);
                temp->clear();

            }
            
        }


        if ((old_val!=mark_locations_choice->value())||(!mark_locations_choice->value())){ //deselect if changing item //TRIAL: allow continuous selecting if edit off: ie in 3d modes
            
            draw->deselect();
            two_element_deselect();
            floorpoly_reset();
            draw->redraw();
        }
        
        
        
        if (mark_locations_choice->value()==0){
            
            location_label_in->deactivate();
            point_in->deactivate();
            clear_selected_location->deactivate();
            clear_last_location->deactivate();
            clear_locations->deactivate();
            overwrite_location_point->deactivate();
            location_label->deactivate();


            if (floor_choice->value()==0){
                zfloor->deactivate();
                zfloor_ok->deactivate();
                add_floor->deactivate();
                remove_floor->deactivate();
            }
            else{
                zfloor->activate();
                zfloor_ok->activate();
                add_floor->activate();


                if (static_cast<int>(floor_choice->size())<4){  
                    // floor choice = { All , 0 , 1 ,.... "list terminator"};
                    // so size of three = {All, 0, terminator} which is where floor deletion is not allowed.  
                    remove_floor->deactivate();
                }
                else{                    
                    remove_floor->activate();
                }
            }

            

        }        
        else{

            if ((mark_locations_choice->value()<=9)&&(mark_locations_choice->value()>0)){
                overwrite_location_point->activate();
                point_in->activate();
            }
            else{
                overwrite_location_point->deactivate();
                point_in->deactivate();
            }

            if (mark_locations_choice->value()==1){
                location_label->activate();
                location_label_in->activate();
            }
            else{
                location_label->deactivate();
                location_label_in->deactivate();
            }



            clear_last_location->deactivate();
            clear_last_location->activate();
            clear_selected_location->activate();
            
            clear_locations->activate();
            
            if (floor_choice->value()==0){
                floor_choice->value(1);
                floor_choice_cb(0);
            }

            zfloor->deactivate();
            zfloor_ok->deactivate();
            add_floor->deactivate();
            remove_floor->deactivate();
            

            if (three_d_choice->value()){
                three_d_choice->value(0);
                three_d_choice->do_callback(three_d_choice,this);
            }
            
        }
        
        if (mark_locations_choice->value()==1){
            show_locations[0]->value(1);
           
            
        }
        if (mark_locations_choice->value()==2){
            show_locations[1]->value(1);
        }
        if (mark_locations_choice->value()==3){
            show_locations[2]->value(1);
        }
        if (mark_locations_choice->value()==4){
            show_locations[3]->value(1);
        }
        if (mark_locations_choice->value()==5){
            show_locations[4]->value(1);
        }
        if (mark_locations_choice->value()==6){
            show_locations[5]->value(1);
        }
        if (mark_locations_choice->value()==7){
            show_locations[6]->value(1);
        }
        if (mark_locations_choice->value()==8){
            show_locations[7]->value(1);
        }
        if (mark_locations_choice->value()==9){
            show_locations[8]->value(1);
        }
        if (mark_locations_choice->value()==10){
            show_walls_sitting->value(1);
        }
        if (mark_locations_choice->value()==11){
            show_walls_standing->value(1);
        }
        if (mark_locations_choice->value()==12){
            show_map->value(1);
        }
        if (mark_locations_choice->value()==13){
            show_map->value(1);
        }
        if (mark_locations_choice->value()==14){            
            show_floor_areas->value(1);
        }

        //floor_choice_cb();

    }
    else{
        

        add_floor->deactivate();
        remove_floor->deactivate();
        mark_locations_choice->value(0);
        location_label_in->deactivate();
        point_in->deactivate();
        clear_selected_location->deactivate();
        clear_last_location->deactivate();
        clear_locations->deactivate();
        overwrite_location_point->deactivate();
        location_label->deactivate();
        push_time();
        push_output("No editing of constructs being used in calculation!\n");
        
        if (!from_calc){
            calc_cb();
        }
        
    }    
    
    draw->redraw();
    old_val=mark_locations_choice->value();
}


void base::test(Fl_Widget* /*widget*/,void* /*data*/){
    //null
}


void base::show_route_menu_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->show_route_menu_cb();
}

void base::show_route_menu_cb(){
    
    
    if (root.calculating!=FREE){
        push_time();
        push_output("Calculation in progress!\n");
        calc_cb();
        return;
    }

    if (root.current->use_turns){
        Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&route_type_c->menu()[2]);
        temp->activate();
        temp =  (Fl_Menu_Item*)(&route_type_c->menu()[4]);
        temp->activate();
    }
    else{

        Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&route_type_c->menu()[2]);
        temp->deactivate();
        temp =  (Fl_Menu_Item*)(&route_type_c->menu()[4]);
        temp->deactivate();

        if ((route_type_c->value()==2)||(route_type_c->value()==4)){
            route_type_c->value(0);
        }
    }

    route_display->resize(win->x()+0.5*win->w()-140,win->y()+0.5*win->h()-74,270,150);
    route_display->show();
  
}

