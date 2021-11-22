/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/base.hpp"
#include "./include/selectwin.hpp"

void base::find_floor_extremes(){ //for drawing purposes
    const std::vector<double>& floor_index = root.current->floor_index; 
    double minz,maxz;
    if (floor_index.size()>0){
        minz = floor_index[0];
        maxz = minz;
        for (unsigned int i=0;i<floor_index.size();i++){
            if (floor_index[i]<minz){
                minz = floor_index[i];
            }
            if (floor_index[i]>maxz){
                maxz = floor_index[i];
            }
        }
        root.current->minimum_z=minz;
        root.current->maximum_z=maxz;
        return;
    }
    else{
        return;//shouldnt get here
    }
}


void base::floorplan_to_map_static(Fl_Widget* /*widget*/, void* data){
    base* program = static_cast<base*> (data);
    program->floorplan_to_map();
}

void base::floorplan_to_map(){
    

    if (root.calculating){
        calc_cb();
        return;
    }

    container* root = &(this->root);

    int ok = choice("Construct spatial graph from active layers/blocks of the floorplan for this floor? Lines constitute vertices connected by edges. Ends of lines less than 1e-6m apart are considered the same vertex.\nWarning:  This can be intractable when not applied to specifically designed sparse floorplans.","Proceed","Cancel",0,"Construct graph?");

    if (ok!=0){
        return;
    }
    
    int floor =  floor_choice->value()-1;


    if ((floor<0)&&(root->current->floor_index.size()==1)){
        Fl::lock();
        floor_choice->value(1);
        floor_choice->do_callback();
        Fl::unlock();
        push_time();
        push_output("Defaulting to floor 0.\n");
        floor=0;
    }
    else if (floor<0){
        push_time();
        push_output("Select a floor!\n");
        
        return;
    }
    
    push_time();
    push_output("Creating graph structures from active layers/blocks in current floor plan...\n");
    push_time();
    push_output("...this process can be extremely time consuming for large floor plans. Press Alt + C to cancel...\n");
    

    root->calculating=FLOORPLAN_CONVERT;
    calc_message_on();

    test_thread = std::thread(&base::floorplan_to_map_thread,this);
    test_thread.detach();

    
    
}


void base::floorplan_to_map_thread(){

    container* root = &(this->root);

    int floor =  floor_choice->value()-1;

    std::vector<vec> to_add;
    std::vector<vertex> to_link;

    root->killthread.reset();

    int kill = construct_graph_from_floorplan(to_add,to_link,floor);
    
    if (kill){

        root->killthread.reset();

        Fl::awake(calc_message_off_cb,this);

        Fl::lock();
        root->calculating=FREE;
        Fl::unlock();
        push_time();
        push_output("Cancelled.\n");
        return;

    }

    int old_size = root->current->points.size();
    
    points_mutex.lock();
    for (unsigned int i=0;i<to_add.size();i++){
        root->current->points.push_back(to_add[i]);
    }
    points_mutex.unlock();

    adj_mutex.lock();
    for (unsigned int i=0;i<to_link.size();i++){
        to_link[i].start+=old_size;
        to_link[i].end+=old_size;
        root->current->adj_list.push_back(to_link[i]);
    }
    adj_mutex.unlock();
    
    //floorplan_to_areas();

    push_time();
    std::stringstream s;
    s<<"...Created "<<to_add.size()<<" vertices and "<<to_link.size()<<" edges..."<<std::endl;
    push_output(s.str());
    push_time();
    push_output("...done.\n");
  

    prepare_undo();
    Fl::awake(calc_message_off_cb,this);

    Fl::lock();
    root->calculating=FREE;
    //show_warning_map();
    Fl::unlock();
    Fl::awake(show_warning_map_cb,this); 
    Fl::awake(&redraw_from_thread,this);

}

int base::construct_graph_from_floorplan(std::vector<vec> &p,std::vector<vertex> &e, int floor){

    container* root = &(this->root);
    
    double zval = root->current->floor_index[floor];

    std::vector<wall> plan_as_walls;

    floorplan_to_barriers(&plan_as_walls,root,floor,0);

    std::vector<vec> to_add;
    std::vector<vertex> to_link;

    double x1,y1,x2,y2;

    for (int ii=plan_as_walls.size()-1;ii>=0;ii--){

        if (root->killthread.get()){
            root->killthread.reset();
            return 1;
        }

        x1= plan_as_walls[ii].start.x;
        y1= plan_as_walls[ii].start.y;
        x2= plan_as_walls[ii].end.x;
        y2= plan_as_walls[ii].end.y;    

        int p1=-1,p2=-1;
            
        for (unsigned int i=0;i<to_add.size();i++){
            if ((equal(to_add[i].x,x1))&&(equal(to_add[i].y,y1))){
             
                p1=i;
                
            }
            
            if ((equal(to_add[i].x,x2))&&(equal(to_add[i].y,y2))){
                
                p2=i;
                
            }
        }
        
        if ((equal(x1,x2))&&(equal(y1,y2))){
                
            p2=0;
            p1=0;
            
        }

        if ((p1!=p2)||((p1<0)&&(p2<0))){
                        
            if (p1<0){
                vec temp;
                temp.x=x1;
                temp.y=y1;
                temp.z=zval;
                temp.floor=floor;
                
                to_add.push_back(temp);
                p1=to_add.size()-1;
            }
            
            if (p2<0){
                vec temp;
                temp.x=x2;
                temp.y=y2;
                temp.z=zval;
                temp.floor=floor;
                
                to_add.push_back(temp);
                p2=to_add.size()-1;
            }

            vertex temp;
            temp.start=p1;
            temp.end=p2;
            to_link.push_back(temp);                

        }
        
        plan_as_walls.pop_back();

    }

    for (int i=to_link.size()-1;i>=0;i--){
        
        int found =0;
        for (int j=0;j<i;j++){
            
            if (((to_link[i].start==to_link[j].start)&&(to_link[i].end==to_link[j].end))||((to_link[i].end==to_link[j].start)&&(to_link[i].start==to_link[j].end))){
                found=1;
            }
            
        }
        if (found){
            to_link[i]=to_link.back();
            to_link.pop_back();
        }
    }


    p = std::move(to_add);
    e = std::move(to_link);
    return 0;

}

void base::floorplan_to_areas_static(Fl_Widget* /*widget*/, void* data){
    base* program = static_cast<base*> (data);
    program->floorplan_to_areas();
}

void base::floorplan_to_areas(){
    
    if (root.calculating){
        calc_cb();
        return;
    }

    container* root = &(this->root);

    int ok = choice("Construct floor areas from the active layers/blocks of the floorplan for this floor? Isolated cycles formed from floorplan objects are considered areas.\nWarning: This can be intractable when not applied to specifically designed sparse floorplans.","Proceed","Cancel",0,"Construct areas?");

    if (ok!=0){
        return;
    }
    
    int floor =  floor_choice->value()-1;


    if ((floor<0)&&(root->current->floor_index.size()==1)){
        Fl::lock();
        floor_choice->value(1);
        floor_choice->do_callback();
        Fl::unlock();
        push_time();
        push_output("Defaulting to floor 0.\n");
        floor=0;
    }
    else if (floor<0){
        push_time();
        push_output("Select a floor!\n");
        
        return;
    }
    
    push_time();
    push_output("Creating floor areas from active layers/blocks in current floor plan...\n");
    push_time();
    push_output("...this process can be extremely time consuming for large floor plans. Press Alt + C to cancel...\n");
    

    root->calculating=FLOORPLAN_CONVERT;
    calc_message_on();

    test_thread = std::thread(&base::floorplan_to_areas_thread,this);
    test_thread.detach();

    
    
}

