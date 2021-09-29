/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/base.hpp"

void base::link_win_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->link_win_cb();
}
void base::link_win_cb(){
    if (root.calculating){
        calc_cb();
        return;
    }
    add_link_win->show();
    add_link_win->resize(win->x()+0.5*win->w()-165,win->y()+0.5*win->h()-155,330,312);
}


void base::close_link_win_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->close_link_win();
}


void base::close_link_win(){

    add_link_win->hide();
}


void base::add_link_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->add_link_cb();
}

void base::add_link_cb(){

    container* root = &(this->root);
            
    std::stringstream s;
    s<<start_add_link->value();

    int start = -1;
    int end = -1;

    try{
        start = stoi(s.str());
    }
    catch (...){
        start = -1;
        end = -1;
    }

    s.str("");
    s<<end_add_link->value();
    

    try{
        end = stoi(s.str());
    }
    catch (...){
        start = -1;
        end = -1;
    }

 
    draw->deselect();
    
    
    if ( (start>=0) && (start < static_cast<int>(root->current->points.size()) )  &&  (end>=0)  &&  (end < static_cast<int> (root->current->points.size()) )  &&  (start!=end)){
        
        int found=0;
        for (unsigned int i=0;i<root->current->adj_list.size();i++){
            
            int p1 = root->current->adj_list[i].start;
            int p2 = root->current->adj_list[i].end;
            
            if (((p1==start)&&(p2==end))||((p1==end)&&(p2==start))){
                found=1;
            }
            
        }
        
        if (!found){
            vertex temp;
            temp.start=start;
            temp.end=end;
            adj_mutex.lock();
            root->current->adj_list.push_back(temp);
            adj_mutex.unlock();
            push_time();
            std::stringstream s;
            s<<"Added edge between vertices "<<start<<" and "<<end<<"."<<std::endl;
            push_output(s.str());
            warning_map_cb();
            prepare_undo();
        }
        else{
            push_time();
            std::stringstream s;
            s<<"Edge already exists - edge not added."<<std::endl;
            push_output(s.str());
        }
        
        
    }
    else{
        push_time();
        push_output("Edge not added. Check input.\n");
    }
    
    draw->redraw();
    
}

void base::remove_link_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->remove_link_cb();
}


void base::remove_link_cb(){
    
    container* root = &(this->root);        


    int start = -1;
    int end = -1;

    
    std::stringstream s;
    s<<start_add_link->value();
    
    try{
        start = stoi(s.str());
    }
    catch (...){
        start = -1;
        end = -1;
    }

    s.str("");
    s<<end_add_link->value();
    
    try{
        end = stoi(s.str());
    }
    catch (...){
        start = -1;
        end = -1;
    }

    draw->deselect();
    
    if ((start >= 0) && (start < static_cast<int>(root->current->points.size()) ) && (end >= 0) && (end < static_cast<int> (root->current->points.size()) ) && (start!=end)){
        
        int found=-1;
        for (unsigned int i=0;i<root->current->adj_list.size();i++){
            
            int p1 = root->current->adj_list[i].start;
            int p2 = root->current->adj_list[i].end;
            
            if (((p1==start)&&(p2==end))||((p1==end)&&(p2==start))){
                found=i;
            }
            
        }
        
        if (found>-1){
            adj_mutex.lock();
            root->current->adj_list.erase(root->current->adj_list.begin()+found);
            adj_mutex.unlock();
            push_time();
            std::stringstream s;
            s<<"Removed edge between vertices "<<start<<" and "<<end<<"."<<std::endl;
            push_output(s.str());
            warning_map_cb();
            prepare_undo();
        }
        else{
            push_time();
            push_output("Edge not removed. Check input.\n");

        }

    }
    else{
        push_time();
        push_output("Edge not removed. Check input.\n");
    }
    
    draw->redraw();
}



void base::remove_vertex_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->remove_vertex_cb();
}


