/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/base.hpp"

void base::clear_building_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->clear_building();
}

void base::clear_building(){
    
    
    if (root.calculating){
        calc_cb();
        return;
    }


    container* root = &(this->root);
    
    
    int i=choice("Clear entire contents of current project slot?","Yes","Cancel",0,"Clear project?");
    
    if (i!=1){
        
        floor_choice->value(0);
        
        floor_choice->clear();
        floor_choice->add("All");
        floor_choice->add("0");
        floor_choice->value(0);
        floor_choice_cb(0);

        if (three_d_choice->value()){
            three_d_choice->value(0);
            three_d_cb();
        }

        root->current->route.clear();
        root->current->matrix.clear();
        std::vector<std::vector<int> >().swap(root->current->matrix);
        
        metrics_mutex.lock();

        root->current->plot_metric.clear();
        std::vector<double>().swap(root->current->plot_metric);
        root->current->max_plot_metric = 0.0;
        root->current->min_plot_metric = 0.0;

        root->current->metrics.clear();
        root->current->metric_des.clear();
        
        std::vector<std::vector<double> >().swap(root->current->metrics);
        std::vector<metric_description>().swap(root->current->metric_des);

        metrics_mutex.unlock();

        root->current->map.clear();            

        root->current->clear();

        draw->first_draw=1;
        draw->redraw();

        std::stringstream s;
        int bb = building_choice->value();
        s<<"Project slot "<<bb+1<<" cleared."<<std::endl;
        prepare_undo();
        push_time();
        push_output(s.str());
        show_warning_map_cb(this);
        show_warning_vis_cb(this);
    }
}

int base::remove_vertex_list(const std::vector<int> &list){

    int to_ret;
    container* root = &(this->root);    
    std::vector<vec>& points = root->current->points;
    std::vector<std::vector<location> >& loc = root->current->locations;
    std::vector<vertex>& segments = root->current->adj_list;
    
    std::vector<int> point_list(points.size());
    std::vector<int> temp_list(points.size());
    std::vector<int> to_delete(points.size(),0); //need a lookup list of which points are deleted in order avoid searching later


    for (unsigned i=0; i<points.size();i++){
        point_list[i]=i;
        temp_list[i]=i;
    }

    int sum=0;

    for (int j=list.size()-1;j>=0;j--){

        int i = list[j];
        to_delete[i]=1;

        int t = temp_list[points.size()-1-sum]; //emulate the swap and pop_back delete method.
        temp_list[points.size()-1-sum] = temp_list[i]; //temp list will be the new order once swapped and deleted
        temp_list[i]=t;
        sum++;   //last sum elements will be removed once actually swapped
    
    }


    for (unsigned int i=0;i<points.size();i++){ //   point_list is then maps the old point index to the new point index after swapping 
        point_list[temp_list[i]]=i;             //   e.g. if the 2nd element of temp_list is 6 then the original point 6 can be found at new index 2.
    }                                           //   so when looing for original point 6 we find it at new index point_list[2]

    ////// end arrange


    metrics_mutex.lock();
    root->current->route.clear();
    root->current->plot_metric.clear();
    root->current->min_plot_metric = 0.0;
    root->current->max_plot_metric = 0.0;
    metrics_mutex.unlock();

    ///// begin reassign and erase


    points_mutex.lock();
    adj_mutex.lock();
    walls_mutex.lock();
    locations_mutex.lock();
    areas_mutex.lock();
    floors_mutex.lock();
    plans_mutex.lock();
    output_mutex.lock();


    for (int i=segments.size()-1;i>=0;i--){//need to count backwards as resizing

        if ((to_delete[segments[i].start])||(to_delete[segments[i].end])){
                
            segments[i] = segments.back();
            segments.pop_back();
        }
    }

    for (int i=segments.size()-1;i>=0;i--){ //update remaining indices in the adj list

        segments[i].start = point_list[segments[i].start];
        segments[i].end = point_list[segments[i].end];
    }

   
   int loc_sum=0;

    for (unsigned int ii=0;ii<loc.size();ii++){
        
        std::vector<location>* loc_vec  = &loc[ii];


        for (int i=loc_vec->size()-1;i>=0;i--){
            if (to_delete[(*loc_vec)[i].point]){
                (*loc_vec)[i] = loc_vec->back();
                loc_vec->pop_back();
                loc_sum++;
            }
        }

        for (unsigned int i=0;i<loc_vec->size();i++){
            loc_vec->at(i).point = point_list[loc_vec->at(i).point];
        }
    }

    to_ret=loc_sum;

    for (int j=list.size()-1;j>=0;j--){

        int i = list[j];
        points[i]=points.back();
        points.pop_back();
    
    }

    output_mutex.unlock();
    plans_mutex.unlock();
    floors_mutex.unlock();
    areas_mutex.unlock();
    locations_mutex.unlock();
    walls_mutex.unlock();
    adj_mutex.unlock();
    points_mutex.unlock();


    Fl::awake(show_warning_map_cb,this);
    Fl::awake(show_warning_vis_cb,this);

    return to_ret;

}

void base::point_in_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->point_in_cb();
}