void base::floorplan_to_areas_thread(){
    
    container* root = &(this->root);
    
    int floor =  floor_choice->value()-1;
    
    std::vector<vec> floor_points;
    std::vector<vertex> floor_links;

    root->killthread.reset();

    int kill = construct_graph_from_floorplan(floor_points,floor_links,floor); ////// NEED TO DEAL WITH KILLED THREAD
    
    if (kill){

        root->killthread.reset();

        Fl::awake(calc_message_off_cb,this);

        Fl::lock();
        root->calculating=FREE;
        Fl::unlock();
        push_time();
        push_output("Cancelled.\n");
        return;

    }

    //construct degree listt
    int culling = 1; // removing all non degree 2 vertices (and associated edges)

    while (culling){ //have to loop, becaue removing vertices means removing edges which might then change the degree of another node

        if (root->killthread.get()){
            root->killthread.reset();
            Fl::awake(calc_message_off_cb,this);
            Fl::lock();
            root->calculating=FREE;
            Fl::unlock();
            push_time();
            push_output("Cancelled.\n");
            return;
        }

        std::vector<int> degree(floor_points.size(),0);

        for (unsigned int i=0;i<floor_links.size();i++){
            degree[floor_links[i].start]++;
            degree[floor_links[i].end]++;
        }

        std::vector<int> not_suitable;

        for (unsigned int i=0;i<degree.size();i++){
            if (degree[i]!=2){
                not_suitable.push_back(i);
            }
        }
        

        std::vector<int> point_list(floor_points.size());
        std::vector<int> temp_list(floor_points.size());
        std::vector<int> to_delete(floor_points.size(),0); //need a lookup list of which points are deleted in order to avoid searching later


        for (unsigned i=0; i<floor_points.size();i++){
            point_list[i]=i;
            temp_list[i]=i;
        }

        int sum=0;

        for (int j=not_suitable.size()-1;j>=0;j--){

            int i = not_suitable[j];
            to_delete[i]=1;

            int t = temp_list[floor_points.size()-1-sum]; //emulate the swap and pop_back delete method.
            temp_list[floor_points.size()-1-sum] = temp_list[i]; //temp list will be the new order once swapped and deleted
            temp_list[i]=t;
            sum++;   //last sum elements will be removed once actually swapped
        
        }


        for (unsigned int i=0;i<floor_points.size();i++){ //   point_list is then maps the old point index to the new point index after swapping 
            point_list[temp_list[i]]=i;                   //   e.g. if the 2nd element of temp_list is 6 then the original point 6 can be found at new index 2.
        }                                                 //   so when looing for original point 6 we find it at new index point_list[2]

        ////// end arrange


        if (not_suitable.size()==0){ //nothing more to remove
            culling = 0;
            break;
        }

        ///// begin reassign and erase


        for (int i=floor_links.size()-1;i>=0;i--){//need to count backwards as resizing

            if ((to_delete[floor_links[i].start])||(to_delete[floor_links[i].end])){
                    
                floor_links[i] = floor_links.back();
                floor_links.pop_back();
            }
        }

        for (int i=floor_links.size()-1;i>=0;i--){ //update remaining indices in the adj list

            floor_links[i].start = point_list[floor_links[i].start];
            floor_links[i].end = point_list[floor_links[i].end];
        }

        for (int j=not_suitable.size()-1;j>=0;j--){

            int i = not_suitable[j];
            floor_points[i]=floor_points.back();
            floor_points.pop_back();
        
        }

    }

    //we now have a graph formed of vertices that have only degree two.

    std::vector<std::vector<int> > point_lists(floor_points.size());
    
    for (unsigned int i=0;i<floor_links.size();i++){
        int ii = floor_links[i].start;
        int jj = floor_links[i].end;
        point_lists[ii].push_back(jj);
        point_lists[jj].push_back(ii);
    }


    // debgging check : should have no output
    // for (unsigned int i=0;i<point_lists.size();i++){
    //     if (point_lists[i].size()!=2){
    //         std::cout<<i<<" "<<point_lists[i].size()<<std::endl;
    //         return;
    //     }
    // }

    std::vector<int> used(floor_points.size(),0);

    int origin = 0;

    std::vector<std::vector<int> > loops;

    while(origin < static_cast<int>(floor_points.size())){
        int index = origin;
        std::vector<int> loop;
        if (!used[index]){
            used[index]=1;
            loop.push_back(index);
            int possible =1;
            while (possible){
                int nextA = point_lists[index][0];
                int nextB = point_lists[index][1];

                if (((nextA == origin)||(nextB==origin))&&(loop.size()>2)){
                    loops.push_back(loop); 
                    possible = 0;
                    continue;
                }
                else if ((used[nextA])&&(used[nextB])){
                    possible = 0;
                }
                else if (used[nextA]){
                    loop.push_back(nextB);
                    used[nextB]=1;
                    index = nextB;                    
                }
                else if (used[nextB]){
                    loop.push_back(nextA);
                    used[nextA]=1;
                    index = nextA;                    
                }
                else{
                    loop.push_back(nextA); // just pick one - start looping in a direction, it doesn't matter
                    used[nextA]=1;
                    index = nextA;   
                }
            }
        }
        origin++;
    }

    

    areas_mutex.lock();

    root->current->floorarea_polygons.pop_back();

    for (unsigned int i=0;i<loops.size();i++){

        std::vector<vec> add;
        for (unsigned int j=0;j<loops[i].size();j++){
            vec temp;
            temp.x = floor_points[loops[i][j]].x;
            temp.y = floor_points[loops[i][j]].y;
            temp.floor = floor;
            add.push_back(temp);
        }

        root->current->floorarea_polygons.push_back(add);
        root->current->floorarea_triangles.push_back(std::vector<triangle>(0));
        draw->ear_clipping(root->current->floorarea_polygons[root->current->floorarea_polygons.size()-1],&root->current->floorarea_triangles[root->current->floorarea_triangles.size()-1]);
        root->current->floorarea_type.push_back(0);
        draw->calculate_areas(&root->current->floorarea_polygons);   
    }
    root->current->floorarea_polygons.push_back(std::vector<vec>(0));

    areas_mutex.unlock();

    push_time();
    std::stringstream s;
    s<<"...Created "<<loops.size()<<" areas..."<<std::endl;
    push_output(s.str());
    push_time();
    push_output("...done.\n");
  

    prepare_undo();
    Fl::awake(calc_message_off_cb,this);

    Fl::lock();
    root->calculating=FREE;
    show_floor_areas->value(1);
    show_floor_areas->do_callback();
    Fl::unlock();

    Fl::awake(&redraw_from_thread,this);
}

void base::clear_dxf_cb_static(Fl_Widget* /*widget*/, void* data){
    
    base* program = static_cast<base*> (data);

    program->clear_dxf_cb();

}

