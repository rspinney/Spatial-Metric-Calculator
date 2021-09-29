/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

/// Contains entry functions conerning metrics routes and location visibility

#include "./include/base.hpp"

void base::map_consistency_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->map_consistency();
}

void base::map_consistency() {
    
    if (!root.calculating){
        
        root.calculating=CHECKING_MAP;
        calc_message_on();

        test_thread = std::thread(&base::check_map_static,this);
        test_thread.detach();

    }
    else{

        calc_cb();
        return;
    }
    
}

void base::check_map_static(void* data){

    base* program = static_cast<base*> (data);
    program->check_map();

    return;

}


void base::check_map(){
    
    push_time();
    push_output("Checking graph consistency...\n");

    points_mutex.lock();
    std::vector<vec> points = root.current->points;
    points_mutex.unlock();

    adj_mutex.lock();
    std::vector<vertex> adj_list = root.current->adj_list;
    adj_mutex.unlock();
    
    root.killthread.reset();
    std::vector<int> include(points.size(),0);

    int duplicates=0;
    
    for (unsigned int i=0;i<adj_list.size();i++){
        
        if (root.killthread.get()){

            push_time();
            push_output("Cancelled.\n");
            root.killthread.reset();
            root.calculating=FREE;
            calc_message_off();

            return;
        }

        int p1= adj_list[i].start;
        int p2= adj_list[i].end;
        
        include[p1]=1;
        include[p2]=1;
        
        for (unsigned int j=0;j<i;j++){
            int p3 = adj_list[j].start;
            int p4 = adj_list[j].end;
            
            if (((p3==p1)&&(p4==p2))||((p3==p2)&&(p4==p1))){
                duplicates=1;
                push_time();
                std::stringstream s;
                s<<"Adjacency list elements "<<i<<" and "<<j<<" are duplicates!"<<std::endl;
                push_output(s.str());
            }
        }
        
        
    }
    
    int bad=0;
    for (unsigned int i=0;i<include.size();i++){
        if (include[i]==0){
            bad=1;

            push_time();
            std::stringstream s;
            s<<"Vertex "<<i<<" not connected to any others on spatial graph"<<std::endl;
            push_output(s.str().c_str());

        }
    }
    if (duplicates){
        bad=1;

        push_time();
        push_output("...done\n");
        push_time();
        push_output("WARNING: Duplicates exist in the adjacency matrix. DO NOT CALCULATE METRICS. This can only occur if they have been read in from an adjacency list with duplicates included. Check files!\n");

    }
    if (!bad){

        push_time();
        push_output("...done\n");
        push_time();
        push_output("Every vertex connected to at least one other and no adjacency list duplicates exist.\n");

    }
    root.calculating=FREE;
    calc_message_off();
    return;
}