void base::point_in_cb(){
    
    
    container* root = &(this->root);
    std::vector<vec>* points = &(root->current->points);
    std::vector<location>* loc0 = &(root->current->locations[0]);
    std::vector<location>* loc1 = &(root->current->locations[1]);
    std::vector<location>* loc2 = &(root->current->locations[2]);
    std::vector<location>* loc3 = &(root->current->locations[3]);
    std::vector<location>* loc4 = &(root->current->locations[4]);
    std::vector<location>* loc5 = &(root->current->locations[5]);
    std::vector<location>* loc6 = &(root->current->locations[6]);
    std::vector<location>* loc7 = &(root->current->locations[7]);
    std::vector<location>* loc8 = &(root->current->locations[8]);
    
    std::vector<location>* loc = NULL;
    
    switch (mark_locations_choice->value()){
        case 1:
            loc=loc0;break;
        case 2:
            loc=loc1;break;
        case 3:
            loc=loc2;break;
        case 4:
            loc=loc3;break;
        case 5:
            loc=loc4;break;
        case 6:
            loc=loc5;break;
        case 7:
            loc=loc6;break;
        case 8:
            loc=loc7;break;
        case 9:
            loc=loc8;break;
        default:
            return;
    }
    
   
    if (draw->selector>0){



        int sel = draw->selected;

        if (draw->group_selection_list.size()==1){
            sel = draw->group_selection_list[0];
        }

        if(draw->group_selection_list.size()>1){
            push_time();
            push_output("Select a single location marker!\n");
            return;
        }


        
        if ((sel>-1)  &&  (sel < static_cast<int>(loc->size()) )  ){
            
            std::stringstream s;
            s<<overwrite_location_point->value();

            for (unsigned int i=0;i<s.str().size();i++){
                if (!isdigit(s.str()[i])){
                    s.str("");
                    s<<"Error!";
                    overwrite_location_point->value(s.str().c_str());
                    push_time();
                    push_output("Error overwriting associated vertex. Check input field.\n");
                    return;
                }
            } 

            int i = -1;

            try{
                i = stoi(s.str());
            }
            catch (...){
                i = -1;
            }


            if ((i>-1)  &&  (i < static_cast<int>(points->size())  )  ){

                s.str("");
                s<<i;
                overwrite_location_point->value(s.str().c_str());
            
                locations_mutex.lock();

                (*loc)[sel].point=i;
                
                locations_mutex.unlock();

                push_time();
                std::stringstream s1;
                s1<<"Location index "<<sel<<" associated with graph vertex "<<s.str()<<std::endl;
                push_output(s1.str());
                prepare_undo();
            }
            else{
                s.str("");
                s<<"Error!";
                
                overwrite_location_point->value(s.str().c_str());
                
                push_time();
                push_output("Error overwriting associated vertex. Check input field.\n");

            }
            
            draw->redraw();
            
        }
        else{
            push_time();
            push_output("Select a location!\n");
        }
        
    }
    else{
        push_time();
        push_output("Select a location!\n");
    }
    
    Fl::focus(win);
    
    
}

void base::location_label_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->location_label_cb();
}

void base::location_label_cb(){
    
   
    container* root = &(this->root);
    std::vector<location>* loc0 = &(root->current->locations[0]);
    std::vector<location>* loc1 = &(root->current->locations[1]);
    std::vector<location>* loc2 = &(root->current->locations[2]);
    std::vector<location>* loc3 = &(root->current->locations[3]);
    std::vector<location>* loc4 = &(root->current->locations[4]);
    std::vector<location>* loc5 = &(root->current->locations[5]);
    std::vector<location>* loc6 = &(root->current->locations[6]);
    std::vector<location>* loc7 = &(root->current->locations[7]);
    std::vector<location>* loc8 = &(root->current->locations[8]);
    
    
    std::vector<location>* loc = NULL;
    
    switch (mark_locations_choice->value()){
        case 1:
            loc=loc0;break;
        case 2:
            loc=loc1;break;
        case 3:
            loc=loc2;break;
        case 4:
            loc=loc3;break;
        case 5:
            loc=loc4;break;
        case 6:
            loc=loc5;break;
        case 7:
            loc=loc6;break;
        case 8:
            loc=loc7;break;
        case 9:
            loc=loc8;break;
        default:
            return;
    }
    
    
    
    if (draw->selector>0){

        int sel = draw->selected;

        if (draw->group_selection_list.size()==1){
            sel = draw->group_selection_list[0];
        }

        if(draw->group_selection_list.size()>1){
            push_time();
            push_output("Select a single location marker!\n");
            return;
        }
        
        if (  (sel>-1) && (sel  <  static_cast<int>(loc->size())  )  ){
            
            
            std::stringstream s;
            s<<location_label->value();
            
            
            locations_mutex.lock();
            (*loc)[sel].label=s.str();
            locations_mutex.unlock();


            push_time();
            std::stringstream s1;
            s1<<"Location index "<<sel<<" given label "<<"\""<<s.str()<<"\""<<std::endl;
            push_output(s1.str());

            
            prepare_undo();
            
        }
        else{
            push_time();
            push_output("Select a location!\n");
        }
        
    }
    else{
        push_time();
        push_output("Select a location!\n");
    }
    
    Fl::focus(win);
    
}

void base::clear_last_location_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->clear_last_location_cb();
}

