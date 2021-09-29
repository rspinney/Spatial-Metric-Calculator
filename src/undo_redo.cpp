/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/base.hpp"

void base::clear_and_prepare_undo_from_thread(void* data){
    base* program = static_cast<base*> (data);
    program->clear_and_prepare_undo();
}


void base::clear_and_prepare_undo(){

    container* root = &(this->root);

    root->current->memento.clear();
    root->current->write_to_memento();

    root->current->current_frame = root->current->memento.size()-1;

}
void base::prepare_undo(){
    

    container* root = &(this->root);

    root->calculating = PREP_UNDO;
    calc_message_on();


    {

        if (root->current->current_frame < static_cast<int>(root->current->memento.size())){
            std::deque<building_base>::iterator start, end;
            start=root->current->memento.begin()+root->current->current_frame+1;
            end=root->current->memento.begin()+root->current->memento.size();
            root->current->memento.erase(start,end);
        }
        root->current->write_to_memento();

        root->current->current_frame = root->current->memento.size()-1;

        if (static_cast<int>(root->current->memento.size()) > MEMENTO_DEPTH){
            root->current->memento.pop_front();
            root->current->current_frame--;        
        }

        root->calculating=FREE;
        calc_message_off();

    }
    
}

void base::undo_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->undo(1);
}


void base::undo(int report_to_user){

    if (root.calculating){

        push_time();
        std::stringstream s;
        s<<"Undo failed: No editing constructs whilst calculating."<<std::endl;
        calc_cb();
        push_output(s.str());
        return;

    }

    
    container* root = &(this->root);


    if (root->current->current_frame<=0) return;

    --root->current->current_frame;


    int i=floor_choice->value();
    
    if (root->current->current_frame < static_cast<int>(root->current->memento.size())){

        std::vector<double>& floor_index = root->current->floor_index;

        points_mutex.lock();        
        adj_mutex.lock();        
        walls_mutex.lock();        
        locations_mutex.lock();        
        areas_mutex.lock();
        floors_mutex.lock();
        plans_mutex.lock();
        metrics_mutex.lock();

        root->current->assign(root->current->memento[root->current->current_frame]);

        metrics_mutex.unlock();
        plans_mutex.unlock();
        floors_mutex.unlock();
        areas_mutex.unlock();
        locations_mutex.unlock();
        walls_mutex.unlock();
        adj_mutex.unlock();
        points_mutex.unlock();


        Fl::lock();

        floor_choice->clear();
            
        floor_choice->add("All");
        
        std::stringstream s;
        
        for (unsigned int i=0; i<floor_index.size();i++){
            s.str("");
            s<<i;
            floor_choice->add(s.str().c_str());
        }
        
        while (i-1>=static_cast<int>(floor_index.size())){
            i=i-1;
            if (i<0){i=0;break;}
        }
        floor_choice->value(i);
        floor_choice_cb(0);
        
        two_element_deselect();

        draw->deselect();
        draw->redraw();

        if (report_to_user){

            push_time();
            s.str("");
            s<<"Reverting to frame "<<root->current->current_frame+1<<" from frame "<<root->current->current_frame+2<<" out of "<<root->current->memento.size()<<" total frames. Maximum frame set at "<<MEMENTO_DEPTH<<"."<<std::endl;
            push_output(s.str().c_str());

        }

        find_floor_extremes();

        draw->show_warning_visibility();
        draw->show_warning_map();

        Fl::unlock();

    }
    
    
}

void base::redo_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->redo(1);
}