void base::metric_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->metric_cb();
}
void base::metric_cb(){
    container* root = &(this->root);
    


    int temp_basis = custom_metric_basis->value();
    if (root->current->use_segments){
        custom_metric_basis->clear();
        custom_metric_basis->add("Origin location based");
        custom_metric_basis->add("Vertex based");
        custom_metric_basis->add("Edge based");
        custom_metric_basis->value(temp_basis);
    }
    else{
        custom_metric_basis->clear();
        custom_metric_basis->add("Origin location based");
        custom_metric_basis->add("Vertex based");        
        if (temp_basis<2){
            custom_metric_basis->value(temp_basis);
        }
        else{
            custom_metric_basis->value(0);   
        }
    }
        

    if (custom_metric_choice->value()!=12)
    {
        custom_metric_destination_choice->activate();
        ask6->activate();
        destination_criteria_choice->activate();
        ask3->activate();
        custom_metric_range_choice->activate();
        ask7->activate();
        custom_metric_range->activate();
        custom_metric_range2->activate();
        ask8->activate();
        custom_metric_floor_choice->activate();
        ask9->activate();
        custom_metric_visibility_choice->activate();
        ask10->activate();
        custom_metric_self_count->activate();
        ask11->activate();
        eig_tol->deactivate();
    }
    
    
    if ((custom_metric_basis->value()==2)&&(custom_metric_destination_choice->value()==1)){
        destination_criteria_choice->value(6);
        route_type_choice->value(4);
        
        Fl_Menu_Item  *temp;
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[0]);
        temp->deactivate(); 
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[1]);
        temp->deactivate(); 
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[2]);
        temp->deactivate(); 
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[3]);
        temp->deactivate(); 
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[4]);
        temp->deactivate();   
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[5]);
        temp->deactivate(); 
        temp =  (Fl_Menu_Item*)(&route_type_choice->menu()[0]);
        temp->deactivate();
        temp =  (Fl_Menu_Item*)(&route_type_choice->menu()[1]);
        temp->deactivate();
        temp =  (Fl_Menu_Item*)(&route_type_choice->menu()[2]);
        temp->deactivate();
        temp =  (Fl_Menu_Item*)(&route_type_choice->menu()[3]);
        temp->deactivate();
        



        // if ((custom_metric_choice->value()!=12)&&(custom_metric_choice->value()!=6)&&(custom_metric_choice->value()!=7)&&(custom_metric_choice->value()!=8)){
        //     custom_metric_choice->value(6);
        // }
    }
    else{

        Fl_Menu_Item  *temp;
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[0]);
        temp->activate(); 
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[1]);
        temp->activate(); 
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[2]);
        temp->activate(); 
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[3]);
        temp->activate(); 
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[4]);
        temp->activate();   
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[5]);
        temp->activate(); 
        temp =  (Fl_Menu_Item*)(&route_type_choice->menu()[0]);
        temp->activate();
        temp =  (Fl_Menu_Item*)(&route_type_choice->menu()[1]);
        temp->activate();
        temp =  (Fl_Menu_Item*)(&route_type_choice->menu()[2]);
        temp->activate();
        temp =  (Fl_Menu_Item*)(&route_type_choice->menu()[3]);
        temp->activate();
    }
    
    
    if ((custom_metric_choice->value()>6)&&(custom_metric_choice->value()!=8)){
        custom_metric_stat_choice->deactivate();
        ask5->deactivate();
    }
    else{
        custom_metric_stat_choice->activate();
        ask5->activate();
    }
    
    int temp =custom_metric_destination_choice->value();
    
    
    
    
    
    int temp2 = custom_metric_visibility_choice->value();
    //if (root->current->vis_valid){
        custom_metric_visibility_choice->clear();
        custom_metric_visibility_choice->add("No constraint");
        custom_metric_visibility_choice->add("Visible (all high barriers)");
        custom_metric_visibility_choice->add("Visible (all low + high barriers)");
        custom_metric_visibility_choice->add("Visible (opaque high barriers)");
        custom_metric_visibility_choice->add("Visible (opaque low + high barriers)");
        custom_metric_visibility_choice->value(temp2);
    // }
    // else{
    //     custom_metric_visibility_choice->clear();
    //     custom_metric_visibility_choice->add("No constraint");
    //     custom_metric_visibility_choice->value(0);
    // }
    
    if (custom_metric_basis->value()==2){
        int size=1;
        custom_metric_destination_choice->clear();
        custom_metric_destination_choice->add("Edges");
        if (((root->current->use_segments)&&(root->current->use_turns))
            &&((custom_metric_choice->value()==6)||(custom_metric_choice->value()==7)||(custom_metric_choice->value()==8)))
        {//(root->current->lines_valid){
            custom_metric_destination_choice->add("Turnless lines");            
            size=2;
        }
        if ((temp<size)&&(temp>=0)){
            custom_metric_destination_choice->value(temp);
        }
        else{
            custom_metric_destination_choice->value(0);
        }
        
        
        custom_metric_visibility_choice->deactivate();
        ask10->deactivate();
    }
    else{
        
        custom_metric_destination_choice->clear();

        
        custom_metric_destination_choice->add("Origin locations");
        custom_metric_destination_choice->add("Destination 1 locations");
        custom_metric_destination_choice->add("Destination 2 locations");
        custom_metric_destination_choice->add("Destination 3 locations");
        custom_metric_destination_choice->add("Destination 4 locations");
        custom_metric_destination_choice->add("Destination 5 locations");
        custom_metric_destination_choice->add("Destination 6 locations");
        custom_metric_destination_choice->add("Destination 7 locations");
        custom_metric_destination_choice->add("Destination 8 locations");
        custom_metric_destination_choice->add("Vertices");
        
        int size =10;
        if ((temp<size)&&(temp>=0)){
            custom_metric_destination_choice->value(temp);
        }
        else{
            custom_metric_destination_choice->value(0);
        }
                
        custom_metric_visibility_choice->activate();
        ask10->activate();
        if (custom_metric_basis->value()==1){
            custom_metric_visibility_choice->deactivate();
            ask10->deactivate();
        }
        if (custom_metric_choice->value()==12){
            custom_metric_visibility_choice->deactivate();
            ask10->deactivate();
        }
        if (custom_metric_destination_choice->value()==9){
            custom_metric_visibility_choice->deactivate();
            ask10->deactivate();
        }
    }
    
    if ((custom_metric_choice->value()==0)||(custom_metric_choice->value()==1)||(custom_metric_choice->value()==7)||(custom_metric_choice->value()==12)){
        route_type_choice->deactivate();
        max_iterations->deactivate();
        ask12->deactivate();
        route_stat_choice->deactivate();
        ask13->deactivate();
        ask2->deactivate();
    }
    else{
        route_type_choice->activate();
        max_iterations->activate();
        ask12->activate();
        route_stat_choice->activate();
        ask13->activate();
        ask2->activate();
        
        if (custom_metric_choice->value()-2 == route_type_choice->value()){
            max_iterations->deactivate();
            ask12->deactivate();
            route_stat_choice->deactivate();
            ask13->deactivate();
        }
        
        if (custom_metric_choice->value()>6){
            route_stat_choice->deactivate();
            ask13->deactivate();
        }
    }
    
    if (custom_metric_range_choice->value()>7){
        destination_criteria_choice->activate();
        ask3->activate();
        custom_metric_range->activate();
        custom_metric_range2->activate();

        ask8->activate();
    }
    else if (custom_metric_range_choice->value()>2){
        destination_criteria_choice->activate();
        ask3->activate();
        custom_metric_range->activate();
        custom_metric_range2->deactivate();

        ask8->activate();
    }
    else if (custom_metric_range_choice->value()>0){
        destination_criteria_choice->activate();
        ask3->activate();
        custom_metric_range->deactivate();
        custom_metric_range2->deactivate();
        ask8->deactivate();
    }
    else{
        destination_criteria_choice->deactivate();
        ask3->deactivate();
        custom_metric_range->deactivate();
        custom_metric_range2->deactivate();
        ask8->deactivate();
    }
    
    
    if (custom_metric_choice->value()==12){
        custom_metric_destination_choice->deactivate();
        ask6->deactivate();
        destination_criteria_choice->deactivate();
        ask3->deactivate();
        custom_metric_range_choice->deactivate();
        ask7->deactivate();
        custom_metric_range->deactivate();
        custom_metric_range2->deactivate();
        ask8->deactivate();
        custom_metric_floor_choice->deactivate();
        ask9->deactivate();
        custom_metric_visibility_choice->deactivate();
        ask10->deactivate();
        custom_metric_self_count->deactivate();
        ask11->deactivate();
        eig_tol->activate();
    }
    

    if (!root->current->use_turns){
        Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&custom_metric_choice->menu()[4]);
        temp->deactivate();
        temp =  (Fl_Menu_Item*)(&custom_metric_choice->menu()[6]);
        temp->deactivate();
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[4]);
        temp->deactivate();
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[6]);
        temp->deactivate();
        temp =  (Fl_Menu_Item*)(&route_type_choice->menu()[2]);
        temp->deactivate();
        temp =  (Fl_Menu_Item*)(&route_type_choice->menu()[4]);
        temp->deactivate();

        if ((custom_metric_choice->value()==4)||(custom_metric_choice->value()==6)){
            custom_metric_choice->value(0);
        }

        if ((destination_criteria_choice->value()==4)||(destination_criteria_choice->value()==6)){
            destination_criteria_choice->value(0);
        }

        if ((route_type_choice->value()==2)||(route_type_choice->value()==4)){
            route_type_choice->value(0);
        }
        
    }
    else{
        Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&custom_metric_choice->menu()[4]);
        temp->activate();
        temp =  (Fl_Menu_Item*)(&custom_metric_choice->menu()[6]);
        temp->activate();   
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[4]);
        temp->activate();
        temp =  (Fl_Menu_Item*)(&destination_criteria_choice->menu()[6]);
        temp->activate();
        temp =  (Fl_Menu_Item*)(&route_type_choice->menu()[2]);
        temp->activate();
        temp =  (Fl_Menu_Item*)(&route_type_choice->menu()[4]);
        temp->activate();
    }

    
}