void base::clear_dxf_cb(){

    if (root.calculating){
        push_time();
        push_output("Busy! Wait for current process to finish.\n");
        calc_cb();

        return;
    }

    if (floor_choice->value()==0){
        std::stringstream s;
        s<<"Choose a floor to remove thefloor plan from!\n";
        push_time();
        push_output(s.str());
        return;
    }
    else{

        std::vector<plan>& floor_plans = root.current->floor_plans;

        root.calculating=FLOORPLAN_UTILITY;
        calc_message_on();

        plan floor_plan;
        int floor = floor_choice->value()-1;


        if ((floor>=0)&&(static_cast<unsigned int>(floor)<floor_plans.size())){

            plans_mutex.lock();    
            floor_plans[floor]=floor_plan;
            plans_mutex.unlock();

            draw->first_draw=1;
            prepare_undo();

            std::stringstream s;
            s<<"Removed floorplan from floor "<<floor<<std::endl;

            push_time();
            push_output(s.str());

        }

        root.calculating=FREE;
        calc_message_off();
        draw->redraw();
        return;
    }

}


void base::open_dxf_cb_static(Fl_Widget* /*widget*/, void* data){
    
    base* program = static_cast<base*> (data);

    program->open_dxf_cb();
}


void base::open_dxf_cb(){
    
    if (root.calculating){
        push_time();
        push_output("Busy! Wait for current process to finish.\n");
        calc_cb();
        return;
    }
    
    if ((floor_choice->value()==0)&&(root.current->floor_index.size()==1)){
        Fl::lock();
        floor_choice->value(1);
        floor_choice->do_callback();
        Fl::unlock();
        push_time();
        push_output("Defaulting to floor 0.\n");
    }
    else  if (floor_choice->value()==0){
        std::stringstream s;
        s<<"Choose a floor to be associated with the floor plan!\n";
        push_time();
        push_output(s.str());
        return;
    }
    
        
    Fl_Native_File_Chooser fnfc;
    fnfc.title("Open floorplan");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    fnfc.filter("*.dxf");
    if ( fnfc.show() ) return;
    
    std::string str=std::string(fnfc.filename());
    root.current->dxf_file_string=str;
    
    root.calculating=LOADING_DXF;
    calc_message_on();

    test_thread = std::thread(&base::open_dxf_thread_static,this);
    test_thread.detach();

    if (!root.draw_floorplan){
        Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[FLOORPLAN_INDEX]);   
        root.draw_floorplan=1;
        temp->set(); 
    }
    draw->redraw();

    
}

void* base::open_dxf_thread_static(void* data){
    base* program = static_cast<base*> (data);

    program->open_dxf_thread();
    return NULL;
}


void base::units_ok_static(Fl_Widget*,void* data){
    base* program = static_cast<base*>(data);

    program->root.calculating = FLOORPLAN_UTILITY;

    double scale_factor = 1.0;
    switch(program->unit_choice->value()){
        case 0: scale_factor = 1.0; break;
        case 1: scale_factor = 0.01; break;
        case 2: scale_factor = 0.001; break;
        case 3: scale_factor = 1000.0; break;
        case 4: scale_factor = 1.0e-6;; break;
        case 5: scale_factor = 0.0254; break;
        case 6: scale_factor = 0.3048; break;
        case 7: scale_factor = 0.9144; break;
        case 8: scale_factor = 1609.344; break;
    }

    int floor = program->floor_choice->value()-1;

    
    program->root.current->holding_plan.scale(scale_factor);

    program->plans_mutex.lock();
    program->root.current->floor_plans[floor] = std::move(program->root.current->holding_plan);
    program->root.current->floor_plans[floor].convert_to_lines();    //sets fast_plan internally
    program->plans_mutex.unlock();

    program->push_time();
    program->push_output("...done.\n");

    
    if (!program->root.current->floor_plans[floor].fast_plan){
        
        std::stringstream s;
        program->push_time();
        s<<"WARNING! Floorplan is very large/contains large numbers of objects."<<std::endl;
        program->push_output(s.str());
        s.str("");
        s<<"------------------- >> "<<"Fixed resolution drawing of the floorplan is disabled and replaced with a dynamic method."<<std::endl;
        program->push_output(s.str());
        s.str("");
        s<<"------------------- >> "<<"This can occur when floorplan units were not imported correctly."<<std::endl;
        program->push_output(s.str());

    }

    plan temp_plan;
    program->root.current->holding_plan = std::move(temp_plan);
    program->prepare_undo();
    
    program->draw->first_draw=1;
    program->draw->redraw();
    program->root.calculating = FREE;
    program->units_win->hide();
    program->unit_choice->value(2); // back to mm

}

void base::units_cancel_static(Fl_Widget*,void* data){
    
    base* program = static_cast<base*>(data);
    program->root.calculating = FLOORPLAN_UTILITY;
    plan temp_plan;
    program->root.current->holding_plan = temp_plan;
    program->draw->redraw();
    program->root.calculating = FREE;
    program->units_win->hide();

}


void base::get_units_from_thread(void* data){
    
    base* program = static_cast<base*>(data);

    program->units_win->resize(program->win->x()+0.5*program->win->w()-170,program->win->y()+0.5*program->win->h()-100,340,150);
    program->units_win->show();
    program->calc_message_off();
    program->root.calculating=FREE;
}