void base::remove_vertex_cb(){
    
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

    int vert = -1;
    
    std::stringstream s;
    s<<remove_vert->value();

    try{
        vert = stoi(s.str());
    }
    catch (...){
        vert = -1;
        push_time();
        push_output("Vertex not removed. Check input.\n");
        return;
    }

    draw->deselect();
    
    if ((vert >= 0) && (vert < static_cast<int>(root->current->points.size()))){
    

        int ii = vert;

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
        points->erase(points->begin()+ii);
        points_mutex.unlock();

        metrics_mutex.lock();
        root->current->route.clear();
        root->current->plot_metric.clear();
        root->current->max_plot_metric = 0.0;
        root->current->min_plot_metric = 0.0;
        metrics_mutex.unlock();
        //need to add deletion/adaption of locations associated with point reomved and points that come after...

        int loc_change=0;

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
        

        push_time();
        std::stringstream ss;
        ss<<"Vertex "<<ii<<" and all associated edges and location markers removed. Note vertex numbers have now changed."<<std::endl;
        push_output(ss.str());

        draw->selected_matrix[1]=-1;
        warning_map_cb();

        if (loc_change>0){
            draw->show_warning_visibility();
        }

        prepare_undo();

    }
    else{
        push_time();
        push_output("Vertex not removed. Check input.\n");
    }

    draw->redraw();
}


void base::add_vertex_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->add_vertex_cb();
}


void base::add_vertex_cb(){
    
    container* root = &(this->root);        

    double x= 0.0,y=0.0;

    
    std::stringstream s;
    s<<x_vert->value();
    std::string str1;
    str1 = s.str();
    char * ex;
    errno = 0;

    try{
        x = std::strtod(str1.c_str(), &ex);
    }
    catch(...){
        push_time();
        push_output("Error reading in vertex location.\n");      
        return;   
    }

    if (*ex != '\0' ||  // error, we didn't consume the entire string
        errno != 0 )   // error, overflow or underflow
    {
        push_time();
        push_output("Error reading in vertex location.\n");      
        return;  
    } 

    s.str("");
    s<<y_vert->value();
    str1 = s.str();
    char * ey;
    errno = 0;

    try{
        y = std::strtod(str1.c_str(), &ey);
    }
    catch(...){
        push_time();
        push_output("Error reading in vertex position.\n");      
        return;   
    }

    if (*ey != '\0' ||  // error, we didn't consume the entire string
        errno != 0 )   // error, overflow or underflow
    {
        push_time();
        push_output("Error reading in vertex position.\n");      
        return;  
    }  

    int floor = floor_choice->value()-1;

    if ((floor<0)||(floor>    static_cast<int>(root->current->floor_index.size() )    )){
        push_time();
        push_output("Error, select a floor.\n");      
        return;  
    }

    vec temp;

    temp.floor = floor;
    temp.z=root->current->floor_index[floor];
    temp.x=x;
    temp.y=y;

    points_mutex.lock();
    root->current->points.push_back(temp);
    points_mutex.unlock();

    push_time();
    std::stringstream ss;
    ss<<"Vertex added at x: "<<x<<" y: "<<y<<" on floor "<<floor<<"."<<std::endl;
    push_output(ss.str());

    show_warning_map_cb(this);
    prepare_undo();

    draw->redraw();
}


void base::add_loc_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->add_loc_cb();
}