void base::delete_metric_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->delete_metric_cb();
}

void base::delete_metric_cb(){
    container* root = &(this->root);

    if (!root->calculating){

        int i = metric_slot_choice->value();
        int n = root->current->metrics.size();

        if (i<n){

            metrics_mutex.lock();

            root->current->plot_metric.clear();
            root->current->max_plot_metric = 0.0;
            root->current->min_plot_metric = 0.0;
            root->current->metrics.erase(root->current->metrics.begin()+i);
            root->current->metric_des.erase(root->current->metric_des.begin()+i);     

            metrics_mutex.unlock();

            prepare_undo();     
            draw->redraw();
            desk_metric_cb();//refreshes window

            push_time();
            std::stringstream s;
            s<<"Metric "<<i+1<<" deleted.\n";
            push_output(s.str().c_str());


 
        }
        else{
            push_time();
            push_output("No metric to delete!\n");
        }
       
    }
    else{
        calc_cb();
    }
}


void base::clear_metrics_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->clear_metrics_cb();
}

void base::clear_metrics_cb(){
    
    container* root = &(this->root);
    
    if (!root->calculating){

        int i = choice("Clear all graph structures or metrics?","Structures","Metrics","Cancel","Clear?");

        if (i==0){
        
            root->current->matrix.clear();
            std::vector<std::vector<int> >().swap(root->current->matrix);
            
            metrics_mutex.lock();

            root->current->map.clear();                        
            root->current->route.clear();

            metrics_mutex.unlock();
            
            warning_map_cb();
            draw->redraw();

            std::stringstream s;
            s<<"Cleared all graph structures for project slot "<<building_choice->value()+1<<"."<<std::endl;

            push_time();
            push_output(s.str());
        }
        else if(i==1){
            
            metrics_mutex.lock();

            root->current->metrics.clear();
            root->current->metric_des.clear();
            
            std::vector<std::vector<double> >().swap(root->current->metrics);
            std::vector<metric_description>().swap(root->current->metric_des);
            
            root->current->route.clear();
            root->current->plot_metric.clear();
            root->current->max_plot_metric = 0.0;
            root->current->min_plot_metric = 0.0;

            metrics_mutex.unlock();
            
            if (root->draw_metric){
                Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[SHOW_METRIC_INDEX]);   
                root->draw_metric=0;
                temp->clear(); 
            }
            draw->redraw();

            std::stringstream s;
            s<<"Deleted all metrics for project slot "<<building_choice->value()+1<<"."<<std::endl;
            prepare_undo();

            push_time();
            push_output(s.str());
        }
    }
    else{
        calc_cb();
    }
    
    return;
}

void base::desk_metric_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->desk_metric_cb();
}

void base::desk_metric_cb(){
    
    container* root = &(this->root);    

    if (custom_metric_calc->shown()){
        exit_desk_metric();
        return;    
    }

    if (root->calculating==FREE){
    
        metric_slot_choice->clear();
        
        std::stringstream s;
        for (unsigned i=0;i<root->current->metrics.size();i++){
            s.str("");
            s<<i+1;
            metric_slot_choice->add(s.str().c_str());
        }
        s.str("");
        s<<root->current->metrics.size()+1<<" (new)";
        metric_slot_choice->add(s.str().c_str());
        
        metric_slot_choice->value(root->current->metrics.size());
        
        
        custom_metric_calc->show();
        metric_cb();

        custom_metric_calc->resize(win->x()+0.5*win->w()-225,win->y()+0.5*win->h()-363,450,726); 
        
    }
    else{
        calc_cb();
    }
    
    
}


void base::close_desk_metric_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->close_desk_metric();
}

void base::close_desk_metric(){
    
    container* root = &(this->root);
    
    if (!root->draw_metric){

        Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[SHOW_METRIC_INDEX]);                
        temp->do_callback(menu,this);
        temp->set();
        
    }

    int slot = metric_slot_choice->value();

   
    if (slot+1<=static_cast<int>(root->current->metrics.size())){

        metrics_mutex.lock();

        root->current->plot_metric = root->current->metrics[slot];
        root->current->plot_metric_des = root->current->metric_des[slot];

        metrics_mutex.unlock();

        double min=0,max=0;
        
        if(root->current->plot_metric.size()>0){
            
            min=9e99;
            for (unsigned int i=0;i<root->current->plot_metric.size();i++){
                if ((root->current->plot_metric[i]<min)&&(root->current->plot_metric[i]>=0)){
                    min=root->current->plot_metric[i];
                }
            }
            
            max=*std::max_element(root->current->plot_metric.begin(),root->current->plot_metric.end());
        }

        metrics_mutex.lock();

        root->current->max_plot_metric = max;
        root->current->min_plot_metric = min;

        metrics_mutex.unlock();

        push_time();
        std::stringstream s;
        s<<"Displaying metric "<< slot +1<<" \n";
        push_output(s.str().c_str());
    }



    custom_metric_calc->hide();
    //Fl::focus(this->win);
   
    if(!root->current->plot_metric_des.basis){

        if (root->desk_model){
    
            Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[DESKMODEL_INDEX]);   
            temp->do_callback(menu,this);
            temp->clear(); 
    
        }

        show_locations[0]->value(1);
        show_locations[0]->do_callback(show_locations[0],this);
    }
    else{
        show_map->value(1);
        show_map->do_callback(show_locations[0],this);
    }
    

    draw->redraw();
    
    
}

void base::exit_desk_metric_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->exit_desk_metric();
}