void* base::open_dxf_thread(){
    
    std::string str = root.current->dxf_file_string;

    std::vector<plan>& floor_plans = root.current->floor_plans;
    
    std::stringstream s;
    push_time();
    s<<"Opening DXF file `"<<str<<"`..."<<std::endl;
    push_output(s.str());
    
    dxf_parser dxf_object;

    int success =0;
    try{
        success = dxf_object.read_in(str.c_str());
    }
    catch(...){
        push_time();
        push_output("Encountered an error reading in the DXF file. Cancelling attempt.\n");
    }
        
    plan floor_plan;

    int no_units=0;

    Fl::lock();
    draw->set_x_down_off();
    Fl::unlock();
    
    if (success){    

        floor_plan.layers_name = dxf_object.layers;
        floor_plan.layers_used= dxf_object.layers_used;
        floor_plan.blocks_used= dxf_object.blocks_used;
        
        
        floor_plan.layers.clear();
        std::vector<int>().swap(floor_plan.layers);
        
        for (unsigned int i=0;i<dxf_object.layers.size();i++){
            floor_plan.layers.push_back(1);
        }

        floor_plan.blocks_active.clear();
        std::vector<int>().swap(floor_plan.blocks_active);
        
        for (unsigned int i=0;i<dxf_object.block_definitions.size();i++){
            floor_plan.blocks_active.push_back(1);
        }
        
        floor_plan.lines_original=dxf_object.lines;
        floor_plan.lines=floor_plan.lines_original;//need this line!
        
        floor_plan.arcs_original=dxf_object.arcs;
        floor_plan.arcs=floor_plan.arcs_original;

        floor_plan.ellipses_original=dxf_object.ellipses;
        floor_plan.ellipses=floor_plan.ellipses_original;
        
        floor_plan.circles_original=dxf_object.circles;
        floor_plan.circles=floor_plan.circles_original;
        
        floor_plan.splines_original=dxf_object.splines;
        floor_plan.splines=floor_plan.splines_original;
        
        
        floor_plan.blocks_original=dxf_object.blocks;
        floor_plan.blocks=floor_plan.blocks_original;//need this line! both for scale function in order to have .blocks and .lines intialised
        
        
        floor_plan.block_definitions_original=dxf_object.block_definitions; //in nested only
        floor_plan.block_definitions=floor_plan.block_definitions_original;
        
        //reference block number for nested only
        
        for (unsigned int i=0;i<floor_plan.block_definitions.size();i++){
            
            for (unsigned int j=0;j<floor_plan.block_definitions[i].nested_blocks.size();j++){
                
                floor_plan.block_definitions[i].nested_blocks[j].list_no=-1;
                
                for (unsigned int ii=0;ii<floor_plan.block_definitions.size();ii++){
                    
                    if(!strcmp(floor_plan.block_definitions[i].nested_blocks[j].name.c_str(),floor_plan.block_definitions[ii].name.c_str())){
                        
                        floor_plan.block_definitions[i].nested_blocks[j].list_no=ii;
                        
                    }
                    
                }
                
            }
        }
        
        
        for (unsigned int i=0;i<floor_plan.blocks.size();i++){
            
            floor_plan.blocks[i].list_no=-1;
            
            for (unsigned int ii=0;ii<floor_plan.block_definitions.size();ii++){
                
                if(!strcmp(floor_plan.blocks[i].name.c_str(),floor_plan.block_definitions[ii].name.c_str())){
                    
                    floor_plan.blocks[i].list_no=ii;
                    
                }
                
            }
            
        }
        //end reference block number for nested only
        
        floor_plan.unit_type=dxf_object.units;

        no_units = floor_plan.scale(this);

        int floor = floor_choice->value()-1;

        if (!no_units){

            if (floor>=0){

                plans_mutex.lock();
                
                floor_plans[floor]=std::move(floor_plan);
                floor_plans[floor].convert_to_lines(); //sets fast_plan internally

                plans_mutex.unlock();

                s.str("");

                std::string unit_string;
                switch (floor_plan.unit_type){
                    case 1: unit_string = "inches";break;
                    case 2: unit_string = "feet";break;
                    case 3: unit_string = "miles";break;
                    case 4: unit_string = "mm";break;
                    case 5: unit_string = "cm";break;
                    case 6: unit_string = "m";break;
                    case 7: unit_string = "km";break;
                    case 8: unit_string = "microinches";break;
                    case 9: unit_string = "mils (milliinches)";break;
                    case 10:unit_string = "yards";break;
                    case 11:unit_string = "angstroms";break;
                    case 12:unit_string = "nanonmeters";break;
                    case 13:unit_string = "microns";break;
                    case 14:unit_string = "decimeters";break;
                    case 15:unit_string = "decameters";break;
                    case 16:unit_string = "hectometers";break;
                    case 17:unit_string = "gigameters";break;
                    case 18:unit_string = "AUs";break;
                    case 19:unit_string = "light years";break;
                    case 20:unit_string = "parsecs";break;
                }

                s<<"Detected units as "<<unit_string<<"."<<std::endl;
                push_time();
                push_output(s.str());

                prepare_undo();
                s.str("");
                s<<"...done."<<std::endl;
                push_time();
                push_output(s.str());

                if (!floor_plans[floor].fast_plan){
        
                    std::stringstream s;
                    push_time();
                    s<<"WARNING! Floorplan is very large/contains large numbers of objects."<<std::endl;
                    push_output(s.str());
                    s.str("");
                    s<<"------------------- >> "<<"Fixed resolution drawing of the floorplan is disabled and replaced with a dynamic method."<<std::endl;
                    push_output(s.str());
                    s.str("");
                    s<<"------------------- >> "<<"This can occur when floorplan units were not imported correctly."<<std::endl;
                    push_output(s.str());
                }
            }

            Fl::awake(calc_message_off_cb,this);
            Fl::lock();
            root.calculating=FREE;
            draw->first_draw=1;
            Fl::unlock();
            
            Fl::awake(&redraw_from_thread,this);
            return NULL;
        }
        else{
        
            s.str("");
            s<<"Warning: No units detected in DXF file."<<std::endl;
            push_time();
            push_output(s.str());

            plans_mutex.lock();
            root.current->holding_plan = std::move(floor_plan);
            plans_mutex.unlock();

            Fl::lock();
            root.calculating=FREE;
            Fl::unlock();
            
            Fl::awake(get_units_from_thread,this);
            return NULL;
        
        }  
    }
    else{
        
        s.str("");
        s<<"Error reading `"<<str<<"`. File not read!!"<<std::endl;
         Fl::lock();
        root.calculating=FREE;
        Fl::unlock();
        push_time();
        push_output(s.str());
    }

    return NULL;
}


void base::scale_floorplan_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->scale_floorplan_cb();
}


void base::scale_floorplan_cb(){
    
    container* root = &(this->root);
    std::vector<plan>& floor_plans = root->current->floor_plans;
    
    int floor=floor_choice->value();

    std::stringstream s;
    s<<scale_floorplan->value();
    std::string str1;
    str1 = s.str();
    char * e;
    errno = 0;
    double scale = std::strtod(str1.c_str(), &e);

    if (*e != '\0' ||  // error, we didn't consume the entire string
        errno != 0 )   // error, overflow or underflow
    {
        push_time();
        push_output("Error reading in floorplan scaling factor.\n");      
        return;  
    }


    if ((floor)&&(scale>(1e-10))){
        
        plans_mutex.lock();
        floor_plans[floor-1].scale(scale);
        plans_mutex.unlock();

        scale_floorplan->value("1.00");
        std::stringstream s;
        s<<"Floorplan for floor "<<floor-1<<" scaled by factor of "<<scale<<".\n";
        push_time();
        push_output(s.str());


        plans_mutex.lock();
        if (root->current->floor_plans[floor-1].fast_plan){
            floor_plans[floor-1].convert_to_lines();
            if (!floor_plans[floor-1].fast_plan){
                std::stringstream s;
                push_time();
                s<<"WARNING! Scaling of floorplan on floor "<<floor -1 <<" has resulted in a very large plan."<<std::endl;
                push_output(s.str());
                s.str("");
                s<<"------------------- >> "<<"Fixed resolution drawing of the floorplan is disabled and replaced with a dynamic method."<<std::endl;
                push_output(s.str());
                s.str("");
                s<<"------------------- >> "<<"This can occur when floorplan units were not imported correctly."<<std::endl;
                push_output(s.str());
                s.str("");
                s<<"------------------- >> "<<"You can undo this change (Ctrl + Z), load a new floorplan, or continue using the dynamic method."<<std::endl;
                push_output(s.str());
            }
        }
        plans_mutex.unlock();

        prepare_undo();
        draw->first_draw=1;
        draw->redraw();
        
    }
    else if(floor){
        scale_floorplan->value("1.00");
        push_time();
        push_output("Not scaled! Check scaling parameter!\n");
    }
    else{
        scale_floorplan->value("1.00");
        std::stringstream s;
        s<<"Select a floor!\n";
        push_time();
        push_output(s.str());
    }

    Fl::focus(win);    
}


void base::shiftx_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->shiftx_cb();
}

void base::shifty_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->shifty_cb();
}



void base::shiftx_cb(){
    
    container* root = &(this->root);
    std::vector<plan>& floor_plans = (root->current->floor_plans);
    
    if (floor_choice->value()>0){

        int f = floor_choice->value()-1;
        
        std::stringstream s;
        s<<xshift->value();
        std::string str1;
        str1 = s.str();
        char * e;
        errno = 0;
        double x = std::strtod(str1.c_str(), &e);

        if (*e != '\0' ||  // error, we didn't consume the entire string
            errno != 0 )   // error, overflow or underflow
        {
            push_time();
            push_output("Error reading in x shift.\n");
            return;
        }

        plans_mutex.lock();
        floor_plans[f].shift_x=x;
        if (root->current->floor_plans[f].fast_plan){
            floor_plans[f].convert_to_lines();
        }
        plans_mutex.unlock();
        s.str("");
        s<<"x-shift for floor "<<f<<" set to "<<std::fixed<<std::setprecision(3)<<x<<std::endl;
        push_time();
        push_output(s.str().c_str());
        prepare_undo();
        draw->redraw();
        
    }
    else{
        std::stringstream s;        
        xshift->value("0.00");
        s.str("");
        s<<"Select a floor!\n";
        push_time();
        push_output(s.str());
    }
    Fl::focus(win);
}

