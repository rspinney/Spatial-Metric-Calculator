/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

 #include "./include/base.hpp"

int base::save_binary(const file_in_out* object){
    
    {
        building obj = (*object).content;
        std::ofstream ofs((*object).file.c_str());
        boost::archive::text_oarchive oa(ofs); //opt for text archive as although easier to meddle with and slightly larger, will work across all architectures
        oa & obj;
    }
    
    return 0;
}

int base::read_binary(file_in_out* object){
    
 

    
    std::ifstream ifs((*object).file.c_str());
    
    
    try
    {
        {
            boost::archive::text_iarchive ia(ifs);
             // read class state from archive
            ia >> (*object).content;//(*obj);
        }
        
    }
    catch (boost::archive::archive_exception& ex) {
        push_time();
        push_output("File not read! Incorrect or outdated file type.\n");
        std::cout<<"huh "<<ex.what()<<std::endl;
        return 1;
    }
    catch (...)
    {
        push_time();
        push_output("File not read! Incorrect or outdated file type.\n");
        std::cout<<"huh "<<std::endl;
        return 1;
    }
    
    

    
    return 0;
    
}



void base::save_binary_cb_static(Fl_Widget*, void* data){
    
    base* program = static_cast<base*> (data);
    

    program->save_binary_cb();
}


void base::save_binary_cb(){

    if (!root.calculating){
        
        
        
        Fl_Native_File_Chooser fnfc;
        
        fnfc.preset_file(root.current->save_file_string.c_str());
        
        fnfc.title("Save project");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
        fnfc.filter("*.proj");
        fnfc.options(Fl_Native_File_Chooser::SAVEAS_CONFIRM);
        if ( fnfc.show() ) return;
        
        std::string str=std::string(fnfc.filename());
        
        root.current->save_file_string = str;
        
        root.calculating=SAVING;
        calc_message_on();

        test_thread = std::thread(&base::save_binary_thread_static,this);
        test_thread.detach();

    }
    else{
        calc_cb();
    }
    
}




void base::read_binary_cb_static(Fl_Widget* /*widget*/, void* data){
    
    base* program = static_cast<base*> (data);
    
    program->read_binary_cb();
}
void base::read_binary_cb(){
    
    if (!root.calculating){
        
        
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open project");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        //fnfc.filter("*.{proj,smc}");
        fnfc.filter("*.proj");
        if ( fnfc.show() ) return;
        
        std::string str=std::string(fnfc.filename());   
                           
        root.current->save_file_string = str;

        floor_choice->value(0);
        floor_choice_cb(0);
        if (three_d_choice->value()==3){
            
            three_d_choice->value(0);
            three_d_cb();
        }
        
        for (int i=0;i<4;i++){
            hist_metric[i]->value(0);
        }
        plot_choice_cb();
        
        data_choice->value(0);
        redraw_data();
        
        draw->redraw();

        root.calculating=LOADING;
        calc_message_on();

        metrics_mutex.lock();

        root.current->plot_metric.clear();
        root.current->max_plot_metric = 0.0;
        root.current->min_plot_metric = 0.0;
        root.current->route.clear();

        metrics_mutex.unlock();

        test_thread = std::thread(&base::read_binary_thread_static,this);
        test_thread.detach();
    }
    else{
        calc_cb();
    }
    
    
}

void* base::save_binary_thread_static(void* data){
    base* program = static_cast<base*> (data);
    program->save_binary_thread();
    return NULL;
}


void* base::save_binary_thread(){


    
    building build_obj;
    building* temp_obj=&build_obj;    
    
    (*temp_obj)=*(root.current);

    file_in_out obj;
    obj.content = (*temp_obj);
    obj.file = root.current->save_file_string;
    
    int bd=0;
    for (unsigned int i=0;i<root.comp_buildings.size();i++){
        if (root.current==root.comp_buildings[i]){
            bd=i;
            break;
        }
    }
    
    
    std::stringstream s;
    push_time();
    s<<"Writing project in slot "<<bd+1<<" to file `"<<root.current->save_file_string<<"`..."<<std::endl;
    push_output(s.str());
    
    save_binary(&obj);
    
    push_time();
    s.str("");
    s<<"...done."<<std::endl;
    push_output(s.str());
    
    root.calculating=FREE;
    calc_message_off();

    return NULL;   
}