void base::exit_desk_metric(){
    
    container* root = &(this->root);

    custom_metric_calc->hide();
    //Fl::focus(this->win);
    
    metrics_mutex.lock();

    root->current->plot_metric.clear();
    root->current->max_plot_metric = 0.0;
    root->current->min_plot_metric = 0.0;

    metrics_mutex.unlock();

    draw->redraw();
    
}

void base::desk_metric_cb1_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->desk_metric_cb1();
}

void base::desk_metric_cb1(){
    
    THREAD_STATE& calculating = root.calculating;
    
    
    if (calculating == FREE){
        calculating=CALC_METRIC;
        calc_message_on();

        mark_locations_choice->value(0);

        metrics_mutex.lock();

        root.current->plot_metric.clear();
        root.current->max_plot_metric = 0.0;
        root.current->min_plot_metric = 0.0;

        metrics_mutex.unlock();

        test_thread = std::thread(&base::custom_metric_thread_static,this);
        test_thread.detach();


        custom_metric_calc->hide();
        //Fl::focus(this->win);
        
        if (!custom_metric_basis->value()){

            if (root.desk_model){

                Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[DESKMODEL_INDEX]);   
                temp->do_callback(menu,this);
                temp->clear(); 
 
            }

            show_locations[0]->value(1);
            show_locations[0]->do_callback(show_locations[0],this);
        }
        else{
            show_map->value(1);
            show_map->do_callback(show_locations[0],this);
        }
        


        if (!root.draw_metric){

            Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[SHOW_METRIC_INDEX]);                
            temp->do_callback(menu,this);
            temp->set();

        }
        
    }
    else{
        calc_cb();
    }
    
    
}


void base::line_calc_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->line_calc_cb();
}

void base::line_calc_cb(){
    
    THREAD_STATE& calculating = root.calculating;
    
    if ((calculating == FREE)&&(root.current->map_valid)){
        calculating=CALC_LINES;
        calc_message_on();
        test_thread = std::thread(&map_structures::calculate_lines_static,this);
        test_thread.detach();

    
    }
    else if(calculating == FREE){
        push_time();
        push_output("Spatial graph out of date! Recalculate structures to calculate lines.\n");
    }
    else{
        calc_cb();
    }

}

void* base::custom_metric_thread_static(void* data){

    base* program = static_cast<base*> (data);
    program->custom_metric_thread();
    return NULL;
}


void base::launch_custom_metric(   int thread,
                                   int start,
                                   int end,
                                   std::vector<double> &temp_metric,
                                   const std::vector<vec>& points,
                                   const std::vector<location>& origins, 
                                   map_structures* map,                                    
                                   int dest_choice_metric,
                                   int route_choice_metric,
                                   int metric, 
                                   int same_floor, 
                                   int visible,
                                   int stat,
                                   int range_option,
                                   std::vector<location>* destination,                            
                                   int is_desk,
                                   double range,
                                   double range2,
                                   int desk_or_point, 
                                   int self_count_aware,
                                   int dest_is_points,
                                   int dest_is_lines,
                                   int iterations,                           
                                   graph_routes&, //gr_dest,
                                   graph_routes& ,//gr_route,
                                   std::vector<double>&, //weight_d,
                                   std::vector<int>&, //visited_d, 
                                   int inverse,
                                   std::vector<std::vector<route_weight_number> >& choice_new,
                                   int route_stat, 
                                   int do_cache,
                                   int &kill_flag,
                                   ERROR_CODE &error_pass)
{



    boost::posix_time::ptime time =boost::posix_time::microsec_clock::local_time();
    boost::posix_time::ptime time_inter =boost::posix_time::microsec_clock::local_time();

    int size = end-start+1;

    temp_metric.reserve(size);
    
    int count = 0;
    std::stringstream output;

    int d_count = 1;

    for (int i=start; i<end; i++){        


        if (root.killthread.get(thread)){
            kill_flag=1;
            stop_console_output();
            return;
        }


        count++;
        ERROR_CODE error;
        
                
        double temp = root.current->map.desk_custom(points,
                                                      origins,
                                                      map,
                                                      i,
                                                      dest_choice_metric,
                                                      route_choice_metric,
                                                      metric, 
                                                      same_floor, 
                                                      visible,
                                                      stat,    
                                                      range_option,
                                                      destination,
                                                      error,
                                                      is_desk,
                                                      range,
                                                      range2,
                                                      desk_or_point,
                                                      self_count_aware,
                                                      dest_is_points,
                                                      dest_is_lines,
                                                      iterations,                                                      
                                                      // gr_dest,
                                                      // gr_route,
                                                      // weight_d,
                                                      // visited_d,
                                                      inverse,
                                                      choice_new,
                                                      route_stat,
                                                      do_cache,
                                                      map_mutex,
                                                      root.killthread,
                                                      thread);
        

        temp_metric.push_back(temp);


        error_pass.collect(error);



        
        if (count%d_count==0){
                
            boost::posix_time::ptime time_new = boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration dur = time_new - time;
            boost::posix_time::time_duration dur_inter = time_new - time_inter;
            time_inter = time;
            double dt = dur.total_milliseconds();
            double dt_inter = dur_inter.total_milliseconds();

            d_count = std::max(1,static_cast<int>(100/std::max(1.0,dt_inter))*d_count);   
        
            if (dt>100){

                time = time_new;

                calc_progress.set(thread, 100.0*(static_cast<double>(count+1)/static_cast<double>(size)));
                
            }

        }
   
    }

}