void base::clear_last_location_cb(){
           
    
    container* root = &(this->root);
    std::vector<vec>* points = &(root->current->points);
    std::vector<vertex>* adj_list = &(root->current->adj_list);
    
    std::vector<location>* loc0 = &(root->current->locations[0]);
    std::vector<location>* loc1 = &(root->current->locations[1]);
    std::vector<location>* loc2 = &(root->current->locations[2]);
    std::vector<location>* loc3 = &(root->current->locations[3]);
    std::vector<location>* loc4 = &(root->current->locations[4]);
    std::vector<location>* loc5 = &(root->current->locations[5]);
    std::vector<location>* loc6 = &(root->current->locations[6]);
    std::vector<location>* loc7 = &(root->current->locations[7]);
    std::vector<location>* loc8 = &(root->current->locations[8]);
    
    std::vector<wall>& walls_standing = (root->current->walls_standing);
    std::vector<wall>& walls_sitting = (root->current->walls_sitting);
    
    std::vector<std::vector<vec> >& floorarea_polygons = (root->current->floorarea_polygons);
    std::vector<std::vector<triangle> >& floorarea_triangles = root->current->floorarea_triangles;
    std::vector<int>& floorarea_type = root->current->floorarea_type;
    
    if (!three_d_choice->value()){

        draw->deselect(); // prevent removal of selected quantities

        std::stringstream s;

        s<<"Clearing last (existing) marked ";
        
        
        switch (mark_locations_choice->value()){
            case 1:
                if (loc0->size()){
                    locations_mutex.lock();
                    loc0->pop_back();
                    locations_mutex.unlock();
                    show_warning_visibility();
                    s<<"origin marker."<<std::endl;
                    push_time();
                    push_output(s.str());
                    prepare_undo();
                }
                break;
            case 2:
                if (loc1->size()){
                    locations_mutex.lock();
                    loc1->pop_back();
                    locations_mutex.unlock();
                    show_warning_visibility();
                    s<<"destination 1 marker."<<std::endl;
                    push_time();
                    push_output(s.str());
                    prepare_undo();
                }
                break;
            case 3:
                if (loc2->size()){
                    locations_mutex.lock();
                    loc2->pop_back();
                    locations_mutex.unlock();
                    show_warning_visibility();
                    s<<"destination 2 marker."<<std::endl;
                    push_time();
                    push_output(s.str());
                    prepare_undo();
                }
                break;
            case 4:
                if (loc3->size()){
                    locations_mutex.lock();
                    loc3->pop_back();
                    locations_mutex.unlock();
                    show_warning_visibility();
                    s<<"destination 3 marker."<<std::endl;
                    push_time();
                    push_output(s.str());
                    prepare_undo();
                }
                break;
            case 5:
                if (loc4->size()){
                    locations_mutex.lock();
                    loc4->pop_back();
                    locations_mutex.unlock();
                    show_warning_visibility();
                    s<<"destination 4 marker."<<std::endl;
                    push_time();
                    push_output(s.str());
                    prepare_undo();
                }
                break;
            case 6:
                if (loc5->size()){
                    locations_mutex.lock();
                    loc5->pop_back();
                    locations_mutex.unlock();
                    show_warning_visibility();
                    s<<"destination 5 marker."<<std::endl;
                    push_time();
                    push_output(s.str());
                    prepare_undo();
                }
                break;
            case 7:
                if (loc6->size()){
                    locations_mutex.lock();
                    loc6->pop_back();
                    locations_mutex.unlock();
                    show_warning_visibility();
                    s<<"destination 6 marker."<<std::endl;
                    push_time();
                    push_output(s.str());
                    prepare_undo();
                }
                break;
            case 8:
                if (loc7->size()){
                    locations_mutex.lock();
                    loc7->pop_back();
                    locations_mutex.unlock();
                    show_warning_visibility();
                    s<<"destination 7 marker."<<std::endl;
                    push_time();
                    push_output(s.str());
                    prepare_undo();
                }
                break;
            case 9:
                if (loc8->size()){
                    locations_mutex.lock();
                    loc8->pop_back();
                    locations_mutex.unlock();
                    show_warning_visibility();
                    s<<"destination 8 marker."<<std::endl;
                    push_time();
                    push_output(s.str());
                    prepare_undo();
                }
                break;
            case 10:
                if (draw->set_first){
                    draw->set_first=0;
                }
                else{
                    if (walls_sitting.size()){
                        walls_mutex.lock();
                        walls_sitting.pop_back();
                        walls_mutex.unlock();
                        show_warning_visibility();
                        s<<"low barrier."<<std::endl;
                        push_time();
                        push_output(s.str());
                        prepare_undo();
                    }
                }
                break;
            case 11:
                if (draw->set_first){
                    draw->set_first=0;
                }
                else{
                    if (walls_standing.size()){
                        walls_mutex.lock();
                        walls_standing.pop_back();
                        walls_mutex.unlock();
                        show_warning_visibility();
                        s<<"high barrier."<<std::endl;
                        push_time();
                        push_output(s.str());
                        prepare_undo();
                    }
                }
                break;
            case 12:
            {
                if (points->size()){
                    
                    int ii = static_cast<int>(points->size()-1);
                    
                    adj_mutex.lock();
                    
                    for (int i=adj_list->size()-1;i>=0;i--){
                        int p1 = (*adj_list)[i].start;
                        int p2 = (*adj_list)[i].end;
                        
                        if ((p1==ii)||(p2==ii)){
                            adj_list->erase(adj_list->begin()+i);
                        }
                        
                    }
                    
                    adj_mutex.unlock();

                    points_mutex.lock();
                    points->erase(points->begin()+ii);
                    points_mutex.unlock();

                    metrics_mutex.lock();
                    root->current->route.clear();
                    root->current->plot_metric.clear();
                    root->current->max_plot_metric = 0.0;
                    root->current->min_plot_metric = 0.0;
                    metrics_mutex.unlock();
                    
                    int loc_change = 0;

                    locations_mutex.lock();

                    loc_change += update_locations(loc0,ii);
                    loc_change += update_locations(loc1,ii);
                    loc_change += update_locations(loc2,ii);
                    loc_change += update_locations(loc3,ii);
                    loc_change += update_locations(loc4,ii);
                    loc_change += update_locations(loc5,ii);
                    loc_change += update_locations(loc6,ii);
                    loc_change += update_locations(loc7,ii);
                    loc_change += update_locations(loc8,ii);
                    
                    locations_mutex.unlock();

                    warning_map_cb();

                    if (loc_change>0){
                        show_warning_visibility();
                    }

                    prepare_undo();

                    s<<"spatial graph vertex."<<std::endl;
                    push_time();
                    push_output(s.str());
                }
                
            }
                break;
            case 13:
            {
                if (adj_list->size()){
                    
                    adj_mutex.lock();
                    adj_list->pop_back();
                    adj_mutex.unlock();

                    metrics_mutex.lock();
                    root->current->route.clear();
                    root->current->plot_metric.clear();
                    root->current->max_plot_metric = 0.0;
                    root->current->min_plot_metric = 0.0;
                    metrics_mutex.unlock();

                    warning_map_cb();
                    s<<"spatial graph edge."<<std::endl;
                    push_time();
                    push_output(s.str());
                    prepare_undo();
                }
                
            }
                break;
            case 14:
                
                
                if (static_cast<int>(floorarea_polygons.size())>1){
                    
                    areas_mutex.lock();
                    
                    floorarea_polygons.pop_back();
                    floorarea_polygons.pop_back();
                    floorarea_polygons.push_back(std::vector<vec>(0));
                
                    if (floorarea_triangles.size()){

                        floorarea_triangles.pop_back();
                        floorarea_type.pop_back();
                    }

                    areas_mutex.unlock();
                    
                    prepare_undo();
                    s<<"floor area polygon."<<std::endl;
                    push_time();
                    push_output(s.str());
                }
                draw->calculate_areas(&floorarea_polygons);
        
                break;
                
        }
        
        draw->redraw();
    }
}