void* base::loading_thread_func_static(void* data){
    base* program = static_cast<base*> (data);
    program->loading_thread_func();
    return NULL;
}

void* base::loading_thread_func(){
    
    
    int start_buff = text->buff->length();
    
    int n=0;
    
    while (loaded==0){
        
        output_mutex.lock();            
        text->buff->remove(start_buff,text->buff->length());            
        output_mutex.unlock();
        push_time();
        std::stringstream s;
        s.str("");
        s<<"...patience...";
        
        if (n==0){
            s<<" -";n++;
        }
        else if(n==1){
            s<<" \\";n++;
        }
        else if (n==2){
            s<<" |";n++;
        }
        else if (n==3){
            s<<" /";
            n=0;
        }
        s<<std::endl;            
        push_output(s.str());            
        
        std::this_thread::sleep_for (std::chrono::milliseconds(125));
    }
    
    return NULL;
    
}


void* base::read_binary_thread_static(void* data){

    base* program = static_cast<base*> (data);
    program->read_binary_thread();
    return NULL;

}

void* base::read_binary_thread(){
    

    
    if (root.current->use_segments){
        Fl::lock();
        edges_button->value(0);
        root.current->use_segments=0;         
        Fl::unlock();
    }

    if (root.current->use_turns){
        Fl::lock();
        turns_button->value(0);
        root.current->use_turns=0;  
        turn_ang->deactivate();
        turn_ang_ok->deactivate();           
        Fl::unlock();
    }

       
    file_in_out temp_obj;

    
    temp_obj.content.floorarea_polygons.clear(); 
    temp_obj.content.floor_areas.clear();
    temp_obj.content.floor_index.clear();
    temp_obj.content.floor_plans.clear();
    
    temp_obj.file = root.current->save_file_string;
    
    int bd=-1;
    
    for (unsigned int i=0;i<root.comp_buildings.size();i++){
        if (root.current==root.comp_buildings[i]){
            bd=i;
            break;
        }
    }
    
    std::stringstream s;
    push_time();
    s<<"Reading into project slot "<<bd+1<<" from project file `"<<root.current->save_file_string<<"`..."<<std::endl;
    push_output(s.str());
    
    int start_buff=text->buff->length();
   
    test_thread2 = std::thread(&base::loading_thread_func_static,this);
    
    
    loaded=0;
    
    int error=read_binary(&temp_obj);
    
    if (!error){

        points_mutex.lock();
        adj_mutex.lock();
        walls_mutex.lock();
        locations_mutex.lock();
        areas_mutex.lock();
        floors_mutex.lock();
        plans_mutex.lock();
        
        root.current->points=temp_obj.content.points;
        root.current->adj_list=temp_obj.content.adj_list;
        root.current->map.clear();
        root.current->custom_metric=temp_obj.content.custom_metric;


        root.current->locations = temp_obj.content.locations;

        root.current->walls_sitting=temp_obj.content.walls_sitting;
        root.current->walls_standing=temp_obj.content.walls_standing;

        
        
        root.current->mid_x=temp_obj.content.mid_x;
        root.current->mid_y=temp_obj.content.mid_y;
        root.current->mid_z=temp_obj.content.mid_z;
        root.current->minimum_x=temp_obj.content.minimum_x;
        root.current->minimum_y=temp_obj.content.minimum_y;
        root.current->minimum_z=temp_obj.content.minimum_z;
        root.current->maximum_x=temp_obj.content.maximum_x;
        root.current->maximum_y=temp_obj.content.maximum_y;
        root.current->maximum_z=temp_obj.content.maximum_z;
        
        root.current->vis_valid=temp_obj.content.vis_valid;
        root.current->map_valid=temp_obj.content.map_valid;
        root.current->lines_valid=temp_obj.content.lines_valid;
        
        
        root.current->vis_valid=0;
        root.current->map_valid=0;
        root.current->lines_valid=0;
        root.current->segs_valid=0;
        root.current->turns_valid=0;
        root.current->metrics = temp_obj.content.metrics;
        root.current->metric_des = temp_obj.content.metric_des;
        
        root.current->floorarea_polygons=temp_obj.content.floorarea_polygons;
        root.current->floorarea_triangles=temp_obj.content.floorarea_triangles;
        root.current->floorarea_type=temp_obj.content.floorarea_type;
        root.current->floor_areas=temp_obj.content.floor_areas;


        plans_mutex.unlock();
        floors_mutex.unlock();
        areas_mutex.unlock();
        locations_mutex.unlock();
        walls_mutex.unlock();
        adj_mutex.unlock();
        points_mutex.unlock();


        Fl::awake(show_warning_vis_start,this);
        Fl::awake(show_warning_map_start,this);
        

        plans_mutex.lock();

        root.current->floor_plans=temp_obj.content.floor_plans;

        plans_mutex.unlock();

        floors_mutex.lock();
        plans_mutex.lock();
        
        root.current->floor_index=temp_obj.content.floor_index;
        
        //hack to deal with old corrupted files with dodgy floor area saves

        for (int i=root.current->floor_areas.size()-1;i>=0;i--){
            if (i>=static_cast<int>(root.current->floor_index.size())){
                root.current->floor_areas.erase(root.current->floor_areas.begin()+i);
            }
        }

        for (int i=root.current->floor_plans.size()-1;i>=0;i--){
            if (i>=static_cast<int>(root.current->floor_index.size())){
                root.current->floor_plans.erase(root.current->floor_plans.begin()+i);
            }
        }


        while (root.current->floor_areas.size()<root.current->floor_index.size()){
            root.current->floor_areas.push_back(0);
        }

        while (root.current->floor_plans.size()<root.current->floor_index.size()){
            plan temp;
            root.current->floor_plans.push_back(temp);
        }

        //end hack to deal with old corrupted files with dodgy floor area saves

       plans_mutex.unlock();
       floors_mutex.unlock();


       //process floor plans
       
       for (unsigned int i=0; i<root.current->floor_plans.size();i++){
            plans_mutex.lock();
            root.current->floor_plans[i].convert_to_lines(); //sets fast_plan internally
            plans_mutex.unlock();
            if (!root.current->floor_plans[i].fast_plan){
                
                std::stringstream s;
                push_time();
                s<<"WARNING! Floorplan on floor "<<i<<" is very large/contains large numbers of objects."<<std::endl;
                push_output(s.str());
                s.str("");
                s<<"------------------- >> "<<"Fixed resolution drawing of the floorplan is disabled and replaced with a dynamic method."<<std::endl;
                push_output(s.str());
            }
       }
       

       //end process floor plans

       //set up metrics and structures

       root.current->map.set_cache_arrays(&(root.current->map),0,0);
        Fl::lock();
        reset_floor_choice();
        metric_slot_choice->clear();
        
        std::stringstream s;
        for (unsigned i=0;i<root.current->metrics.size();i++){
            s.str("");
            s<<i+1;
            metric_slot_choice->add(s.str().c_str());
        }
        s.str("");
        s<<root.current->metrics.size()+1<<" (new)";
        metric_slot_choice->add(s.str().c_str());
        
        metric_slot_choice->value(root.current->metrics.size());
     
        Fl::unlock();
        
        
        find_floor_extremes();
           
        root.current->route.clear();
        std::vector<std::vector<int> >().swap(root.current->route);
        
        draw->first_draw=1;
        Fl::awake(&redraw_from_thread,this);
  
        
    }
    
    loaded=1;
    
    test_thread2.join();
    
    Fl::lock();
    text->buff->remove(start_buff,text->buff->length());
    Fl::unlock();
    
    if (!error){
    
        push_time();
        s.str("");
        s<<"...done."<<std::endl;
        push_output(s.str());
        push_time();
        s.str("");
        s<<"Note: edge and angular analysis disabled by default."<<std::endl;
        push_output(s.str());
        prepare_undo();

        if (root.current->adj_list.size()>MAX_DISPLAY_LINKS){
            Fl::lock();
            line_thickness->value(0);
            Fl::unlock();
            if (!root.limit_edge){
                Fl::lock();
                Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[LIMIT_EDGE_INDEX]);   
                temp->do_callback(menu,this);
                temp->set();         
                Fl::unlock();
                
            }
            push_time();
            push_output("Imported project contains a large number of edges...\n");
            push_time();
            push_output("Limiting edge visualisation and setting visualisation of edge widths to 0.\n");
        }

        Fl::awake(&redraw_from_thread,this);
        
    }
    else{
        push_time();
        s.str("");
        s<<"Error reading file."<<std::endl;
        push_output(s.str());
    }
    
    root.calculating=FREE;
    calc_message_off();
    return NULL;
}