void base::shifty_cb(){
    
    container* root = &(this->root);
    std::vector<plan>& floor_plans = (root->current->floor_plans);
    
    if (floor_choice->value()>0){
        
        int f = floor_choice->value()-1;

        std::stringstream s;
        s<<yshift->value();
        std::string str1;
        str1 = s.str();
        char * e;
        errno = 0;
        double y = std::strtod(str1.c_str(), &e);

        if (*e != '\0' ||  // error, we didn't consume the entire string
            errno != 0 )   // error, overflow or underflow
        {
            push_time();
            push_output("Error reading in x shift.\n");
            return;
        }

        plans_mutex.lock();
        floor_plans[f].shift_y=y;
        if (root->current->floor_plans[f].fast_plan){
            floor_plans[f].convert_to_lines();
        }
        plans_mutex.unlock();

        s.str("");
        s<<"y-shift for floor "<<f<<" set to "<<std::fixed<<std::setprecision(3)<<y<<std::endl;
        push_time();
        push_output(s.str().c_str());
        prepare_undo();
        draw->redraw();
    }
    else{
        std::stringstream s;
        yshift->value("0.00");
        s.str("");
        s<<"Select a floor!\n";
        push_time();
        push_output(s.str());
    }
    Fl::focus(win);
}


void base::zfloor_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->zfloor_cb();
}

void base::zfloor_cb(){
    
    if (root.calculating){
        calc_cb();
        return;
    }
    
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
    
    std::vector<wall>& walls_standing = (root->current->walls_standing);
    std::vector<wall>& walls_sitting = (root->current->walls_sitting);
    std::vector<double>& floor_index = root->current->floor_index;
    
    if ( (floor_choice->value())  &&  (  static_cast<int> (floor_index.size()) > floor_choice->value()-1) ){
        
        
        {
            std::stringstream s;
            s<<zfloor->value();
            std::string str1;
            str1 = s.str();
            char * e;
            errno = 0;
            double x = std::strtod(str1.c_str(), &e);

            if (*e != '\0' ||  // error, we didn't consume the entire string
                errno != 0 )   // error, overflow or underflow
            {
                push_time();
                push_output("Error reading in floor height.\n");
                return;
            }
            floor_index[floor_choice->value()-1]=x;
            s.str("");
            s.precision(5);
            s<<"Height (z dimension) of floor "<<floor_choice->value()-1<<" set to "<<std::fixed << std::setprecision(4)<<x<<std::endl;
            push_time();
            push_output(s.str());
        }
        
        points_mutex.lock();

        for (unsigned int i=0;i<points->size();i++){
            
            if ((*points)[i].floor==floor_choice->value()-1){
                (*points)[i].z=floor_index[floor_choice->value()-1];
            }
            
        }

        points_mutex.unlock();

        walls_mutex.lock();
        
        for (unsigned int i=0;i<walls_sitting.size();i++){
            
            if (walls_sitting[i].start.floor==floor_choice->value()-1){
                
                walls_sitting[i].start.z=floor_index[floor_choice->value()-1];
                walls_sitting[i].end.z=floor_index[floor_choice->value()-1];
                
            }
            
        }

        
        for (unsigned int i=0;i<walls_standing.size();i++){
            
            if (walls_standing[i].start.floor==floor_choice->value()-1){
                
                walls_standing[i].start.z=floor_index[floor_choice->value()-1];
                walls_standing[i].end.z=floor_index[floor_choice->value()-1];
                
            }
            
        }

        walls_mutex.unlock();
        
        std::vector<location>* loc = NULL;
        

        locations_mutex.lock();
        
        for (unsigned int place_loop=0;place_loop<9;place_loop++){//;place_loop<9;place_loop++){
            
            switch (place_loop){
                case 0:loc=loc0;break;
                case 1:loc=loc1;break;
                case 2:loc=loc2;break;
                case 3:loc=loc3;break;
                case 4:loc=loc4;break;
                case 5:loc=loc5;break;
                case 6:loc=loc6;break;
                case 7:loc=loc7;break;
                case 8:loc=loc8;break;
            }
            
            for (unsigned int i=0;i<loc->size();i++){
                if ((*loc)[i].loc.floor==floor_choice->value()-1){
                    (*loc)[i].loc.z=floor_index[floor_choice->value()-1];
                }
            }
        }
        
        locations_mutex.unlock();
        
        find_floor_extremes();

        warning_map_cb();
        prepare_undo();
        draw->redraw();
    }
    Fl::focus(win);
}

void base::find_extrema_points(){
    
    container* root = &(this->root);
    std::vector<vec>* points = &(root->current->points);
    
    double &mid_x = root->current->mid_x;
    double &mid_y = root->current->mid_y;
    double &mid_z = root->current->mid_z;
    
    double &minimum_x = root->current->minimum_x;
    double &minimum_y = root->current->minimum_y;
    double &minimum_z = root->current->minimum_z;
    
    double &maximum_x = root->current->maximum_x;
    double &maximum_y = root->current->maximum_y;
    double &maximum_z = root->current->maximum_z;
    
    double minx=9e99,maxx=-9e99,miny=9e99,maxy=-9e99,minz=9e99,maxz=-9e99;
    
    
    for (unsigned int i=0;i<points->size();i++){
        
        if ((*points)[i].x<minx)minx=(*points)[i].x;
        if ((*points)[i].x>maxx)maxx=(*points)[i].x;
        if ((*points)[i].y<miny)miny=(*points)[i].y;
        if ((*points)[i].y>maxy)maxy=(*points)[i].y;
        if ((*points)[i].z<minz)minz=(*points)[i].z;
        if ((*points)[i].z>maxz)maxz=(*points)[i].z;
        
        
    }
    mid_x=0.5*(minx+maxx);
    mid_y=0.5*(miny+maxy);
    mid_z=0.5*(minz+maxz);
    
    
    minimum_z=minz;//-midz;
    maximum_z=maxz;//-midz;
    minimum_x=minx;//-midx;
    maximum_x=maxx;//-midx;
    minimum_y=miny;//-midy;
    maximum_y=maxy;//-midy;
        
}

void base::add_floor_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    if (program->root.calculating){
        program->push_time();
        program->push_output("No editing of constructions during calculations!\n");
        program->calc_cb();
        return;
    }
    program->add_floor_cb();
}

void base::remove_floor_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    if (program->root.calculating){
        program->push_time();
        program->push_output("No editing of constructions during calculations!\n");
        program->calc_cb();
        return;
    }    
    program->remove_floor_cb();
}