void base::clear_selected_location_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->clear_selected_location_cb();
}

void base::clear_selected_location_cb(){
    
    container* root = &(this->root);
    std::vector<vec>* points = &(root->current->points);
    std::vector<vertex>* adj_list = &(root->current->adj_list);
    
    std::vector<location>* loc0 = &(root->current->locations[0]);
    std::vector<location>* loc1 = &(root->current->locations[1]);
    std::vector<location>* loc2 = &(root->current->locations[2]);
    std::vector<location>* loc3 = &(root->current->locations[3]);
    std::vector<location>* loc4 = &(root->current->locations[4]);
    std::vector<location>* loc5 = &(root->current->locations[5]);
    std::vector<location>* loc6 = &(root->current->locations[6]);
    std::vector<location>* loc7 = &(root->current->locations[7]);
    std::vector<location>* loc8 = &(root->current->locations[8]);
    
    
    std::vector<wall>* walls_standing = &(root->current->walls_standing);
    std::vector<wall>* walls_sitting = &(root->current->walls_sitting);
    
    std::vector<std::vector<vec> >& floorarea_polygons = root->current->floorarea_polygons;
    std::vector<std::vector<triangle> >& floorarea_triangles = root->current->floorarea_triangles;
    std::vector<int>& floorarea_type = root->current->floorarea_type;
    
    if ((mark_locations_choice->value())&&(!three_d_choice->value())){

        //single object deleted
        if ((draw->selector>0)&&((draw->selected>-1)||((draw->selected_matrix[0]>-1)&&(draw->selected_matrix[1]>-1)))){

            
            std::vector<location>* temp=NULL;
            std::vector<wall>* temp_wall=NULL;
            
            int loc_or_wall=-1;
            
            std::stringstream s;

            switch (draw->selector){
                    
                case 1:
                    temp=loc0;
                    loc_or_wall=0;
                    s<<"Selected origin location marker removed."<<std::endl;
                    
                    break;
                case 2:
                    temp=loc1;
                    loc_or_wall=0;
                    s<<"Selected destination 1 marker removed."<<std::endl;
                    
                    break;
                case 3:
                    temp=loc2;
                    loc_or_wall=0;
                    s<<"Selected destination 2 marker removed."<<std::endl;
                    
                    break;
                case 4:
                    temp=loc3;
                    loc_or_wall=0;
                    s<<"Selected destination 3 marker removed."<<std::endl;
                    
                    break;
                case 5:
                    temp=loc4;
                    loc_or_wall=0;
                    s<<"Selected destination 4 marker removed."<<std::endl;
                    
                    break;
                case 6:
                    temp=loc5;
                    loc_or_wall=0;
                    s<<"Selected destination 5 marker removed."<<std::endl;
                    
                    break;
                case 7:
                    temp=loc6;
                    loc_or_wall=0;
                    s<<"Selected destination 6 marker removed."<<std::endl;
                    
                    break;
                case 8:
                    temp=loc7;
                    loc_or_wall=0;
                    s<<"Selected destination 7 marker removed."<<std::endl;
                    
                    break;
                case 9:
                    temp=loc8;
                    loc_or_wall=0;
                    s<<"Selected destination 8 marker removed."<<std::endl;
                    
                    break;
                case 10:
                    temp_wall=walls_sitting;
                    loc_or_wall=1;
                    s<<"Selected low barrier removed."<<std::endl;
                    
                    break;
                case 11:
                    temp_wall=walls_standing;
                    loc_or_wall=1;
                    s<<"Selected high barrier removed."<<std::endl;
                    
                    break;
                case 12:
                    loc_or_wall=2;
                    s<<"Selected spatial graph vertex removed in addition to all associated edges and locations."<<std::endl;
                    break;
                case 13:
                    loc_or_wall=3;
                    s<<"Selected spatial graph edge removed."<<std::endl;
                    break;
                case 14:
                    loc_or_wall=4;
                    s<<"Selected floor area polygon removed."<<std::endl;
                    break;
                default:
                    break;
            }
            
            
            if ((draw->selected>-1)&&(loc_or_wall==0)){ //erasing location
                if (draw->selected  <  static_cast<int>(temp->size()) ){
                    
                    locations_mutex.lock();
                    temp->erase(temp->begin()+draw->selected);
                    locations_mutex.unlock();

                    draw->deselect();
                    
                    draw->redraw();
                    
                    
                    show_warning_visibility();
                    prepare_undo();
                    push_time();
                    push_output(s.str());
                    
                }
            }
            else if ((draw->selected>-1)&&(loc_or_wall==1)){ //erasing wall
                if (draw->selected < static_cast<int>(temp_wall->size()) ){
                                    
                    walls_mutex.lock();
                    temp_wall->erase(temp_wall->begin()+draw->selected);
                    walls_mutex.unlock();

                    draw->deselect();
                    draw->redraw();
                    
                    show_warning_visibility();
                    prepare_undo();
                    push_time();
                    push_output(s.str());
                    
                }
            }
            else if ((draw->selected>-1)&&(loc_or_wall==2)){ //erasing spatial map point
                
                
                int ii = draw->selected;
                
                adj_mutex.lock();

                for (int i=adj_list->size()-1;i>=0;i--){
                    int p1 = (*adj_list)[i].start;
                    int p2 = (*adj_list)[i].end;
                    
                    if ((p1==ii)||(p2==ii)){
                        adj_list->erase(adj_list->begin()+i);

                    }
                    
                }
                
                for (unsigned int i=0;i<adj_list->size();i++){
                    
                    int p1 = (*adj_list)[i].start;
                    int p2 = (*adj_list)[i].end;
                    
                    
                    if (p1>ii){
                        (*adj_list)[i].start--;
                    }
                    if (p2>ii){
                        (*adj_list)[i].end--;
                    }
                    
                }
             
                adj_mutex.unlock();

                points_mutex.lock();
                points->erase(points->begin()+draw->selected);
                points_mutex.unlock();

                metrics_mutex.lock();
                root->current->route.clear();
                root->current->plot_metric.clear();
                root->current->max_plot_metric = 0.0;
                root->current->min_plot_metric = 0.0;
                metrics_mutex.unlock();
                
                
                //need to add deletion/adaption of locations associated with point reomved and points that come after...
                
                int loc_change = 0;

                locations_mutex.lock();

                loc_change += update_locations(loc0,draw->selected);
                loc_change += update_locations(loc1,draw->selected);
                loc_change += update_locations(loc2,draw->selected);
                loc_change += update_locations(loc3,draw->selected);
                loc_change += update_locations(loc4,draw->selected);
                loc_change += update_locations(loc5,draw->selected);
                loc_change += update_locations(loc6,draw->selected);
                loc_change += update_locations(loc7,draw->selected);
                loc_change += update_locations(loc8,draw->selected);
                
                locations_mutex.unlock();

                push_time();
                push_output(s.str());

                draw->deselect();
                warning_map_cb();

                if (loc_change>0){
                    show_warning_visibility();
                }

                draw->redraw();
                prepare_undo();
                
            }
            else if ((draw->selected>-1)&&(loc_or_wall==3)){ //erasing spatial map link
                
                adj_mutex.lock();

                adj_list->erase(adj_list->begin()+draw->selected);

                adj_mutex.unlock();


                draw->deselect();

                metrics_mutex.lock();

                root->current->route.clear();
                root->current->plot_metric.clear();
                root->current->max_plot_metric = 0.0;
                root->current->min_plot_metric = 0.0;

                metrics_mutex.unlock();

                warning_map_cb();
                draw->redraw();

                push_time();
                push_output(s.str());
                prepare_undo();
            }
            else if ((draw->selected>-1)&&(loc_or_wall==4)){//erasing area;
                
                areas_mutex.lock();
                
                floorarea_polygons.erase(floorarea_polygons.begin()+draw->selected);
                
                floorarea_triangles.erase(floorarea_triangles.begin()+draw->selected);
                floorarea_type.erase(floorarea_type.begin()+draw->selected);
                
                areas_mutex.unlock();
                draw->deselect();

                draw->calculate_areas(&floorarea_polygons);
                
                draw->redraw();

                push_time();
                push_output(s.str());
                prepare_undo();
                
            }

                       
        }
        else if ((draw->selector>0)&&(draw->group_selection_list.size())){ //delete list of selected objects
            
            int sel = draw->selector;
            const std::vector<int> group = draw->group_selection_list;
            draw->deselect();


            if ((sel>0)&&(sel<10)){//location markers

                int s = sel - 1;

                std::vector<location> &loc = root->current->locations[s];

                locations_mutex.lock();
                for (int i = static_cast<int>(group.size()) -1; i>=0 ; i--){

                    loc[group[i]] = loc.back(); // can use order changing delete because visibility changes anyway
                    loc.pop_back();

                }

                locations_mutex.unlock();

                std::stringstream ss;

                ss<<group.size();

                if (s==0){
                    ss<<" selected origin location markers removed."<<std::endl;
                }
                else{
                    ss<<" selected destination "<<s<<" location markers removed."<<std::endl;
                }

                show_warning_visibility();

                push_time();
                push_output(ss.str());
                draw->redraw();
                prepare_undo();

            }
            else if ((sel==10)||(sel==11)){//walls/barriers

                std::vector<wall> *walls = &(root->current->walls_sitting);

                if (sel==11){
                    walls = &(root->current->walls_standing);
                }

                walls_mutex.lock();   //dont need to worry about ordering of walls so long as group and walls agree, so swap as needed from the back...
                for (int i = static_cast<int>(group.size()) -1; i>=0 ; i--){

                    (*walls)[group[i]] = walls->back();
                    walls->pop_back();

                }
                walls_mutex.unlock();

                std::stringstream ss;

                ss<<group.size();

                if (sel==10){
                    ss<<" selected low barriers removed."<<std::endl;
                }
                else{
                    ss<<" selected high barriers removed."<<std::endl;   
                }

                show_warning_visibility();

                push_time();
                push_output(ss.str());

                
                draw->redraw();
                prepare_undo();
            }
            else if (sel==13){//edges

                std::vector<vertex> &edges = root->current->adj_list;

                adj_mutex.lock();
                for (int i = static_cast<int>(group.size()) -1; i>=0 ; i--){

                    edges[group[i]] = edges.back(); //order destroying - graph structures flaged as needing to be recalculated anyway
                    edges.pop_back();

                }
                adj_mutex.unlock();


                metrics_mutex.lock();
                root->current->route.clear();
                root->current->plot_metric.clear();
                root->current->max_plot_metric = 0.0;
                root->current->min_plot_metric = 0.0;
                metrics_mutex.unlock();


                std::stringstream ss;

                ss<<group.size();

                ss<<" selected edges removed."<<std::endl;

                warning_map_cb();

                push_time();
                push_output(ss.str());

                
                draw->redraw();
                prepare_undo();
            }
            else if (sel==14){//areas

                std::vector<std::vector<vec> > &polygons = root->current->floorarea_polygons;
                std::vector<std::vector<triangle> > &triangles = root->current->floorarea_triangles;
                std::vector<int> &types = root->current->floorarea_type;

                areas_mutex.lock();

                polygons.pop_back();
                for (int i = static_cast<int>(group.size()) -1; i>=0 ; i--){

                    polygons[group[i]] = polygons.back();
                    polygons.pop_back();
                    triangles[group[i]] = triangles.back();
                    triangles.pop_back();
                    types[group[i]] = types.back();
                    types.pop_back();


                }
                polygons.push_back(std::vector<vec>(0));
                areas_mutex.unlock();

                std::stringstream ss;

                ss<<group.size();

                ss<<" selected areas removed."<<std::endl;

                push_time();
                push_output(ss.str());
                
                draw->redraw();
                prepare_undo();
            }
            else if (sel==12){//points

                int loc_change = remove_vertex_list(group);                

                warning_map_cb();

                if (loc_change>0){
                    show_warning_visibility();
                }

                std::stringstream ss;

                ss<<group.size();

                ss<<" selected vertices removed in addition to all associated edges and locations."<<std::endl;

                push_time();
                push_output(ss.str());

                
                draw->redraw();
                prepare_undo();
            }
   
        }
               
    }

    draw->redraw();
    
}