void* base::custom_metric_thread(){
    
    std::vector<location>* loc0 = &(root.current->locations[0]);
    std::vector<location>* loc1 = &(root.current->locations[1]);
    std::vector<location>* loc2 = &(root.current->locations[2]);
    std::vector<location>* loc3 = &(root.current->locations[3]);
    std::vector<location>* loc4 = &(root.current->locations[4]);
    std::vector<location>* loc5 = &(root.current->locations[5]);
    std::vector<location>* loc6 = &(root.current->locations[6]);
    std::vector<location>* loc7 = &(root.current->locations[7]);
    std::vector<location>* loc8 = &(root.current->locations[8]);


    std::vector<vec>& points = root.current->points;
    
    map_structures& map = root.current->map;
    
    std::vector<double> custom_metric;

    int choice1,choice2,choice3,choice4,choice5,choice6=0,choice7,choice8,is_desk=0;//error=0;//error_found1=0
    int self_count_aware=0,dest_is_points=0,dest_is_lines=0,iter=20,inverse,route_stat,do_cache;
    double range,range2;
    std::vector<int> error_found(9,0);
    
    choice1=destination_criteria_choice->value();
    choice2=route_type_choice->value();
    choice3=custom_metric_choice->value();
    choice4=custom_metric_floor_choice->value();
    choice5=custom_metric_visibility_choice->value();
    choice6=custom_metric_stat_choice->value();
    range = custom_metric_range->value();
    range2 = custom_metric_range2->value();
    choice7 = custom_metric_range_choice->value();
    choice8 = custom_metric_basis->value();

    route_stat = route_stat_choice->value();
    
    do_cache = cache_metric_data->value();
    
    {
        std::stringstream s;
        s<<max_iterations->value();

        try{
            iter = stoi(s.str());
        }
        catch (...){
            push_time();
            push_output("Error reading in maximum iterations - setting to default value of 20.\n");      
            iter = 20;
        }

    }
    
    double tol;
    
    {
        std::stringstream s;
        s<<eig_tol->value();

        
        try{

            char * e;
            tol = std::strtod(s.str().c_str(), &e);

            if (*e != '\0' ||  // error, we didn't consume the entire string
                errno != 0 )   // error, overflow or underflow
            {
                push_time();
                push_output("Error reading in eigenvalue centrality tolerance - setting to default value of 0.001.\n");      
                tol = 0.0001;
            }
        }
        catch(...){
            push_time();
            push_output("Error reading in eigenvalue centrality tolerance - setting to default value of 0.001.\n");      
            tol = 0.0001;
        }

        

    }
    
    
    if (((custom_metric_range->active()))&&(range<0)){
        push_time();
        push_output("No negative metric constraints allowed! Metric computation cancelled.\n");
        root.calculating=FREE;
        calc_message_off();

        return NULL;

    }

    if (((custom_metric_range2->active()))&&(range2<0)){
        push_time();
        push_output("No negative metric constraints allowed! Metric computation cancelled.\n");
        root.calculating=FREE;
        calc_message_off();

        return NULL;

    }

    if (((custom_metric_range2->active()))&&(range2<range)){
        push_time();
        push_output("Range 2 value must be greater than range 1 value! Metric computation cancelled.\n");
        root.calculating=FREE;
        calc_message_off();

        return NULL;

    }
    
    if (((max_iterations->active()))&&(iter<0)){
        push_time();
        push_output("No negative iteration values allowed! Metric computation cancelled.\n");
        root.calculating=FREE;
        calc_message_off();

        return NULL;

    }
    
    if (((eig_tol->active()))&&(tol<0)){
        push_time();
        push_output("No negative tolerances allowed! Metric computation cancelled.\n");
        root.calculating=FREE;
        calc_message_off();

        return NULL;

    }
    

    root.killthread.reset();
    //////////////////// PRE-REQUISITE CALCULATIONS /////////////////////////////////

    if ((choice8==2)&&(custom_metric_destination_choice->value()==1)){
        dest_is_lines = 1; // need to know this before pre-requisite calculations
    }

    

    if  ((!root.current->map_valid)||
         ((!root.current->segs_valid)&&(root.current->use_segments))||
         ((!root.current->turns_valid)&&(root.current->use_turns))||
         (force_create_button->value()))
    {
        root.calculating=CALC_METRIC;
        calc_message_on();
        root.current->map.clear();
        root.current->map.calculate_distances_paths(this);
        root.calculating=CALC_METRIC;
        
        if (root.calculating==FREE){
            root.killthread.reset();
            root.calculating=FREE;
            calc_message_off();
            return NULL;
        }

        if  (!root.current->map_valid){
            push_time();
            push_output("Construction of graph structures failed - metric calculation cancelled.\n");
            root.killthread.reset();
            root.calculating=FREE;
            calc_message_off();
            return NULL;
        }
    }

    if (root.killthread.get()){
        root.killthread.reset();
        root.calculating=FREE;
        calc_message_off();
        return NULL;
    }

    int visible = choice5;

    if  (((!root.current->vis_valid)&&(visible))||
         (force_visibility_button->value()))
    {
        root.calculating=CALC_METRIC;
        calc_message_on();
        visibility_calc1();
        root.calculating=CALC_METRIC;
        if  (!root.current->vis_valid){
            push_time();
            push_output("Construction of visibility data failed - metric calculation cancelled.\n");
            root.killthread.reset();
            root.calculating=FREE;
            calc_message_off();
            return NULL;
        }
    }

    
    if (root.killthread.get()){
        root.killthread.reset();
        root.calculating=FREE;
        calc_message_off();
        return NULL;
    }

    if  ((dest_is_lines)&&((!root.current->lines_valid)||(!root.current->map_valid)))
    {
        root.calculating=CALC_METRIC;
        calc_message_on();
        root.current->map.calculate_lines_static(this);
        root.calculating=CALC_METRIC;
        
        if  (!root.current->lines_valid){
            push_time();
            push_output("Construction of turnless lines failed - metric calculation cancelled.\n");
            root.killthread.reset();
            root.calculating=FREE;
            calc_message_off();
            return NULL;
        }
    }

    if (root.killthread.get()){
        root.killthread.reset();
        root.calculating=FREE;
        calc_message_off();
        return NULL;
    }
    

    calc_message_on();

    ////////////////// END PRE-REQUISITE CALCULATIONS ///////////////////////////////



    ////////////////// GENERATE GRAPH BASED DESTINATIONS //////////////////////


    std::vector<location>* destination;
    
    std::vector<location> point_dest,seg_dest,line_dest;
    
    for (unsigned int i=0;i<points.size();i++){
        location temp;
        temp.loc.x=points[i].x;
        temp.loc.y=points[i].y;
        temp.loc.z=points[i].z;
        temp.loc.floor=points[i].floor;
        temp.point=i;
        point_dest.push_back(temp);
    }
    
    for (unsigned int i=0;i<root.current->map.links.size();i++){ //temp bit: only bit of segment needed is its index ie temp.point (despite not being a point)
        location temp;
        temp.point=i;
        temp.loc.floor=0;
        seg_dest.push_back(temp);
        
    }
    
    for (unsigned int i=0;i<root.current->map.line_segments.size();i++){
        location temp;
        temp.point=root.current->map.line_segments[i];
        temp.loc.floor=0;
        line_dest.push_back(temp);
        
    }
    
    ////////////////// END GENERATE GRAPH BASED DESTINATIONS //////////////////////

    if (choice8<2){
        switch (custom_metric_destination_choice->value()){
            case 0:destination=loc0;is_desk=1;
                break;
            case 1:destination=loc1;break;
            case 2:destination=loc2;break;
            case 3:destination=loc3;break;
            case 4:destination=loc4;break;
            case 5:destination=loc5;break;
            case 6:destination=loc6;break;
            case 7:destination=loc7;break;
            case 8:destination=loc8;break;
            case 9:destination=&point_dest;
                dest_is_points=1;
                break;

            default:
                push_time();
                push_output("Input error. Check selectors and try again.\n");
                root.calculating=FREE;
                calc_message_off();

                return NULL;

        }
    }
    else if (choice8==2){ 
        
        if (custom_metric_destination_choice->value()==0){
            destination=&seg_dest;
            self_count_aware=1;
        }
        else if (custom_metric_destination_choice->value()==1){
            destination=&line_dest;dest_is_lines=1;
        }
        else{
            root.calculating=FREE;
            calc_message_off();

            return NULL;

        }
    }
    else{
        root.calculating=FREE;
        calc_message_off();
        return NULL;
    }
    
    inverse = inverse_option->value();
    
    self_count_aware = custom_metric_self_count->value();
    if (self_count_aware==1){
        self_count_aware=0;
    }
    else if (self_count_aware==0){
        self_count_aware=1;
    }
    

    int size=loc0->size();
    
    if (choice8==1){
        size=points.size();
    }
    else if (choice8==2){
        size=map.links.size();
    }
    

    if(!size){
        push_time();
        if (choice8==0){
            push_output("Failed. No origin constructs.\n");
        }
        else{
            push_output("Failed. No graph constructs.\n");
        }
        root.calculating=FREE;
        calc_message_off();
        return NULL;
    }

    
    std::stringstream output;
        
    
    std::vector<std::vector<route_weight_number> > choice;
    
    graph_routes gr_route,gr_dest;
    std::vector<double> weight;
    std::vector<int> visited;
    
    ERROR_CODE errors;



    if ((choice3==9)||(choice3==10)||(choice3==11)){
        

        std::vector<std::vector<route_weight_number> >* temp = map.desk_choice_new(&choice,&map,choice2,choice8,iter,&root,errors,do_cache);
        
        if (do_cache){
            choice = *temp;
        }
    }

    // if (error_choice==MAX_PATHS_REACHED){
    //     push_time();
    //     push_output("WARNING: Not all paths found. The max. number of routes may be too small.\n");
    // }
    // else if (error_choice==MAX_PATHS_REACHED_NONE_FOUND){
    //     push_time();
    //     push_output("WARNING: Max. routes limit reached, but all contain loops - this indicates zero value loops exist - check graph.\n");            

    // }
    // else if (error_choice == LOOPS_DETECTED){
    //     push_time();
    //     push_output("WARNING: Loops detected in shortest paths - check graph/metric measure.\n");
    // }

    if (errors.is_error()){
        std::vector<std::string> error_msgs = errors.get_error_messages();
        for (unsigned int i=0;i<error_msgs.size();i++){
            push_time();
            push_output(error_msgs[i]);
        }
    }
    
    if (errors.NO_GRAPH){
        root.killthread.reset();
        root.calculating=FREE;
        push_time();
        push_output("No graph constructs! Cancelled.\n");
        calc_message_off();
        return NULL;
    }

    if (root.killthread.get()){
        root.killthread.reset();
        root.calculating=FREE;
        push_time();
        push_output("Cancelled.\n");
        calc_message_off();
        return NULL;
    }
 

    

    

    if (choice3==12){
        
        int ss;
        if (choice8==2){
            ss=map.links.size();
        }
        else{
            ss=map.num_points;
        }
        
        std::vector<double> eigen(ss);                      
        
        if (choice8==2){
            map.power_iteration(eigen,map.S,&root,tol);
        }
        else{
          
            
            map.power_iteration(eigen,map.G,&root,tol);
        }
        
        if (root.killthread.get()){
            push_time();
            push_output("Cancelled.\n");
            root.calculating=FREE;
            calc_message_off();
            root.killthread.reset();
            
            return NULL;
        }
        
     
        push_time();
        push_output("Starting metric calculation...\n");

        for (int i=0;i<size;i++){
            if (!choice8){
                double val=eigen[(*loc0)[i].point];
                if (inverse){
                    custom_metric.push_back(1.0/val);
                }
                else{
                    custom_metric.push_back(eigen[(*loc0)[i].point]);
                }
            }
            else{
                double val=eigen[i];
                if (inverse){
                    custom_metric.push_back(1.0/val);
                }
                else{
                    custom_metric.push_back(eigen[i]);
                }
            }
        }
        
    }

    if (choice3!=12){
    

        push_time();
        push_output("Starting metric calculation...\n");


        //begin threading

        // output_mutex.lock();
        // buffer_point=text->buff->length();
        // output_mutex.unlock();


        unsigned int nt = std::max(static_cast<unsigned int>(MIN_THREAD),std::thread::hardware_concurrency());

        if (size<static_cast<int>(nt) ){
            nt=size;
        }


        std::vector<std::vector<double> > temp_metric(nt,std::vector<double>());

        std::vector<int> index_points(nt,0);                
        index_points[0]=0;
        for (unsigned int i=1;i<nt;i++){
            
            index_points[i]=index_points[i-1] + std::max(1,size/static_cast<int>(nt));
        }
        
        std::vector<int> flags(nt,0);
        std::vector<std::thread> threads(nt);
        std::vector<int> kill_flags(nt,0);
        std::vector<ERROR_CODE> thread_errors(nt);


        output_mutex.lock();
        calc_progress.reset(nt);
        root.killthread.reset(nt);
        output_mutex.unlock();

        if (calc_progress.size()<static_cast<int>(nt)) nt = calc_progress.size();
        if (root.killthread.size()<static_cast<int>(nt)) nt = root.killthread.size(); 

        Fl::awake(add_console_updater,this);
        wait_for_console();

        for (unsigned int i=0;i<nt;i++){
            int start = index_points[i];
            int end;
            if (i<nt-1){
                end = index_points[i+1];
            }
            else{
                end = size;
            }
            threads[i] = std::thread(&base::launch_custom_metric,
                                     this,
                                     i,
                                     start,
                                     end,
                                     std::ref(temp_metric[i]),
                                     std::ref(points),
                                     std::ref(*loc0),
                                     &map,                                     
                                     choice1,
                                     choice2,
                                     choice3,
                                     choice4,
                                     choice5,
                                     choice6,
                                     choice7,
                                     destination,                                     
                                     is_desk,
                                     range,
                                     range2,
                                     choice8,
                                     self_count_aware,
                                     dest_is_points,
                                     dest_is_lines,
                                     iter,                                     
                                     std::ref(gr_dest),
                                     std::ref(gr_route),
                                     std::ref(weight),
                                     std::ref(visited),
                                     inverse,
                                     std::ref(choice),
                                     route_stat,
                                     do_cache,
                                     std::ref(kill_flags[i]),
                                     std::ref(thread_errors[i]));

        }

        for (unsigned int i=0;i<nt;i++){
            threads[i].join();
        }

        stop_console_output();

        for (unsigned int i=0;i<nt;i++){
            errors.collect(thread_errors[i]);//get all of the errors found in the threads
        }

        for (unsigned int i=0;i<nt;i++){
        
            if (kill_flags[i]){
                
                push_time();
                push_output("Cancelled.\n");

                Fl::lock();
                root.calculating=FREE;
                root.killthread.reset();
                Fl::unlock();
                calc_message_off();

                return NULL;
            }
        }

        output.str("");
        output.precision(3);
        output.setf( std::ios::fixed, std:: ios::floatfield );
        output<<"------------------- >> "<<100<<"% calculated."<<std::endl;
        
        Fl::lock(); //make the call directly to ensure it reaches the screen "first"
        output_mutex.lock();
        output_string = output.str();    
        text->buff->remove(buffer_point,text->buff->length()); //buffer_point and output_string are protected, everything else only used by main thread
        text->buff->append(output_string.c_str());
        output_mutex.unlock();

        text->display->buffer(text->buff);
        text->display->insert_position(text->buff->line_start(text->buff->length()));//moves insert position to the character corresponding to the start of the line that the last character is on
        text->display->show_insert_position(); //scrolls to that point
        text->display->redraw();

        output_mutex.lock();
        finished_console_output=1;
        output_mutex.unlock();
        Fl::unlock();


        custom_metric.reserve(size);

        for (unsigned int i=0;i<nt;i++){
            for (unsigned int j=0;j<temp_metric[i].size();j++){
                double temp = temp_metric[i][j];
                if (isinf(temp)){
                    temp = -2;
                }
                if (isnan(temp)){
                    temp = -2;
                }
                custom_metric.push_back(temp);
            }            
        }


        //end threading
        
    
        
    }
    push_time();
    push_output("...done.\n");
    
    
    if (errors.is_error()){
        std::vector<std::string> error_msgs = errors.get_error_messages();
        for (unsigned int i=0;i<error_msgs.size();i++){
            push_time();
            push_output(error_msgs[i]);
        }
    }

    
    Fl::lock();
    
    
    metric_description temp_des;
    
    temp_des.destination_crit= ((destination_criteria_choice->active())  ?   destination_criteria_choice->value()   :   -1);
    temp_des.route=((route_type_choice->active())  ?   route_type_choice->value()   :   -1);
    temp_des.metric=((custom_metric_choice->active())  ?   custom_metric_choice->value()   :   -1);
    temp_des.visible=((custom_metric_visibility_choice->active())  ?   custom_metric_visibility_choice->value()   :   -1);
    temp_des.floor=((custom_metric_floor_choice->active())  ?   custom_metric_floor_choice->value()   :   -1);
    temp_des.stat=((custom_metric_stat_choice->active())  ?   custom_metric_stat_choice->value()   :   -1);
    temp_des.destination=((custom_metric_destination_choice->active())  ?   custom_metric_destination_choice->value()   :   -1);
    temp_des.range_option=((custom_metric_range_choice->active())  ?   custom_metric_range_choice->value()   :   -1);
    temp_des.basis=custom_metric_basis->value();
    temp_des.route_break = ((route_stat_choice->active())  ?   route_stat_choice->value()   :   -1);
    temp_des.self_count = ((custom_metric_self_count->active())  ?   custom_metric_self_count->value()   :   -1);
    
    temp_des.iterations = ((max_iterations->active())  ?   iter   :   -1);
    temp_des.inverse = inverse;
    
    temp_des.eig_tolerance = ((eig_tol->active())   ?    tol     :    -1);
    temp_des.metric_range = ((custom_metric_range->active())   ?    range    :      -1 );
    temp_des.metric_range2 = ((custom_metric_range2->active())   ?    range2    :      -1 );
    temp_des.metric_turn = root.current->turn_definition;
    
    temp_des.errors = errors;

    Fl::unlock();
    
    
    
    int ch = metric_slot_choice->value();
    
    if ((ch>=0)&&(ch<static_cast<int>(root.current->metrics.size()))){
        

        double min=0,max=0;
        
        if(custom_metric.size()>0){
            
            min=9e99;
            for (unsigned int i=0;i<custom_metric.size();i++){
                if ((custom_metric[i]<min)&&(custom_metric[i]>=0)){
                    min=custom_metric[i];
                }
            }
            
            max=*std::max_element(custom_metric.begin(),custom_metric.end());
        }

        metrics_mutex.lock();

        root.current->max_plot_metric = max;
        root.current->min_plot_metric = min;


        root.current->plot_metric=custom_metric;
        root.current->plot_metric_des = temp_des;
        root.current->metrics[ch]=std::move(custom_metric);
        root.current->metric_des[ch]=std::move(temp_des);


        metrics_mutex.unlock();

        Fl::lock();
        draw->deselect();
        Fl::unlock();

        prepare_undo();//new here ?!?!?!

    }
    else if (ch==static_cast<int>(root.current->metrics.size())){

        double min=0,max=0;
        
        if(custom_metric.size()>0){
            
            min=9e99;
            for (unsigned int i=0;i<custom_metric.size();i++){
                if ((custom_metric[i]<min)&&(custom_metric[i]>=0)){
                    min=custom_metric[i];
                }
            }
            
            max=*std::max_element(custom_metric.begin(),custom_metric.end());
        }

        metrics_mutex.lock();

        root.current->max_plot_metric = max;
        root.current->min_plot_metric = min;

        root.current->plot_metric=custom_metric;
        root.current->plot_metric_des = temp_des;
        root.current->metrics.push_back(std::move(custom_metric));
        root.current->metric_des.push_back(std::move(temp_des));


        metrics_mutex.unlock();

        Fl::lock();
        draw->deselect();
        Fl::unlock();

        prepare_undo();

    }
    else{
        std::cout<<"Error. Report bug."<<std::endl;
        Fl::lock();
        root.calculating=FREE;
        Fl::unlock();
        calc_message_off();
        return NULL;
    }

    Fl::lock();
    plot->histogram();
    root.calculating=FREE;
    Fl::unlock();

    Fl::awake(&redraw_from_thread,this);

    calc_message_off();

    return NULL;

}