void base::add_loc_cb(){
    
    container* root = &(this->root);        

    double x= 0.0,y=0.0;
    
    std::stringstream s;
    s<<x_loc->value();
    std::string str1;
    str1 = s.str();
    char * ex;
    errno = 0;

    try{
        x = std::strtod(str1.c_str(), &ex);
    }
    catch(...){
        push_time();
        push_output("Error reading in location marker position.\n");      
        return;   
    }

    if (*ex != '\0' ||  // error, we didn't consume the entire string
        errno != 0 )   // error, overflow or underflow
    {
        push_time();
        push_output("Error reading in location marker position.\n");      
        return;  
    } 

    s.str("");
    s<<y_loc->value();
    str1 = s.str();
    char * ey;
    errno = 0;

    try{
        y = std::strtod(str1.c_str(), &ey);
    }
    catch(...){
        push_time();
        push_output("Error reading in location marker position.\n");          
        return;   
    }

    if (*ey != '\0' ||  // error, we didn't consume the entire string
        errno != 0 )   // error, overflow or underflow
    {
        push_time();
        push_output("Error reading in location marker position.\n");          
        return;  
    }  

    int floor = floor_choice->value()-1;

    if ((floor<0)||(floor>    static_cast<int>(root->current->floor_index.size() )    )){
        push_time();
        push_output("Error, select a floor.\n");      
        return;  
    }


    int loc = loc_choice->value();
    if ((loc<0)||(loc>8)){
        push_time();
        push_output("Error with location type.\n");
        return;
    }

    std::vector<location> *loc_ptr = &root->current->locations[loc];

    location temp;

    temp.loc.floor = floor;
    temp.loc.z=root->current->floor_index[floor];
    temp.loc.x=x;
    temp.loc.y=y;

    double min=9e90; 
    int ii=0;
    int found =0;

    for (unsigned int i=0;i<root->current->points.size();i++){

        if (root->current->points[i].floor==floor){
            found=1;            
        }
        
        if (root->current->points[i].floor==temp.loc.floor){
            double r= sqrt((root->current->points[i].x-temp.loc.x)*(root->current->points[i].x-temp.loc.x)+(root->current->points[i].y-temp.loc.y)*(root->current->points[i].y-temp.loc.y));
            if (r<min){
                ii=i;
                min=r;
            }
        }
    }
    

    if (!found){
        push_time();
        push_output("No spatial graph vertices found on this floor! Complete the spatial graph to continue.\n");
        return;
    }


    temp.point=ii;                            
    
    temp.segment=0;
    temp.is_desk=0;
    if (loc>0){
        temp.is_desk = 1;
    }
    
    temp.label="";

    locations_mutex.lock();
    loc_ptr->push_back(temp);
    locations_mutex.unlock();

    show_locations[loc]->value(1);

    push_time();
    std::stringstream ss;
    if (loc==0){
        ss<<"Origin ";
    }
    else{
        ss<<"Destination "<<loc<<" ";
    }
    ss<<"location marker added at x: "<<x<<" y: "<<y<<", associated with vertex "<<temp.point<<", on floor "<<floor<<"."<<std::endl;
    push_output(ss.str());

    show_warning_map_cb(this);
    prepare_undo();

    draw->redraw();
}

void base::remove_loc_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->remove_loc_cb();
}


void base::remove_loc_cb(){
    
    container* root = &(this->root); 

    int index = -1;

    std::stringstream s;
    s<<loc_remove_index->value();

    try{
        index = stoi(s.str());
    }
    catch (...){
        index = -1;
        push_time();
        push_output("Location marker not removed. Check input.\n");
        return;
    }


    int loc = loc_choice->value();
    if ((loc<0)||(loc>8)){
        push_time();
        push_output("Error with location type.\n");
        return;
    }

    draw->deselect();

    std::vector<location> *loc_ptr = &root->current->locations[loc];

    
    if ((index >= 0) && (index < static_cast<int>(loc_ptr->size()))){
    
        locations_mutex.lock();

        loc_ptr->erase(loc_ptr->begin()+index);

        locations_mutex.unlock();

        std::stringstream ss;
        if (loc==0){
            ss<<"Origin location marker ";
        }
        else{
            ss<<"Destination "<<loc<<" location marker ";
        }
        ss<<"with index "<<index<<" removed. Note indexing has now changed."<<std::endl;

        push_time();
        push_output(ss.str());

        warning_map_cb();
        prepare_undo();


    }
    else{
        push_time();
        push_output("Location marker not removed. Check input.\n");
    }

    draw->redraw();
}