void base::remove_floor_cb(){
    
    container* root = &(this->root);
    std::vector<double>& floor_areas = root->current->floor_areas;
    std::vector<double>& floor_index = root->current->floor_index;
    std::vector<plan>& floor_plans = root->current->floor_plans;
    std::vector<vec>& points = root->current->points;
    std::vector<std::vector<location> >& loc = root->current->locations;
    std::vector<wall>& walls_sitting= root->current->walls_sitting;
    std::vector<wall>& walls_standing= root->current->walls_standing;
    std::vector<std::vector<triangle> >& floorarea_triangles = root->current->floorarea_triangles;
    std::vector<std::vector<vec> >& floorarea_polygons = root->current->floorarea_polygons;
    std::vector<int>& floorarea_type = root->current->floorarea_type;
    

    int i=choice("Remove current floor? All associated constructs will be deleted.","Delete","Cancel",0,"Remove floor?");

    if (i==1){ //cancel
        return;
    }
    else{

        if (static_cast<int>(floor_index.size())<2){
            push_time();
            push_output("Floor not deleted. Can't have less that one floor! \n");
            return;
        }
        int delete_floor = floor_choice->value()-1;

        remove_map_from_floor(delete_floor);

        metrics_mutex.lock();
        root->current->route.clear();
        root->current->plot_metric.clear();
        root->current->min_plot_metric = 0.0;
        root->current->max_plot_metric = 0.0;
        metrics_mutex.unlock();

        walls_mutex.lock();
        
        for (int ii=0;ii<2;ii++){
            std::vector<wall>* wall_vec = &walls_standing; //stop MSVC from complaining
             switch(ii){
                case 0:
                    wall_vec = &walls_sitting;
                    break;
                case 1:
                    wall_vec = &walls_standing;
                    break;

            }

            for (int i=wall_vec->size()-1;i>=0;i--){
                if ((*wall_vec)[i].start.floor==delete_floor){
                    //wall_vec->erase(wall_vec->begin()+i);
                    (*wall_vec)[i] = wall_vec->back(); //order doesnt matter running through backwards
                    wall_vec->pop_back();
                }
            }

        }
        
        walls_mutex.unlock();
        

        areas_mutex.lock();
        
        for (int ii=floorarea_polygons.size()-2;ii>=0;ii--){//last one is a dummy polygons for editing.

            if (static_cast<int>(floorarea_polygons[ii].size())>0){

                if (floorarea_polygons[ii][0].floor==delete_floor){
                    
                    floorarea_polygons[i] = floorarea_polygons.back();
                    floorarea_polygons.pop_back();

                    floorarea_triangles[i] = floorarea_triangles.back();
                    floorarea_triangles.pop_back();

                    floorarea_type[i] = floorarea_type.back();
                    floorarea_type.pop_back();

                    
                }
            }
        }
        
        floor_areas.erase(floor_areas.begin()+delete_floor);
        areas_mutex.unlock();
                
        plans_mutex.lock();
        
        floor_plans.erase(floor_plans.begin()+delete_floor);

        plans_mutex.unlock();
        
        ////end reassign and erasing floor

        //////adjust floors

        floors_mutex.lock();

        for (int i=floor_index.size()-1;i>=0;i--){
            if (i>=delete_floor){
                floor_index[i]=floor_index[i-1];
            }
        }

        floor_index.erase(floor_index.begin()+delete_floor);

        floors_mutex.unlock();

        
        //////end adjust floors

        ////// move floor indexed items into place

        areas_mutex.lock();

        for (int ii=floorarea_polygons.size()-2;ii>=0;ii--){//last one is a dummy polygons for editing.

            for (unsigned int j=0;j<floorarea_polygons[ii].size();j++){
                
                if (floorarea_polygons[ii][j].floor>=delete_floor){
                    floorarea_polygons[ii][j].floor--;
                    
                    floorarea_polygons[ii][j].z = floor_index[floorarea_polygons[ii][j].floor];
                }
                
            }

            for (unsigned int j=0;j<floorarea_triangles[ii].size();j++){
               
                floorarea_triangles[ii][j].z1 = floor_index[floorarea_polygons[ii][0].floor];
                floorarea_triangles[ii][j].z2 = floor_index[floorarea_polygons[ii][0].floor];
                floorarea_triangles[ii][j].z3 = floor_index[floorarea_polygons[ii][0].floor];
                
            }
        }

        areas_mutex.unlock();

        walls_mutex.lock();

        for (int ii=0;ii<2;ii++){
            std::vector<wall>* wall_vec = &walls_standing; //stop MSVC from complaining
             switch(ii){
                case 0:
                    wall_vec = &walls_sitting;
                    break;
                case 1:
                    wall_vec = &walls_standing;
                    break;
            }

            for (int i=wall_vec->size()-1;i>=0;i--){
                if ((*wall_vec)[i].start.floor>=delete_floor){
                    (*wall_vec)[i].start.floor--;
                    (*wall_vec)[i].end.floor--;
                    (*wall_vec)[i].start.z = floor_index[(*wall_vec)[i].start.floor];
                    (*wall_vec)[i].end.z = floor_index[(*wall_vec)[i].end.floor];

                }
            }

        }

        walls_mutex.unlock();

        points_mutex.lock();

        for (unsigned int i=0;i<points.size();i++){

            if (points[i].floor >= delete_floor){
                points[i].floor--;
                points[i].z = floor_index[points[i].floor];
            }

        }

        points_mutex.unlock();

        locations_mutex.lock();

        for (unsigned ii=0;ii<loc.size();ii++){
            std::vector<location>* loc_vec = &loc[ii];
            
            for (unsigned int i=0;i<loc_vec->size();i++){
                if ((*loc_vec)[i].loc.floor>=delete_floor){
                    (*loc_vec)[i].loc.floor--;
                    (*loc_vec)[i].loc.z = floor_index[(*loc_vec)[i].loc.floor];
                }
            }

        }

        locations_mutex.unlock();

        //// end move items into place

        floor_choice->clear();
        
        floor_choice->add("All");
        
        std::stringstream s;
        
        for (unsigned int i=0; i<floor_index.size();i++){
            s.str("");
            s<<i;
            floor_choice->add(s.str().c_str());
        }

        int new_floor = delete_floor + 1;
        if (new_floor>static_cast<int>(floor_index.size())){
            new_floor = floor_index.size();
        }

        floor_choice->value(new_floor);
        floor_choice_cb(0);
        soft_reset_fields();
        find_floor_extremes();
        draw->redraw();
        
        std::stringstream ss;
        ss<<"Floor "<<delete_floor<<" and all associated constructs removed."<<std::endl;
        push_time();
        push_output(ss.str());
        prepare_undo();

    }
}