void base::create_structures_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->create_structures();
}

void base::create_structures(){
    
    container* root = &(this->root);
    
    
    if (root->calculating==FREE){
        
        root->calculating=CALC_STRUCTURES;
        calc_message_on();
        root->current->map.clear();
        turn_ang->deactivate();
        zfloor->deactivate();

        test_thread = std::thread(&map_structures::calculate_distances_paths_static,this);
        test_thread.detach();

    }
    else{
        calc_cb();
    }
    
    
}

void* base::route_thread_static(void* data){

    base* program = static_cast<base*> (data);
    program->route_thread();

    return NULL;

}


void* base::route_thread(){
    
    map_structures& map = root.current->map;
    
    int choice=route_type_c->value();

    int start = -1;
    int end = -1;

    std::stringstream s;
    s<<start_route_input->value();

    try{
        start = stoi(s.str());
    }
    catch (...){
        start = -1;
        end = -1;
    }
    
    s.str("");
    s<<end_route_input->value();

    try{
        end = stoi(s.str());
    }
    catch (...){
        start = -1;
        end = -1;
    }

  
    push_time();
    push_output("Calculating route...\n");
    

    if  ((!root.current->map_valid)||
         ((!root.current->segs_valid)&&(root.current->use_segments))||
         ((!root.current->turns_valid)&&(root.current->use_turns)))
    {
        root.current->map.clear();
        root.current->map.calculate_distances_paths(this);
        root.calculating=CALC_ROUTE;
    }

    if (root.killthread.get()){
        root.killthread.reset();
        root.calculating=FREE;
        calc_message_off();
        return NULL;
    }

    
    double value;
    
    std::vector<std::vector<int> > temp =map.calculate_route(&root,map,choice,start,end,root.current->dual_route,1000,value);
    
    push_time();
    
    s.str("");
    if (temp.size()>0){
        s<<"...done. "<<temp.size()<<" equivalent route(s) with metric value "<< value<<"."<<std::endl;
    }
    else{
        s<<"...no valid routes. Check input."<<std::endl;   
    }
    push_output(s.str().c_str());
    
    Fl::lock();
    root.current->route =temp;
    root.calculating=FREE;
    
    Fl::unlock();
    Fl::awake(&redraw_from_thread,this);

    calc_message_off();

    return NULL;
    
}