void base::redo(int report_to_user){

    if (root.calculating){

        push_time();
        std::stringstream s;
        s<<"Redo failed: No editing constructs whilst calculating."<<std::endl;
        calc_cb();
        push_output(s.str());
        return;

    }

    if (root.current->current_frame + 1 < static_cast<int>(root.current->memento.size()) ){

        ++root.current->current_frame;


        int i=floor_choice->value();

        std::vector<double>& floor_index = root.current->floor_index;

        points_mutex.lock();
        adj_mutex.lock();
        walls_mutex.lock();
        locations_mutex.lock();
        areas_mutex.lock();
        floors_mutex.lock();
        plans_mutex.lock();
        metrics_mutex.lock();

        root.current->assign(root.current->memento[root.current->current_frame]);

        metrics_mutex.unlock();
        plans_mutex.unlock();
        floors_mutex.unlock();
        areas_mutex.unlock();
        locations_mutex.unlock();
        walls_mutex.unlock();
        adj_mutex.unlock();
        points_mutex.unlock();

        Fl::lock();

        floor_choice->clear();
            
        floor_choice->add("All");
        
        std::stringstream s;
        
        for (unsigned int i=0; i<floor_index.size();i++){
            s.str("");
            s<<i;
            floor_choice->add(s.str().c_str());
        }
        
        while (i-1>=static_cast<int>(floor_index.size())){
            i=i-1;
            if (i<0){i=0;break;}
        }
        floor_choice->value(i);
        floor_choice_cb(0); //reverting back to no undo functionality for last polygon until I can figure out a better way
        

        if (report_to_user){
            push_time();
            s.str("");
            s<<"Reverting to frame "<<root.current->current_frame+1<<" from frame "<<root.current->current_frame<<" out of "<<root.current->memento.size()<<" total frames. Maximum frame set at "<<MEMENTO_DEPTH<<"."<<std::endl;
            push_output(s.str().c_str());
        }

        two_element_deselect();

        draw->deselect();
        draw->redraw();

        find_floor_extremes();

        draw->show_warning_visibility();
        draw->show_warning_map();

        Fl::unlock();
    }
   
}


void base::memento_depth_win_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->memento_depth_win_cb();
}
void base::memento_depth_win_cb(){

    std::stringstream s;
    s<<MEMENTO_DEPTH;
    memento_depth_input->value(s.str().c_str());
    memento_depth_win->show();
    memento_depth_win->resize(win->x()+0.5*win->w()-140,win->y()+0.5*win->h()-45,280,90);
}

void base::change_memento_depth_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->change_memento_depth_cb();
}


void base::change_memento_depth_cb(){


    std::stringstream s;
    s<<memento_depth_input->value();

    int new_val = -1;
    int old_val = MEMENTO_DEPTH;

    try{
        new_val = stoi(s.str());
    }
    catch (...){
        push_time();
        push_output("Problem with new undo stack depth - check input");
        return;
    }

    if (new_val <1){
        push_time();
        push_output("Stack not changed: minimum stack length is 1. (This corresponds to no undo/redo functionality).\n");
        return;
    }

    s.str("");
    s<<"Resize undo stack to "<<new_val<<" from "<<old_val<<" for all project slots? This is irreversible.";

    int i = choice(s.str().c_str(),"Yes","Cancel",0,"Resize stack?");

    if (i==0){ //go ahead with change of stack
        

        if ((old_val!=new_val)&&(new_val>0)){

            MEMENTO_DEPTH = new_val;

            std::stringstream s;
            s<<"Adjusting undo/redo stack length from "<<old_val<<" to "<<MEMENTO_DEPTH<<"."<<std::endl;

            push_time();
            push_output(s.str());

            for (unsigned int ii=0; ii<NUM_BUILDINGS;ii++){

                

                int front_count = 0;

                while (static_cast<int>(root.comp_buildings[ii]->memento.size()) > MEMENTO_DEPTH){
                    //std::cout<<"on project "<<i<<" depth = "<<root.comp_buildings[ii]->memento.size()<<std::endl;
                    root.comp_buildings[ii]->memento.pop_front();
                    root.comp_buildings[ii]->current_frame--; 
                    front_count++;       
                    if (root.comp_buildings[ii]->current_frame<=0){
                        break;
                    }
                }

               
                int back_count = 0;

                while (static_cast<int>(root.comp_buildings[ii]->memento.size()) > MEMENTO_DEPTH){
                    //std::cout<<"on project "<<i<<" depth = "<<root.comp_buildings[ii]->memento.size()<<std::endl;
                    root.comp_buildings[ii]->memento.pop_back();
                    back_count++;
                }


                if ((front_count>0)||(back_count>0)){

                    std::stringstream s;
                    s<<"Adjustments made to existing stack for project "<<ii+1<<":"<<std::endl;

                    push_time();
                    push_output(s.str());

                    s.str("");
                    s<<"Preferentially removing undo frames, then redo frames until stack length reached."<<std::endl;
                    push_time();
                    push_output(s.str());

                    s.str("");
                    s<<"Removed "<<front_count<<" frame(s) from the undo end."<<std::endl;

                    push_time();
                    push_output(s.str());


                    s.str("");
                    s<<"Removed "<<back_count<<" frame(s) from the redo end."<<std::endl;

                    push_time();
                    push_output(s.str());

                }

            }
        }
        


    }

    memento_depth_win->hide();
    return;
}


void base::close_memento_depth_win_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->close_memento_depth_win();
}


void base::close_memento_depth_win(){

    memento_depth_win->hide();
}