std::vector<int> base::remove_map_from_floor(int floor){


    std::vector<int> to_ret;// {vertices removeed, locations_removed}

    ///arrange what to delete/move

    int delete_floor = floor;

    container* root = &(this->root);    
    std::vector<vec>& points = root->current->points;
    std::vector<std::vector<location> >& loc = root->current->locations;
    std::vector<vertex>& segments = root->current->adj_list;
    
    std::vector<int> point_list(points.size());
    std::vector<int> temp_list(points.size());


    for (unsigned i=0; i<points.size();i++){
        point_list[i]=i;
        temp_list[i]=i;
    }

    int sum=0;

    for (int i=points.size()-1;i>=0;i--){

        if (points[i].floor == delete_floor){

            int t = temp_list[points.size()-1-sum]; //emulate the swap and pop_back delete method.
            temp_list[points.size()-1-sum] = temp_list[i]; //temp list will be the new order once swapped and deleted
            temp_list[i]=t;
            sum++;   //last sum elements will be removed once actually swapped
        }

    }

    to_ret.push_back(sum);

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

        if ((points[segments[i].start].floor==delete_floor)||(points[segments[i].end].floor==delete_floor)){
                
            segments[i] = segments.back();
            segments.pop_back();
        }
    }

    for (int i=segments.size()-1;i>=0;i--){

        segments[i].start = point_list[segments[i].start];
        segments[i].end = point_list[segments[i].end];
    }

   
   int loc_sum=0;

    for (unsigned int ii=0;ii<loc.size();ii++){
        
        std::vector<location>* loc_vec  = &loc[ii];


        for (int i=loc_vec->size()-1;i>=0;i--){
            if ((*loc_vec)[i].loc.floor==delete_floor){
                (*loc_vec)[i] = loc_vec->back();
                loc_vec->pop_back();
                loc_sum++;
            }
        }

        for (unsigned int i=0;i<loc_vec->size();i++){
            loc_vec->at(i).point = point_list[loc_vec->at(i).point];
        }
    }

    to_ret.push_back(loc_sum);

    for (int i=points.size()-1;i>=0;i--){

        if (points[i].floor==delete_floor){
            points[i]=points.back();
            points.pop_back();
        }
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

void base::add_floor_cb(){
    
    container* root = &(this->root);
    std::vector<double>& floor_areas = root->current->floor_areas;
    std::vector<double>& floor_index = root->current->floor_index;
    std::vector<plan>& floor_plans = root->current->floor_plans;
    std::vector<vec>& points = root->current->points;
    std::vector<location>& loc0 = root->current->locations[0];
    std::vector<location>& loc1 = root->current->locations[1];
    std::vector<location>& loc2 = root->current->locations[2];
    std::vector<location>& loc3 = root->current->locations[3];
    std::vector<location>& loc4 = root->current->locations[4];
    std::vector<location>& loc5 = root->current->locations[5];
    std::vector<location>& loc6 = root->current->locations[6];
    std::vector<location>& loc7 = root->current->locations[7];
    std::vector<location>& loc8 = root->current->locations[8];
    std::vector<wall>& walls_sitting= root->current->walls_sitting;
    std::vector<wall>& walls_standing= root->current->walls_standing;
    std::vector<std::vector<vec> >& floorarea_polygons = root->current->floorarea_polygons;
    
    
    int i=choice("Insert new floor above or below current floor?","Cancel","Above","Below","Add floor?");

    if (i==0){ //cancel
        return;
    }
    else{
        
        int selection =floor_choice->value(); // value 1 = floor 0  etc.
        int selection_orig=selection;
    
        if (i==2){ //default is insert new floor above current floor , i==2 corresponds to inserting new floor below current floor
            selection--;
            // i.e. selection = floor+1 (insert above)
            //      selection = floor (insert below)
        }

        double height_delta = 3.0;

        double insert_height = floor_index[selection_orig-1];       

        floors_mutex.lock();

        floor_index.insert(floor_index.begin()+selection,insert_height);

        for (unsigned int ii=selection_orig;ii<floor_index.size();ii++){
            floor_index[ii]+=height_delta;
        }

        floors_mutex.unlock();

        std::stringstream ss;
        ss<<"New floor "<<selection<<" added with height "<<std::fixed << std::setprecision(4)<<floor_index[selection]<<std::endl;

        push_time();
        push_output(ss.str());

        floor_areas.insert(floor_areas.begin()+selection,0);
        
        std::stringstream s;
        s<<floor_index.size()-1;
        floor_choice->add(s.str().c_str());
        plan temp;

        plans_mutex.lock();

        floor_plans.insert(floor_plans.begin()+selection,temp);
        
        plans_mutex.unlock();

        floor_choice->value(selection+1);

        points_mutex.lock();
        adj_mutex.lock();
        walls_mutex.lock();
        locations_mutex.lock();
        areas_mutex.lock();
        floors_mutex.lock();
        plans_mutex.lock();


        for (unsigned int ii=0;ii<points.size();ii++){
            
            if (points[ii].floor>=selection){
                points[ii].floor++;
                points[ii].z = floor_index[points[ii].floor];
            }
        }
        for (unsigned int ii=0;ii<loc0.size();ii++){
            if (loc0[ii].loc.floor>=selection){
                loc0[ii].loc.floor++;
                loc0[ii].loc.z = floor_index[loc0[ii].loc.floor];
            }
        }
        for (unsigned int ii=0;ii<loc1.size();ii++){
            if (loc1[ii].loc.floor>=selection){
                loc1[ii].loc.floor++;
                loc1[ii].loc.z = floor_index[loc1[ii].loc.floor];
            }
        }
        for (unsigned int ii=0;ii<loc2.size();ii++){
            if (loc2[ii].loc.floor>=selection){
                loc2[ii].loc.floor++;
                loc2[ii].loc.z = floor_index[loc2[ii].loc.floor];
            }
        }
        for (unsigned int ii=0;ii<loc3.size();ii++){
            if (loc3[ii].loc.floor>=selection){
                loc3[ii].loc.floor++;
                loc3[ii].loc.z = floor_index[loc3[ii].loc.floor];
            }
        }
        for (unsigned int ii=0;ii<loc4.size();ii++){
            if (loc4[ii].loc.floor>=selection){
                loc4[ii].loc.floor++;
                loc4[ii].loc.z = floor_index[loc4[ii].loc.floor];
            }
        }
        for (unsigned int ii=0;ii<loc5.size();ii++){
            if (loc5[ii].loc.floor>=selection){
                loc5[ii].loc.floor++;
                loc5[ii].loc.z = floor_index[loc5[ii].loc.floor];
            }
        }
        for (unsigned int ii=0;ii<loc6.size();ii++){
            if (loc6[ii].loc.floor>=selection){
                loc6[ii].loc.floor++;
                loc6[ii].loc.z = floor_index[loc6[ii].loc.floor];
            }
        }
        for (unsigned int ii=0;ii<loc7.size();ii++){
            if (loc7[ii].loc.floor>=selection){
                loc7[ii].loc.floor++;
                loc7[ii].loc.z = floor_index[loc7[ii].loc.floor];
            }
        }
        for (unsigned int ii=0;ii<loc8.size();ii++){
            if (loc8[ii].loc.floor>=selection){
                loc8[ii].loc.floor++;
                loc8[ii].loc.z = floor_index[loc8[ii].loc.floor];
            }
        }
        for (unsigned int ii=0;ii<walls_sitting.size();ii++){
            if (walls_sitting[ii].start.floor>=selection){
                walls_sitting[ii].start.floor++;
                walls_sitting[ii].end.floor++;
                walls_sitting[ii].start.z = floor_index[walls_sitting[ii].start.floor];
                walls_sitting[ii].end.z = floor_index[walls_sitting[ii].start.floor];
            }
        }
        for (unsigned int ii=0;ii<walls_standing.size();ii++){
            if (walls_standing[ii].start.floor>=selection){
                walls_standing[ii].start.floor++;
                walls_standing[ii].end.floor++;
                walls_standing[ii].start.z = floor_index[walls_standing[ii].start.floor];
                walls_standing[ii].end.z = floor_index[walls_standing[ii].start.floor];
            }
        }
        
        for (unsigned int ii=0;ii<floorarea_polygons.size();ii++){
            if (static_cast<int>(floorarea_polygons[ii].size())>0){
                if (floorarea_polygons[ii][0].floor>=selection){
                    
                    for (unsigned int j=0;j<floorarea_polygons[ii].size();j++){
                        floorarea_polygons[ii][j].floor++;
                        floorarea_polygons[ii][j].z = floor_index[floorarea_polygons[ii][j].floor];
                    }
                }
            }
        }

        plans_mutex.unlock();
        floors_mutex.unlock();
        areas_mutex.unlock();
        locations_mutex.unlock();
        walls_mutex.unlock();
        adj_mutex.unlock();
        points_mutex.unlock();


        find_floor_extremes();

        show_warning_map_cb(this);

        draw->redraw();
        
        floor_choice_cb(0);

        prepare_undo();

    }
   
    
}


void base::plan_colors_menu_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->plan_colors_menu_cb();
}