int base::update_locations(std::vector<location>* location,int removed_point){
    
    
    
    std::vector<int> for_removal;
    
    for (unsigned int i=0;i<location->size();i++){ //find all locations associated with spatial map point
        if ((*location)[i].point==removed_point){
            for_removal.push_back(i);
        }
        
    }
    
    for (unsigned int i=0;i<location->size();i++){ // change the spatial map point for all locations that which will move one down the list to fill the gap
        if ((*location)[i].point>removed_point){
            (*location)[i].point--;
        }
        
    }
    
    
    for (int i=static_cast<int>(for_removal.size()-1);i>-1;i--){ //can guarantee that for_removal is ordered absolutely. HAVE to run backwards
        (*location).erase((*location).begin()+for_removal[i]);//delete the points in question: only works if running backwards otherwise the index changes
        
    }

    if (for_removal.size()>0){ // indicate if any locations were removed
        return 1;
    }
    else{
        return 0;
    }
    
}

void base::clear_locations_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->clear_locations_cb();
}

void base::clear_locations_cb(){
    
    container* root = &(this->root);
    std::vector<vec>* points = &(root->current->points);
    std::vector<vertex>* adj_list = &(root->current->adj_list);
    
    std::vector<location>* loc0 = &(root->current->locations[0]);
    
    
    std::vector<wall>& walls_sitting = root->current->walls_sitting;
    std::vector<wall>& walls_standing = root->current->walls_standing;
    std::vector<std::vector<vec> >& floorarea_polygons = root->current->floorarea_polygons;
    std::vector<std::vector<triangle> >& floorarea_triangles = root->current->floorarea_triangles;
    std::vector<int>& floorarea_type = root->current->floorarea_type;
    
    
    if ((!three_d_choice->value()&&(mark_locations_choice->value()))){
        
        int i=choice("Delete all constructs from the current floor or all floors?","Current","All","Cancel","Delete?");    
        
        if (i==2){
            return;
        }
        else if (i==1){ //clear everything across all floors

            draw->deselect();
            switch (mark_locations_choice->value()){
                case 0: break;
                case 1:
                    if (loc0->size()>0){
                        locations_mutex.lock();
                        loc0->clear();
                        std::vector<location>().swap(*loc0);
                        locations_mutex.unlock();
                        show_warning_visibility();

                        push_time();
                        push_output("All origin location markers removed.\n");
                        prepare_undo();
                    }
                    else{
                        push_time();
                        push_output("No origin location markers to remove.\n");

                    }

                    break;
                
                case 10:
                    if (walls_sitting.size()>0){
                        walls_mutex.lock();
                        walls_sitting.clear();
                        std::vector<wall>().swap(walls_sitting);
                        walls_mutex.unlock();
                        if (draw->set_first){
                            draw->set_first=0;
                        }
                        push_time();
                        push_output("All low barriers removed.\n");
                        show_warning_visibility();
                        prepare_undo();
                    }
                    else{
                        push_time();
                        push_output("No low barriers to remove.\n");
                    }
                    break;
                case 11:
                    if (walls_standing.size()>0){
                        walls_mutex.lock();
                        walls_standing.clear();
                        std::vector<wall>().swap(walls_standing);
                        walls_mutex.unlock();
                        if (draw->set_first){
                            draw->set_first=0;
                        }
                        push_time();
                        push_output("All high barriers removed.\n");
                        show_warning_visibility();
                        prepare_undo();
                    }
                    else{
                        push_time();
                        push_output("No high barriers to remove.\n");
                    }
                    break;
                case 12:

                    if (points->size()>0){

                        locations_mutex.lock();

                        for (unsigned int i=0;i<root->current->locations.size();i++){
                            root->current->locations[i].clear();
                            std::vector<location>().swap(root->current->locations[i]);
                        }
            
                        locations_mutex.unlock();
                        
                        adj_mutex.lock();
                        adj_list->clear();
                        std::vector<vertex>().swap(*adj_list);
                        adj_mutex.unlock();

                        points_mutex.lock();
                        points->clear();
                        std::vector<vec>().swap(*points);
                        points_mutex.unlock();

                        metrics_mutex.lock();
                        root->current->route.clear();
                        root->current->plot_metric.clear();
                        root->current->min_plot_metric = 0.0;
                        root->current->max_plot_metric = 0.0;


                        metrics_mutex.unlock();
                        
                        warning_map_cb();
                        show_warning_vis_cb(this);
                        prepare_undo();
                        push_time();
                        push_output("All spatial graph vertices removed in addition to all associated edges and locations.\n");                    
                    }
                    else{
                        push_time();
                        push_output("No vertices to remove.\n");
                    }
                    break;
                case 13:

                    if (adj_list->size()>0){
                        adj_mutex.lock();
                        adj_list->clear();
                        std::vector<vertex>().swap(*adj_list);
                        adj_mutex.unlock();

                        metrics_mutex.lock();
                        root->current->route.clear();
                        root->current->plot_metric.clear();
                        root->current->min_plot_metric = 0.0;
                        root->current->max_plot_metric = 0.0;
                        metrics_mutex.unlock();
                        warning_map_cb();

                        push_time();
                        push_output("All spatial graph edges removed.\n");
                        prepare_undo();
                    }
                    else{
                        push_time();
                        push_output("No edges to remove.\n");

                    }
                    break;
                case 14:


                    if (floorarea_triangles.size()>0){
                        areas_mutex.lock();

                        floorarea_polygons.clear();
                        std::vector<std::vector<vec> >().swap(floorarea_polygons);
                        floorarea_triangles.clear();
                        std::vector<std::vector<triangle> >().swap(floorarea_triangles);
                        floorarea_type.clear();
                        std::vector<int>().swap(floorarea_type);

                        floorarea_polygons.push_back(std::vector<vec>(0));
                        
                        areas_mutex.unlock();

                        draw->calculate_areas(&floorarea_polygons);

                        push_time();
                        push_output("All floor area polygons removed.\n");
                        prepare_undo();
                    }
                    else{
                        push_time();
                        push_output("No floor areas to remove.\n");
                    }
                    break;
                default:
                    if ((mark_locations_choice->value()>1)&&(mark_locations_choice->value() < 10)){

                        int selector = mark_locations_choice->value()  -  1;

                        std::vector<location>* loc = &(root->current->locations[selector]);

                        if (loc->size()>0){
                            locations_mutex.lock();
                            loc->clear();
                            std::vector<location>().swap(*loc);
                            locations_mutex.unlock();
                            show_warning_visibility();
                            std::stringstream ss;
                            ss<<"All destination "<<selector<<" location markers removed."<<std::endl;
                            push_time();
                            push_output(ss.str());
                            prepare_undo();
                        }
                        else{
                            std::stringstream ss;
                            ss<<"No destination "<<selector<<" location markers to remove."<<std::endl;
                            push_time();
                            push_output(ss.str());

                        }
                    }
            }

        }
        else if (i==0){  //clear constructs on the current floor

            draw->deselect();
            int floor = floor_choice->value()-1;


            if (mark_locations_choice->value()==1){

                locations_mutex.lock();
                int count = 0;
                for (int i = static_cast<int>(loc0->size())-1; i>=0 ; i--){
                    if ((*loc0)[i].loc.floor == floor){
                        (*loc0)[i] = loc0->back(); // can use order changing delete because visibility changes anyway
                        loc0->pop_back();
                        count++;
                    }
                }
                locations_mutex.unlock();
                std::stringstream s;
                if (count){
                    s<<"All origin location markers, totalling "<<count<<" markers, removed from floor "<<floor<<"."<<std::endl;
                    show_warning_visibility();
                }
                else{
                    s<<"No origin markers to delete on floor "<<floor<<"."<<std::endl;
                }
                push_time();
                push_output(s.str());
                prepare_undo();
            }
            else if (mark_locations_choice->value()<10){

                int selector = mark_locations_choice->value()  -  1;

                std::vector<location>* loc = &(root->current->locations[selector]);

                locations_mutex.lock();
                int count = 0;
                for (int i = static_cast<int>(loc->size())-1; i>=0 ; i--){
                    if ((*loc)[i].loc.floor == floor){
                        (*loc)[i] = loc->back(); // can use order changing delete because visibility changes anyway
                        loc->pop_back();
                        count++;
                    }
                }
                locations_mutex.unlock();
                std::stringstream s;
                if (count){
                    s<<"All destination "<< selector <<" location markers, totalling "<<count<<" markers, removed from floor "<<floor<<"."<<std::endl;
                    show_warning_visibility();
                }
                else{
                    s<<"No origin markers to delete on floor "<<floor<<"."<<std::endl;
                }
                push_time();
                push_output(s.str());
                prepare_undo();
            }
            else if (mark_locations_choice->value()==10){

                std::vector<wall> &w = root->current->walls_sitting;

                walls_mutex.lock();
                int count = 0;
                for (int i = static_cast<int>(w.size())-1; i>=0 ; i--){
                    if (w[i].start.floor == floor){
                        w[i] = w.back(); // can use order changing delete because visibility changes anyway
                        w.pop_back();
                        count++;
                    }
                }
                walls_mutex.unlock();
                std::stringstream s;
                if (count){
                    s<<"All "<<count<<" low barriers removed from floor "<<floor<<"."<<std::endl;
                    show_warning_visibility();
                }
                else{
                    s<<"No low barriers to delete on floor "<<floor<<"."<<std::endl;
                }
                push_time();
                push_output(s.str());
                prepare_undo();
            }
            else if (mark_locations_choice->value()==11){

                std::vector<wall> &w = root->current->walls_standing;

                walls_mutex.lock();
                int count = 0;
                for (int i = static_cast<int>(w.size())-1; i>=0 ; i--){
                    if (w[i].start.floor == floor){
                        w[i] = w.back(); // can use order changing delete because visibility changes anyway
                        w.pop_back();
                        count++;
                    }
                }
                walls_mutex.unlock();
                std::stringstream s;
                if (count){
                    s<<"All "<<count<<" high barriers removed from floor "<<floor<<"."<<std::endl;
                    show_warning_visibility();
                }
                else{
                    s<<"No high barriers to delete on floor "<<floor<<"."<<std::endl;
                }
                push_time();
                push_output(s.str());
                prepare_undo();
            }
            else if (mark_locations_choice->value()==12){ //vertices

                std::vector<int> removed = remove_map_from_floor(floor);
                metrics_mutex.lock();
                root->current->route.clear();
                root->current->plot_metric.clear();
                root->current->max_plot_metric = 0.0;
                root->current->min_plot_metric = 0.0;
                metrics_mutex.unlock();

                draw->deselect();

                warning_map_cb();

                if (removed[1]>0){
                    show_warning_visibility();
                }

                draw->redraw();
                std::stringstream s;
                if (removed[0]){
                    s<<"All "<<removed[0]<<" vertices removed from floor "<<floor<<" in addition to all associated edges and locations."<<std::endl;
                }
                else{
                    s<<"No vertices to delete on floor "<<floor<<"."<<std::endl;
                }
                push_time();
                push_output(s.str());
                prepare_undo();
            }
            else if (mark_locations_choice->value()==13){

                int count = 0;

                adj_mutex.lock();

                for (int i=adj_list->size()-1;i>=0;i--){
                    int p1 = (*adj_list)[i].start;
                    int p2 = (*adj_list)[i].end;
                    
                    if (((*points)[p1].floor==floor)&&((*points)[p2].floor==floor)){
                        (*adj_list)[i] = adj_list->back(); // can use order changing delete because visibility changes anyway
                        adj_list->pop_back();
                        count++;
                    }                    
                }
             
                adj_mutex.unlock();

                draw->redraw();
                std::stringstream s;
                if (count){
                    s<<"All "<<count<<" edges removed from floor "<<floor<<". Cross floor edges not included."<<std::endl;
                    warning_map_cb();
                }
                else{
                    s<<"No edges to delete on floor "<<floor<<". Cross floor edges not included."<<std::endl;
                }
                push_time();
                push_output(s.str());
                prepare_undo();
            }
            else if (mark_locations_choice->value()==14){

                int count = 0;

                areas_mutex.lock();

                floorarea_polygons.pop_back();

                for (int i=floorarea_triangles.size()-1;i>=0;i--){
                    
                    int fl = floorarea_polygons[i][0].floor;
                    
                    if (fl == floor){
                        
                        floorarea_polygons[i] = floorarea_polygons.back(); // can use order changing delete because visibility changes anyway
                        floorarea_polygons.pop_back();
                        floorarea_triangles[i] = floorarea_triangles.back(); // can use order changing delete because visibility changes anyway
                        floorarea_triangles.pop_back();
                        floorarea_type[i] = floorarea_type.back(); // can use order changing delete because visibility changes anyway
                        floorarea_type.pop_back();
                        
                        count++;
                    }                    
                }

                floorarea_polygons.push_back(std::vector<vec>(0));

                areas_mutex.unlock();

                draw->redraw();
                std::stringstream s;
                if (count){
                    s<<"All "<<count<<" floor areas removed from floor "<<floor<<"."<<std::endl;
                    warning_map_cb();
                }
                else{
                    s<<"No floor areas to delete on floor "<<floor<<"."<<std::endl;
                }
                push_time();
                push_output(s.str());
                prepare_undo();

            }

        }

        draw->redraw();
    }
}