void base::ok_route_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->ok_route_cb();
}

void base::ok_route_cb(){
    route_display->hide();
    //Fl::focus(this->win);

    THREAD_STATE& calculating = root.calculating;

    if (calculating == FREE){

        if (!root.current->map_valid){
            int i = choice("Proceeding incurs recalculation of graph structures to match displayed graph. Continue?","Yes","No",0,"Proceed?");
            if (i){
                return;
            }
        }

        root.current->route.clear();
        std::vector<std::vector<int> >().swap(root.current->route);

        calculating=CALC_ROUTE;
        calc_message_on();
        
        if (!root.draw_route){

            Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[SHOW_ROUTE_INDEX]);                
            temp->do_callback(menu,this);
            temp->set();
            
        }

        test_thread = std::thread(&base::route_thread_static,this);
        test_thread.detach();

        custom_metric_calc->hide();
    }
    else{
        calc_cb();
    }
}

void base::visibility_select_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->visibility_select_cb();
}

void base::visibility_select_cb(){

    if ((show_visibility->value())&&(!root.current->vis_valid)&&(root.calculating==FREE)){

        int i = choice("Proceeding incurs calculation of visibility data to match displayed locations/barriers. Continue?","Yes","No",0,"Proceed?");
        if (i){
            show_visibility->value(0);
            redraw();
            return;
        } 
        else{
            draw->deselect();
            mark_locations_choice->value(0);
            mark_locations_choice->do_callback();
            root.calculating=CALC_VIS;
            calc_message_on();
            std::thread th = std::thread(&base::visibility_calc1,this);
            th.detach();
        }
    }
    else if ((show_visibility->value())&&(!root.current->vis_valid)){
        show_visibility->value(0);
        calc_cb();
        push_time();
        push_output("Visibility data requires recalculation, but a calculation is in progress - Cannot display visibility.\n");
    }
    else if (!show_visibility->value()){
        if ((!show_locations[0]->value())&&(mark_locations_choice->value()==1)){//display origins if continuing to edit
            show_locations[0]->value(1);
            show_locations[0]->do_callback();
        }
    }
 
    redraw();   
}