void base::plan_colors_menu_cb()
{
    int draw_colors = draw->get_plan_colors();
    int new_color = abs(draw_colors - 1);
    draw->do_plan_colors(new_color);
    plan_colors_button->value(new_color);
    draw->redraw();
}



void base::plan_colors_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->plan_colors_cb();
}



void base::plan_colors_cb()
{
    int draw_colors = plan_colors_button->value(); //new value
    draw->do_plan_colors(draw_colors);



    if (draw_colors){
        Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[FLOORPLAN_COLORS_INDEX]);  
        temp->set();
    }
    else{
        Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[FLOORPLAN_COLORS_INDEX]);  
        temp->clear();   
    }
           


    draw->redraw();
}


void base::splines_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->splines_cb();
}



void base::splines_cb()
{
    int use_splines = splines_button->value();
    draw->do_splines(use_splines);
    draw->redraw();
}



void base::LayerCB_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->LayerCB();
}



void base::LayerCB()
{
    // Create a modal dialog with layers checkbuttons
    
    if (floor_choice->value()){
        LayerWin* dlg = new LayerWin(this);
        //IMPORTANT:
        //LayerWin class follows these rules:
        //  1) MUST only be initialised on the heap with "new"
        //  2) MUST NOT be freed with "delete" - it manages its own memory due to unknown lifetime
        dlg->Create();
    }
    else{
        push_time();
        push_output("Select a floor!\n");
    }
}


void base::BlockCB_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->BlockCB();
}



void base::BlockCB()
{
    // Create a modal dialog with layers checkbuttons
    
    if (floor_choice->value()){
        BlockWin* dlg = new BlockWin(this);
        //IMPORTANT:
        //BlockWin class follows these rules:
        //  1) MUST only be initialised on the heap with "new"
        //  2) MUST NOT be freed with "delete" - it manages its own memory due to unknown lifetime
        dlg->Create();
    }
    else{
        push_time();
        push_output("Select a floor!\n");
    }
}

void base::lines_to_walls_standing_static(Fl_Widget* /*widget*/, void* data){
    base* program = static_cast<base*> (data);
    program->lines_to_walls_standing();
}
void base::lines_to_walls_standing_transparent_static(Fl_Widget* /*widget*/, void* data){
    base* program = static_cast<base*> (data);
    program->lines_to_walls_standing_transparent();
}

void base::lines_to_walls_sitting_static(Fl_Widget* /*widget*/, void* data){
    base* program = static_cast<base*> (data);
    program->lines_to_walls_sitting();
}
void base::lines_to_walls_sitting_transparent_static(Fl_Widget* /*widget*/, void* data){
    base* program = static_cast<base*> (data);
    program->lines_to_walls_sitting_transparent();
}


void base::lines_to_walls_standing(){
    
    if (root.calculating){
        calc_cb();
        return;
    }
    
    int floor =  floor_choice->value()-1;
    
    if ((floor<0)&&(root.current->floor_index.size()==1)){
        Fl::lock();
        floor_choice->value(1);
        floor_choice->do_callback();
        Fl::unlock();
        push_time();
        push_output("Defaulting to floor 0.\n");
        floor=0;
    }
    else if (floor<0){
        push_time();
        push_output("Select a floor!\n");
        
        return;
    }

    int proceed = choice("Proceeding will add to the current set of opaque standing barriers with objects found in the active layers/blocks of the floorplan on this floor.\nExcessive numbers of barriers will negatively impact performance. Proceed?","Continue","Cancel",0,"Add barriers?");
    
    if (proceed){
        return;
    }

    lines_to_walls(0);

    push_time();
    push_output("Generated opaque standing height barriers from displayed floor plan objects.\n");

}

void base::lines_to_walls_standing_transparent(){
    
    if (root.calculating){
        calc_cb();
        return;
    }
    
    int floor =  floor_choice->value()-1;
    
    
    if ((floor<0)&&(root.current->floor_index.size()==1)){
        Fl::lock();
        floor_choice->value(1);
        floor_choice->do_callback();
        Fl::unlock();
        push_time();
        push_output("Defaulting to floor 0.\n");
        floor=0;
    }
    else if (floor<0){
        push_time();
        push_output("Select a floor!\n");
        
        return;
    }
    
    int proceed = choice("Proceeding will add to the current set of transparent standing barriers with objects found in the active layers/blocks of the floorplan on this floor.\nExcessive numbers of barriers will negatively impact performance. Proceed?","Continue","Cancel",0,"Add barriers?");
    
    if (proceed){
        return;
    }

    lines_to_walls(1);
    
    push_time();
    push_output("Generated transparent standing height barriers from displayed floor plan objects.\n");

}

void base::lines_to_walls_sitting(){
    
    if (root.calculating){
        calc_cb();
        return;
    }

    int floor =  floor_choice->value()-1;
    
    
    if ((floor<0)&&(root.current->floor_index.size()==1)){
        Fl::lock();
        floor_choice->value(1);
        floor_choice->do_callback();
        Fl::unlock();
        push_time();
        push_output("Defaulting to floor 0.\n");
        floor=0;

    }
    else if (floor<0){
        push_time();
        push_output("Select a floor!\n");
        
        return;
    }

    int proceed = choice("Proceeding will add to the current set of opaque sitting barriers with objects found in the active layers/blocks of the floorplan on this floor.\nExcessive numbers of barriers will negatively impact performance. Proceed?","Continue","Cancel",0,"Add barriers?");
    
    if (proceed){
        return;
    }

    lines_to_walls(2);
    
    push_time();
    push_output("Generated opaque sitting height barriers from displayed floor plan objects.\n");
    
}

void base::lines_to_walls_sitting_transparent(){
    
    if (root.calculating){
        calc_cb();
        return;
    }

    int floor =  floor_choice->value()-1;
    
    if ((floor<0)&&(root.current->floor_index.size()==1)){
        Fl::lock();
        floor_choice->value(1);
        floor_choice->do_callback();
        Fl::unlock();
        push_time();
        push_output("Defaulting to floor 0.\n");
        floor=0;
    }
    else if (floor<0){
        push_time();
        push_output("Select a floor!\n");
        
        return;
    }

    int proceed = choice("Proceeding will add to the current set of transparent sitting barriers with objects found in the active layers/blocks of the floorplan on this floor.\nExcessive numbers of barriers will negatively impact performance. Proceed?","Continue","Cancel",0,"Add barriers?");

    if (proceed){
        return;
    }

    lines_to_walls(3);
    
    push_time();
    push_output("Generated transparent sitting height barriers from displayed floor plan objects.\n");
}


void* base::read_walls_pass_static(void* data){

    base* program = static_cast<base*> (data);
    program->read_walls_pass();

    return NULL;

}

void* base::read_walls_pass(){ 
    
    int i=readin_walls(root.current->walls_standing,root.current->walls_sitting,wall_type,wall_floor,desks_file);
    
    Fl::lock();
    show_walls_sitting->value(1);
    show_walls_standing->value(1);
    Fl::unlock();
       
    std::vector<location>& loc0 = root.current->locations[0];
    
    for (unsigned int i=0;i<loc0.size();i++){
        int floor = loc0[i].loc.floor;
        loc0[i].loc.z = root.current->floor_index[floor];
    }
    
    if (!i) Fl::awake(show_warning_vis_cb,this);
    root.calculating=FREE;
    Fl::awake(calc_message_off_cb,this);
    return NULL;
    
}