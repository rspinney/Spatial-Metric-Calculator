/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/graph.hpp"
#include "./include/minheap.hpp"
#include "./include/base.hpp"

#undef min
#undef max


bool equal(double a, double b)
{
    return fabs(a - b) < EPSILON;
}


void map_structures::clear(){
    
    
    angular_array.clear();
    turns_array.clear();
    distance_array.clear();
    segments_array.clear();
    seg_angular_array.clear();
    seg_turns_array.clear();
    seg_distance_array.clear();
    seg_distance_3d_array.clear();
    seg_segments_array.clear();
    distance_3d_array.clear();
    dual_points.clear();
    points2dual.clear();
    points_2_segs.clear();
    links.clear();
    
    
    std::vector<graph_routes>().swap(angular_array);
    std::vector<graph_routes>().swap(turns_array);
    std::vector<graph_routes>().swap(distance_array);
    std::vector<graph_routes>().swap(distance_3d_array);
    std::vector<graph_routes>().swap(segments_array);
    
    std::vector<graph_routes>().swap(seg_angular_array);
    std::vector<graph_routes>().swap(seg_turns_array);
    std::vector<graph_routes>().swap(seg_distance_array);
    std::vector<graph_routes>().swap(seg_distance_3d_array);
    std::vector<graph_routes>().swap(seg_segments_array);
    
    std::vector<vertex>().swap(links);
    std::vector<vertex>().swap(dual_points);
    
    num_segments=0;
    num_points=0;
    num_links=0;
    
    line_segments.clear();
    std::vector<int32_t> ().swap(line_segments);
    
    G_dual_ang.clear();
    G_dual_turn.clear();
    S_dual_ang.clear();
    S_dual_turn.clear();
    S.clear();
    S_dist.clear();
    S_dist_3D.clear();
    G.clear();
    G_dist.clear();
    G_dist_3D.clear();
            
    
    choice_cache_G_dist.clear();
    choice_cache_G_dist_3D.clear();
    choice_cache_G.clear();
    choice_cache_G_ang.clear();
    choice_cache_G_turn.clear();
    choice_cache_S_dist.clear();
    choice_cache_S_dist_3D.clear();
    choice_cache_S.clear();
    choice_cache_S_ang.clear();
    choice_cache_S_turn.clear();
    
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_G_dist);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_G_dist_3D);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_G);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_G_ang);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_G_turn);
    
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_S_dist);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_S_dist_3D);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_S);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_S_ang);
    std::vector<std::vector<route_weight_number> >().swap(choice_cache_S_turn);
    
}



//generate all shortest paths between start node and node.

void map_structures::iterate_path_reference(return_iterate* to_return,const std::vector< std::vector<int> > &path,const std::vector< int> &mult_path,const int &start_node,const int &node,const int &/*finish*/, int /*depth*/,const int &size,int N,int /*NN*/){
    
    

    to_return->error.clear();
    to_return->paths.clear();

    
    
    std::vector<std::vector<int> > mult_choice,mult_choice_new;
    
    mult_choice.push_back(std::vector<int>(0));
    mult_choice.push_back(std::vector<int>(0));
    
    mult_choice_new=mult_choice;
    
    int iterating=1;
    int first=1;
    
    int count_attempt=0;
    
    while (iterating==1){
        
        count_attempt++;
        
        mult_choice_new[0].clear();
        mult_choice_new[1].clear();
        
        
        int split_node=-1;
        
        for (int i=mult_choice[0].size()-1;i>=0;i--){
            if (mult_choice[0][i]<(mult_choice[1][i]-1)){
                split_node=i;
                break;
            }
        }
        
        if ((!first)&&(split_node==-1)){
            iterating=0;
            break;
        }
        
        if (first){
            first=0;
        }
        
        std::vector<int> path_temp;
        path_temp.push_back(node);
        
        
        
        int n=node;
        int count=0;
        int error=0;
        

        while ((n!=start_node)&&(!error)){
            
            int mc;
            
            if (count==split_node){
                mc=mult_choice[0][count]+1;
            }
            else if (count<split_node){
                mc=mult_choice[0][count];
            }
            else{
                mc=0;
            }
                     
                
            mult_choice_new[0].push_back(mc);
            mult_choice_new[1].push_back(mult_path[n]);
            
            int new_node= path[n][mc];
            
            path_temp.push_back(new_node);
            
            n=new_node;
            
            for (unsigned int i=0;i<path_temp.size()-1;i++){
                if ((n!=start_node)&&(n==path_temp[i])){
                    to_return->error.LOOPS_DETECTED=1;
                    error=1;
                }
            }
           
            
            if (count>size){//path longer than size of graph - shouldn't happen
                error=1;
                to_return->error.LOOPS_DETECTED=1;
            }
            
            count++;
            
        }
       
        if (!error){
            to_return->paths.push_back(path_temp);
        }
       
        mult_choice=mult_choice_new;
        
        
        if (count_attempt>=N){
            if (to_return->paths.size()){
                to_return->error.MAX_PATHS_REACHED=1;
                break;
            }
            else{
                to_return->error.MAX_PATHS_REACHED_NONE_FOUND=1;
                break;
            }
        }
         
    }
 
    
}


void map_structures::populate_raw_inner(int start,int end,std::vector<std::vector<route_weight_number> >* choice,const graph &G,int dual,const map_structures* map,int iterations,int id,int &flag,kill_switch &kill, ERROR_CODE &error,int n_total){

    std::vector<double> weight;
    std::vector<int> visited;
    graph_routes temp;
    return_iterate answer;

    int n = end-start;

    std::stringstream output;

    boost::posix_time::ptime time =boost::posix_time::microsec_clock::local_time();
    boost::posix_time::ptime time_inter =boost::posix_time::microsec_clock::local_time();
    std::this_thread::sleep_for (std::chrono::milliseconds(100));

    int interval = 1;
    int dt_goal = 100;

    for (int i = start;i<end;i++){

        

        dijkstra_reference_list_heap_raw(&temp,G,i,dual, map->num_links,weight, visited);
            
        for (int j=0;j<n_total;j++){

            (*choice)[i][j].weight = temp.weight[j];
            
            iterate_path_reference(&answer,temp.path,temp.mult_path, temp.node_origin,temp.end_node[j],temp.end_node[j],0,map->num_points,iterations,iterations);
            
            error.collect(answer.error); //pass on path iteration errors
            
            if (temp.weight[j]>9e98){
                (*choice)[i][j].number = 0;
            }
            else{
                (*choice)[i][j].number = static_cast<int>( answer.paths.size() );
            }
        }

        if (i%interval==0){
            
            boost::posix_time::ptime time_new = boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration dur = time_new - time;
            boost::posix_time::time_duration dur_inter = time_new - time_inter;
            time_inter = time;
            double dt = dur.total_milliseconds();
            double dt_inter = dur_inter.total_milliseconds();

            interval = std::max(1,static_cast<int>(dt_goal/std::max(1.0,dt_inter))*interval);   

            if (dt>dt_goal){

                time = time_new;

                if (kill.get(id)){
                    flag=1;
                    program->stop_console_output();
                    return;
                }

                program->calc_progress.set(id,100.0*(static_cast<double>(i-start+1)/static_cast<double>(n)));
            
            }
        }   
    }

}

//multi-threaded version
void map_structures::populate_raw2(std::vector<std::vector<route_weight_number> >* choice,const graph& G,int seg, int dual,const map_structures* map, int iterations,ERROR_CODE& error,kill_switch& kill){
    
    int n;
    
    if (!dual){
        if (!seg){
            n=map->num_points;
        }
        else{
            n=map->num_links;
        }
    }
    else{
        if (!seg){
            n=2*map->num_links+map->num_points;
        }
        else{
            n=3*map->num_links;
        }
    }
    
    if (n==0){
        error.NO_GRAPH=1;
        return;
    }

    choice->resize(n,std::vector<route_weight_number>(n));
    
    program->push_time();
    program->push_output("Starting weights and route numbers calculation...\n");

    

    unsigned int nt = std::max(static_cast<unsigned int>(MIN_THREAD),std::thread::hardware_concurrency());

    if (n<static_cast<int>(nt) ){
        nt=n;
    }

    program->output_mutex.lock();
    program->calc_progress.reset(nt);
    program->root.killthread.reset(nt);
    program->output_mutex.unlock();

    if (program->calc_progress.size()<static_cast<int>(nt)) nt = program->calc_progress.size();
    if (program->root.killthread.size()<static_cast<int>(nt)) nt = program->root.killthread.size(); 

    Fl::awake(base::add_console_updater,program);
    program->wait_for_console();


    std::vector<int> index_points(nt,0);                
    index_points[0]=0;
    for (unsigned int i=1;i<nt;i++){
        
        index_points[i]=index_points[i-1] + std::max(1,n/static_cast<int>(nt));
    }
    
    std::vector<int> flags(nt,0);
    std::vector<std::thread> threads(nt);

    for (unsigned int i=0;i<nt;i++){
        int start = index_points[i];
        int end;
        if (i<nt-1){
            end = index_points[i+1];
        }
        else{
            end = n;
        }
        threads[i] = std::thread(&map_structures::populate_raw_inner,this,start,end,choice,G,dual,map,iterations,i,std::ref(flags[i]),std::ref(kill),std::ref(error),n);
    }

    for (unsigned int i=0;i<nt;i++){
        threads[i].join();
    }

    program->stop_console_output();

    for (unsigned int i=0;i<nt;i++){
        if (flags[i]){
            return;
        }
    }

    std::stringstream output;
    output.str("");
    output.precision(3);
    output.setf( std::ios::fixed, std:: ios::floatfield );
    output<<"------------------- >> "<<100<<"% calculated"<<std::endl;
    program->output_mutex.lock();
    program->output_string = output.str();
    program->output_mutex.unlock();
    Fl::awake(&base::reset_console_buffer_and_push,program);
    std::this_thread::sleep_for (std::chrono::milliseconds(100));

    program->push_time();
    program->push_output("...done.\n");

    return;
    
}




//obsolete - now performed in parallel by populate_raw2
void map_structures::populate_raw(std::vector<std::vector<route_weight_number> >* choice,const graph& G,int seg, int dual,const map_structures* map, int iterations,ERROR_CODE& error,kill_switch& kill){
    
    int n;
    
    if (!dual){
        if (!seg){
            n=map->num_points;
        }
        else{
            n=map->num_links;
        }
    }
    else{
        if (!seg){
            n=2*map->num_links+map->num_points;
        }
        else{
            n=3*map->num_links;
        }
    }
    
    choice->resize(n,std::vector<route_weight_number>(n));
    std::vector<double> weight;
    std::vector<int> visited;
    graph_routes temp;
    return_iterate answer;
    
    program->push_time();
    program->push_output("Starting weights and route numbers calculation...\n");
    
    int counter = 0;
    int start_buff=program->text->buff->length();
    std::stringstream output;

    for (int i=0;i<n;i++){
        
        if (kill.get()){
            return;
        }
              
        counter++;
        if (((counter)>0.01*n)||((i+1)==n)){
            counter=0;
            Fl::lock();
            program->text->buff->remove(start_buff,program->text->buff->length());
            Fl::unlock();
            output.str("");
            output.precision(3);
            output.setf( std::ios::fixed, std:: ios::floatfield );
            output<<"------------------- >> "<<100*(static_cast<double>(i+1)/static_cast<double>(n))<<"% calculated"<<std::endl;
            Fl::lock();
            program->push_output(output.str());
            Fl::unlock();
        }
             
        dijkstra_reference_list_heap_raw(&temp,G,i,dual, map->num_links,weight, visited);
        
        for (int j=0;j<n;j++){
            (*choice)[i][j].weight = temp.weight[j];
        
            
            iterate_path_reference(&answer,temp.path,temp.mult_path, temp.node_origin,temp.end_node[j],temp.end_node[j],0,map->num_points,iterations,iterations);
            
            error.collect(answer.error);//pass on path iteration errors
            
            if (temp.weight[j]>9e98){
                (*choice)[i][j].number = 0;
            }
            else{
                (*choice)[i][j].number = static_cast<int>( answer.paths.size() );
            }
        }
    }

    Fl::lock();
    program->text->buff->remove(start_buff,program->text->buff->length());
    Fl::unlock();
    output.str("");
    output.precision(3);
    output.setf( std::ios::fixed, std:: ios::floatfield );
    output<<"------------------- >> "<<100<<"% calculated"<<std::endl;
    Fl::lock();
    program->push_output(output.str());
    Fl::unlock();

    program->push_time();
    program->push_output("...done.\n");
    
}


void map_structures::dijkstra_reference_list_heap_raw(graph_routes* return_struct, const graph& G,int start_node,int dual,int num_links,std::vector<double>& weight,std::vector<int>& visited){

    
    std::vector<int>& mult_path = return_struct->mult_path;
    std::vector<std::vector<int> >& path = return_struct->path;
    
    int num_nodes=G.neighbours.size();
    
    
    if (static_cast<int>(return_struct->mult_path.size())!=num_nodes){
        
        return_struct->end_node.resize(num_nodes);
        return_struct->weight.resize(num_nodes);
        mult_path.resize(num_nodes);
        path.resize(num_nodes);
        
        
    }
    
    if (static_cast<int>(weight.size())!=num_nodes){
        weight.resize(num_nodes);
    }
    if (static_cast<int>(visited.size())!=num_nodes){
        visited.resize(num_nodes);
    }
    
    for (int i=0;i<num_nodes;i++){
        
        mult_path[i]=1;
        path[i]=std::vector<int>(1,start_node);
        weight[i]=INFINITY;
        visited[i]=0;
    }
    
    
    
    weight[start_node]=0;
    visited[start_node]=0;
    
    
    //HEAP
    
    std::vector<weight_index> vec_in;
    
    vec_in.reserve(num_nodes);

    for (int i=0;i<num_nodes;i++){
        weight_index temp;
        temp.weight=INFINITY;
        //temp.weight_sort=INFINITY;
        temp.index=i;
        vec_in.push_back(temp);
    }
    
    vec_in[start_node].weight=0;
    //vec_in[start_node].weight_sort=0;
    
    
    min_heap heap(vec_in);
    
    //END HEAP
    

    
    int count=1,current=0;
    double temp=0;
    
    
    double val=0,weight_current=0;
    
    while (count<num_nodes+1){
        
        current=heap.get_min(); //explicitly returns original matrix index
        weight_current = heap.data[0].weight; // get node value
        heap.data[0].weight=INFINITY; //send visited nodes to the bottom of the heap'
        heap.send_down(0); //actually send it down
        
        for (unsigned int ii=0;ii<G.neighbours[current].size();ii++){
            
            int i=G.neighbours[current][ii];
            
            val=G.weights.at(current).at(i);  //from current to i
            
            if (dual){
                if (i==start_node){
                    val=-1;
                }
                
                if ((current>=2*num_links)&&(current!=start_node)){
                    val=-1; //<<-- absolutely necessary to stop routing through into/out of nodes (ie you can go into node > 2*num_links, but not back out (but you are allowed out of start)
                }
            }
            
                        
            if (val>=0){
                
                temp=weight_current+val;
                
                int heap_index = heap.index_back[i];  //we knwo node i but need to know where in the heap it is
                
                double weight_compare = weight[i];//heap.data[heap_index].weight; //this is its actual weight (visited or not)
                
                         
                if (equal(temp,weight_compare)){  //fine
                    mult_path[i]++;
                    path[i].push_back(current);
                }
                else if (temp<weight_compare){
                    
                    weight[i]=temp;
                    heap.data[heap_index].weight=temp;
                    heap.send_up(heap_index); //send up as it might be the new minimum
                    
                    path[i][0]=current;
                    mult_path[i]=1;
                    path[i].resize(1);
                }     
                
            }
        }
        
        
        count++;
    }
    
    return_struct->node_origin=start_node;
    
    for (int i=0;i<num_nodes;i++){
            
        return_struct->end_node[i]=i;
    }
    
    return_struct->origin=start_node;

    return_struct->weight = std::move(weight);
     
    
}



void map_structures::dijkstra_reference_list_heap(graph_routes* return_struct,const graph& G,int start_node,int dual, int num_links,std::vector<double>& weight,std::vector<int>& visited){
    

    std::vector<int>& mult_path = return_struct->mult_path;
    std::vector<std::vector<int> >& path = return_struct->path;
    
    int num_nodes=G.neighbours.size();
    int num_points=num_nodes;
    
    
    if (dual){
        
        num_points=num_nodes-2*num_links;
        start_node+=2*num_links;
        
    }
    
    
    if (static_cast<int>(return_struct->mult_path.size())!=num_nodes){
        
        return_struct->end_node.resize(num_nodes);
        return_struct->weight.resize(num_points);
        mult_path.resize(num_nodes);
        path.resize(num_nodes);
        
        
    }
    
    if (static_cast<int>(weight.size())!=num_nodes){
        weight.resize(num_nodes);
    }
    if (static_cast<int>(visited.size())!=num_nodes){
        visited.resize(num_nodes);
    }
    
    for (int i=0;i<num_nodes;i++){
        
        mult_path[i]=1;
        path[i]=std::vector<int>(1,start_node);
        weight[i]=INFINITY;
        visited[i]=0;
    }

    
    weight[start_node]=0;
    visited[start_node]=0;
    
    
    //HEAP
    
    
    std::vector<weight_index> vec_in;
    
    for (int i=0;i<num_nodes;i++){
        weight_index temp;
        temp.weight=INFINITY;
        //temp.weight_sort=INFINITY;
        temp.index=i;
        vec_in.push_back(temp);
    }
    
    vec_in[start_node].weight=0;
    //vec_in[start_node].weight_sort=0;
    
    
    min_heap heap(vec_in);
    
    //END HEAP
    
    int count=1,current;
    double temp;
    
    
    double val,weight_current;
    
    while (count<num_nodes+1){                        
        
        current=heap.get_min(); //explicitly returns original matrix index
        weight_current = heap.data[0].weight; // get node value
        heap.data[0].weight=INFINITY; //send visited nodes to the bottom of the heap'
        heap.send_down(0); //actually send it down
        
        
        //for (int i=0;i<num_nodes;i++){
        for (unsigned int ii=0;ii<G.neighbours[current].size();ii++){
            
            int i=G.neighbours[current][ii];
            
            val=G.weights.at(current).at(i);//[current][i];
            
            if (dual){
                if (i==start_node){
                    val=-1;
                }
                
                if ((current>=2*num_links)&&(current!=start_node)){
                    val=-1;
                }
            }
                                
            if (val>=0){
                
                temp=weight_current+val;
                
                int heap_index = heap.index_back[i];  //we knwo node i but need to know where in the heap it is
                
                double weight_compare = weight[i];// heap.data[heap_index].weight; //this is its actual weight (visited or not)
                
                if (equal(temp,weight_compare)){  //fine
                    mult_path[i]++;
                    path[i].push_back(current);
                }
                else if (temp<weight_compare){
                    
                    weight[i]=temp;
                    heap.data[heap_index].weight=temp;
                    heap.send_up(heap_index); //send up as it might be the new minimum
                    
                    path[i][0]=current;
                    mult_path[i]=1;
                    path[i].resize(1);
                }               
                
            }
        }
        
        
        count++;
    }
        

    return_struct->node_origin=start_node;
    
    
    for (int i=0;i<num_points;i++){
        if (dual) {
            
            return_struct->end_node[i]=i+2*num_links;
        }
        else{
            
            return_struct->end_node[i]=i;
        }
    }
    
    
    if (dual){
        return_struct->origin=start_node-2*num_links;
        
        for (int i=0;i<num_points;i++){
            
            return_struct->weight[i]=weight[i+2*num_links];
        }
    }
    else{
        return_struct->origin=start_node;

        return_struct->weight=std::move(weight);
        
    }   
    
}

void* map_structures::calculate_distances_paths_static(void* data){

    base* program = static_cast<base*>(data);

    program->root.current->map.calculate_distances_paths(data);

    return NULL;
}


void* map_structures::calculate_distances_paths(void* /*data*/){
    
    container* root = &(program->root); //shadow "container root"
    
    const std::vector<vec>& points = root->current->points;
    const std::vector<vertex>& adj_list = root->current->adj_list;
    map_structures* map=&(root->current->map);
    
    
    int use_segments = program->root.current->use_segments;
    int use_turns = program->root.current->use_turns;

    std::stringstream output;

    double pi=3.141592653589793238;
    
    std::vector<vertex> dual_points,lines,links_temp;
    std::vector<int> lines_to_links,links_to_lines;
    int num_points,num_segments,num_dual_points,num_dual_points_seg;
    
    
    num_points=points.size();
    
    //CONSTRUCT SEGMENT AND DISTANCE MATRICES//

    root->killthread.reset();

    program->push_time();
    program->push_output("Starting construction of weight matrices for all metrics/starting configurations... \n");
    
    
    //populate points to segments matrix
 
    
    //CREATE LINKS/SEGEMENTS ARRAY IN TERMS OF POINTS///
   
    
    std::map<vertex,int> points_2_segs;
    
    for (unsigned int i=0;i<root->current->adj_list.size();i++){


        if (root->killthread.get()){
                
            program->push_time();
            program->push_output("Cancelled.\n");

            Fl::lock();
            root->current->map_valid=0;
            root->current->segs_valid=0;
            root->current->turns_valid=0;
            root->calculating=FREE;
            program->calc_message_off();
            Fl::unlock();

            return NULL;
        }


        vertex temp,temp1;
        
        temp.start = adj_list[i].start;
        temp.end = adj_list[i].end;
        
        temp1.start = adj_list[i].end;
        temp1.end = adj_list[i].start;
        
        points_2_segs[temp]=i;
        points_2_segs[temp1]=i;
    }
    
    
    const std::vector<vertex>& links = root->current->adj_list;
    
    
    num_segments=links.size();
    
    
    
    graph S,S_dist,S_dist_3D,G,G_dist,G_dist_3D;
    
    
    for (int i=0;i<num_points;i++){


        if (root->killthread.get()){
                
            program->push_time();
            program->push_output("Cancelled.\n");

            Fl::lock();
            root->current->map_valid=0;
            root->current->segs_valid=0;
            root->current->turns_valid=0;
            root->calculating=FREE;
            program->calc_message_off();
            Fl::unlock();

            return NULL;
        }


        G.neighbours.push_back(std::vector<int>(0));  //vector of neighbours for each node
        G_dist.neighbours.push_back(std::vector<int>(0));
        G_dist_3D.neighbours.push_back(std::vector<int>(0));
        
        G.weights.push_back(std::map<int,double>());  //vector of hash tables which for neighbour node's values encode as keys for weights from the node to those neighbours
        G_dist.weights.push_back(std::map<int,double>());
        G_dist_3D.weights.push_back(std::map<int,double>());
    }
    
    
    for (unsigned int ii=0;ii<root->current->adj_list.size();ii++){
        

        if (root->killthread.get()){
                
            program->push_time();
            program->push_output("Cancelled.\n");

            Fl::lock();
            root->current->map_valid=0;
            root->current->segs_valid=0;
            root->current->turns_valid=0;
            root->calculating=FREE;
            program->calc_message_off();
            Fl::unlock();

            return NULL;
        }


        int i = root->current->adj_list[ii].start;
        int j = root->current->adj_list[ii].end;
        
        double r2=sqrt((points[i].x-points[j].x)*(points[i].x-points[j].x)+(points[i].y-points[j].y)*(points[i].y-points[j].y));
        double r3=sqrt((points[i].x-points[j].x)*(points[i].x-points[j].x)+(points[i].y-points[j].y)*(points[i].y-points[j].y)+(points[i].z-points[j].z)*(points[i].z-points[j].z));
        
        if (equal(r2,0)) r2=1e-6;
        if (equal(r3,0)) r3=1e-6;
        
        G.neighbours[i].push_back(j);
        G.neighbours[j].push_back(i);
        G_dist.neighbours[i].push_back(j);
        G_dist.neighbours[j].push_back(i);
        G_dist_3D.neighbours[i].push_back(j);
        G_dist_3D.neighbours[j].push_back(i);

        G.weights[i][j]=1;
        G.weights[j][i]=1;
        G_dist.weights[i][j]=r2;
        G_dist.weights[j][i]=r2;
        G_dist_3D.weights[i][j]=r3;
        G_dist_3D.weights[j][i]=r3;
        
    }
    
    
    //CREATE POINTS BASED ARRAYS//
    
    program->push_time();
    program->push_output("...created vertex based metric arrays...\n");
    
    ///CREATE SEGMENT BASED ARRAYS//

    if (use_segments){
    
        for (int i=0;i<num_segments;i++){
                    
            
            S.neighbours.push_back(std::vector<int>(0));
            S_dist.neighbours.push_back(std::vector<int>(0));
            S_dist_3D.neighbours.push_back(std::vector<int>(0));
            
            S.weights.push_back(std::map<int,double>());
            S_dist.weights.push_back(std::map<int,double>());
            S_dist_3D.weights.push_back(std::map<int,double>());
            
            
            for (int j=0;j<num_segments;j++){

                if (root->killthread.get()){
                
                    program->push_time();
                    program->push_output("Cancelled.\n");

                    Fl::lock();
                    root->current->map_valid=0;
                    root->current->segs_valid=0;
                    root->current->turns_valid=0;
                    root->calculating=FREE;
                    program->calc_message_off();         
                    Fl::unlock();

                    return NULL;
                }


                if (i!=j){
                    
                                    
                    if (((links[i].start==links[j].start))||((links[i].start==links[j].end))||((links[i].end==links[j].start))||((links[i].end==links[j].end))){
                        
                        
                        double r2=0.5*sqrt((points[links[i].start].x-points[links[i].end].x)*(points[links[i].start].x-points[links[i].end].x)+(points[links[i].start].y-points[links[i].end].y)*(points[links[i].start].y-points[links[i].end].y))+0.5*sqrt((points[links[j].start].x-points[links[j].end].x)*(points[links[j].start].x-points[links[j].end].x)+(points[links[j].start].y-points[links[j].end].y)*(points[links[j].start].y-points[links[j].end].y));
                        double r3=0.5*sqrt((points[links[i].start].x-points[links[i].end].x)*(points[links[i].start].x-points[links[i].end].x)+(points[links[i].start].y-points[links[i].end].y)*(points[links[i].start].y-points[links[i].end].y)+(points[links[i].start].z-points[links[i].end].z)*(points[links[i].start].z-points[links[i].end].z))+0.5*sqrt((points[links[j].start].x-points[links[j].end].x)*(points[links[j].start].x-points[links[j].end].x)+(points[links[j].start].y-points[links[j].end].y)*(points[links[j].start].y-points[links[j].end].y)+(points[links[j].start].z-points[links[j].end].z)*(points[links[j].start].z-points[links[j].end].z));
                        
                        if (equal(r2,0)) r2=2e-6;
                        if (equal(r3,0)) r3=2e-6;
                        
                        S.neighbours[i].push_back(j);
                        S_dist.neighbours[i].push_back(j);
                        S_dist_3D.neighbours[i].push_back(j);
                        S.weights[i][j]=1;
                        S_dist.weights[i][j]=r2;
                        S_dist_3D.weights[i][j]=r3;
                        

                    }
                }
                
            }
        }
        
        Fl::lock();
        program->push_time();
        program->push_output("...created edge based metric arrays...\n");
        Fl::unlock();
    
    }
    //CREATE DUALGRAPH POINTS//
    
    std::map<vertex,int> points2dual;
    graph G_dual_ang,G_dual_turn,S_dual_ang,S_dual_turn;

    if (use_turns){
        for (unsigned int i=0; i<2*(links.size())+(points.size());i++){
            
            vertex temp;
            
            if (i<links.size()){
                temp.start=links[i].start;  //forward direction for segment
                temp.end=links[i].end;
            }
            else if(i<2*links.size()){
                temp.end=links[i-links.size()].start; //reverse direction for segment
                temp.start=links[i-links.size()].end;
                
            }
            else{
                temp.start=i-2*links.size(); //entry/exit segments
                temp.end=i-2*links.size();
            }
            
            dual_points.push_back(temp);
        }
            
        
        
        for (unsigned int i=0;i<dual_points.size();i++){
            vertex temp;
            temp.start = dual_points[i].start;
            temp.end = dual_points[i].end;
            points2dual[temp]=i;
        }

        num_dual_points=dual_points.size();
        num_dual_points_seg=3*links.size();
        
        Fl::lock();
        program->push_time();
        program->push_output("...created dual-graph vertices...\n");
        Fl::unlock();
    
      //CREATE DUALGRAPH MATRICES//
            
        
        for (int i=0;i<num_dual_points;i++){    
            
            G_dual_ang.neighbours.push_back(std::vector<int>(0));
            G_dual_turn.neighbours.push_back(std::vector<int>(0));
            
            G_dual_ang.weights.push_back(std::map<int,double>());
            G_dual_turn.weights.push_back(std::map<int,double>());
            
            
            for (int j=0;j<num_dual_points;j++){

                if (root->killthread.get()){
                
                    program->push_time();
                    program->push_output("Cancelled.\n");

                    Fl::lock();
                    root->current->map_valid=0;
                    root->current->segs_valid=0;
                    root->current->turns_valid=0;
                    root->calculating=FREE;
                    program->calc_message_off();
                    Fl::unlock();

                    return NULL;
                }

                
                if (((dual_points[i].start==dual_points[i].end)||(dual_points[j].start==dual_points[j].end))&&(dual_points[i].end==dual_points[j].start)){
                    
                    G_dual_ang.neighbours[i].push_back(j);
                    G_dual_turn.neighbours[i].push_back(j);
                    
                    G_dual_ang.weights[i][j]=0;
                    G_dual_turn.weights[i][j]=0;
                    
                }
                else if(dual_points[i].end==dual_points[j].start){
                                    
                    int old_point,mid_point,new_point;
                    double x1,x2,y1,y2,theta1,theta2,temp;
                    
                    old_point=dual_points[i].start;
                    mid_point=dual_points[i].end;
                    new_point=dual_points[j].end;
                    
                    x1=points[new_point].x-points[mid_point].x;
                    y1=points[new_point].y-points[mid_point].y;
                    
                    x2=points[mid_point].x-points[old_point].x;
                    y2=points[mid_point].y-points[old_point].y;

                    
                    theta1=atan2(y1,x1)*360.0/(2.0*pi);
                    theta2=atan2(y2,x2)*360.0/(2.0*pi);
                    
                    temp=theta1-theta2;
                    if (temp>180){
                        temp-=360;
                    }
                    else if(temp< -180){
                        temp+=360;
                    }
                     
                    //dealing with floor changes: planar angle changes only
                    
                    if ((points[old_point].x==points[mid_point].x)&&(points[old_point].y==points[mid_point].y)){ //from vertical link
                        
                        temp =0;
                        
                    }
                    else if ((points[new_point].x==points[mid_point].x)&&(points[new_point].y==points[mid_point].y)){ //into vertical link
                        
                        temp=0;
                    }
                    
                    //end dealing with floor changes
                    
                    if (old_point==new_point){
                        //temp=180; //old version
                        temp=INFINITY; // but let's implement strictly no backtrcking
                    }
                    
                    G_dual_ang.neighbours[i].push_back(j);
                    G_dual_turn.neighbours[i].push_back(j);
                    
                    G_dual_ang.weights[i][j]=fabs(temp);
                    double tt;
                    if (fabs(temp)>program->root.current->turn_definition)  {
                        tt=1.0;
                        if (old_point==new_point){
                            tt=INFINITY; // strictly no backtracking
                        }
                    }
                    else{
                        tt=0.0;
                    }
                    G_dual_turn.weights[i][j]=tt;
                    
                }
            }
        }
        
        program->push_time();
        program->push_output("...created vertex based dual-graph metric arrays...\n");

    }
    
    
    if ((use_segments)&&(use_turns)){
    
        for (int i=0;i<num_dual_points_seg;i++){
            

            S_dual_ang.neighbours.push_back(std::vector<int>(0));
            S_dual_turn.neighbours.push_back(std::vector<int>(0));
            
            S_dual_ang.weights.push_back(std::map<int,double>());
            S_dual_turn.weights.push_back(std::map<int,double>());
            
            if (i<static_cast<int>(links.size())){
                
                S_dual_ang.neighbours[i].push_back(i+2*links.size());
                S_dual_turn.neighbours[i].push_back(i+2*links.size());
                S_dual_ang.weights[i][i+2*links.size()]=0;
                S_dual_turn.weights[i][i+2*links.size()]=0;
            }
            else if (i<2*static_cast<int>(links.size())){
                
                S_dual_ang.neighbours[i].push_back(i+links.size());
                S_dual_turn.neighbours[i].push_back(i+links.size());
                S_dual_ang.weights[i][i+links.size()]=0;
                S_dual_turn.weights[i][i+links.size()]=0;
            }
            else{
                S_dual_ang.neighbours[i].push_back(i-links.size());
                S_dual_turn.neighbours[i].push_back(i-links.size());
                S_dual_ang.weights[i][i-links.size()]=0;
                S_dual_turn.weights[i][i-links.size()]=0;
                S_dual_ang.neighbours[i].push_back(i-2*links.size());
                S_dual_turn.neighbours[i].push_back(i-2*links.size());
                S_dual_ang.weights[i][i-2*links.size()]=0;
                S_dual_turn.weights[i][i-2*links.size()]=0;
            }
            
            for (int j=0;j<num_dual_points_seg;j++){

                if (root->killthread.get()){
                
                    program->push_time();
                    program->push_output("Cancelled.\n");

                    Fl::lock();
                    root->current->map_valid=0;
                    root->current->segs_valid=0;
                    root->current->turns_valid=0;
                    root->calculating=FREE;
                    program->calc_message_off();        
                    Fl::unlock();

                    return NULL;
                }

                
                if ((i<2*static_cast<int>(links.size()))&&(j<2*static_cast<int>(links.size()))){
                    
                    if(dual_points[i].end==dual_points[j].start){
                        
                        int old_point,mid_point,new_point;
                        double x1,x2,y1,y2,theta1,theta2,temp;
                        
                        old_point=dual_points[i].start;
                        mid_point=dual_points[i].end;
                        new_point=dual_points[j].end;
                        
                        x1=points[new_point].x-points[mid_point].x;
                        y1=points[new_point].y-points[mid_point].y;
                        
                        x2=points[mid_point].x-points[old_point].x;
                        y2=points[mid_point].y-points[old_point].y;

                        theta1=atan2(y1,x1)*360.0/(2.0*pi);
                        theta2=atan2(y2,x2)*360.0/(2.0*pi);
                        
                        temp=theta1-theta2;
                        if (temp>180){
                            temp-=360;
                        }
                        else if(temp< -180){
                            temp+=360;
                        }
                        
                        //dealing with floor changes: planar angle changes only
                        
                        if ((points[old_point].x==points[mid_point].x)&&(points[old_point].y==points[mid_point].y)){ //from vertical link
                            temp =0;
                        }
                        else if ((points[new_point].x==points[mid_point].x)&&(points[new_point].y==points[mid_point].y)){ //into vertical link
                            temp=0;
                        }
                        
                        //end dealing with floor changes
                                                                
                        if (old_point==new_point){
                            //temp=180;//old version
                            temp=INFINITY; // strictly no backtracking
                        }
                        
 
                        S_dual_ang.neighbours[i].push_back(j);
                        S_dual_turn.neighbours[i].push_back(j);
                        
                        S_dual_ang.weights[i][j]=fabs(temp);
                        double tt;
                        if (fabs(temp)>program->root.current->turn_definition){
                            tt=1.0;
                            if (old_point==new_point){
                                tt=INFINITY; // strictly no backtracking
                            }
                        }
                        else{
                            tt=0.0;
                        }
                        S_dual_turn.weights[i][j]=tt;
                        
                        
                    }
                }
     
            }
        }
        Fl::lock();
        program->push_time();
        program->push_output("...created edge based dual-graph metric arrays...\n");
        Fl::unlock();
     
    }
    Fl::lock();
    program->push_time();
  
    program->push_output("...done.\n");
    Fl::unlock();
    /////////////////////END OF GRAPH CONSTRUCTION///////////////////
   
    
    ////////////////////START OF METRIC/PATH STRUCTURE GENERATION////////////
    
   
    Fl::lock();
    program->push_time();
    program->push_output("Wrapping up...\n");
    Fl::unlock();
    
    map->num_points=num_points;
    map->num_segments=num_segments;
    map->num_links=links.size();
    map->links=std::move(links);
    map->dual_points=std::move(dual_points);
    
    map->points2dual=std::move(points2dual);
    
    
    map->G=std::move(G);
    map->G_dist=std::move(G_dist);
    map->G_dist_3D=std::move(G_dist_3D);
    if (use_turns){
        map->G_dual_ang=std::move(G_dual_ang);
        map->G_dual_turn=std::move(G_dual_turn);
    }


    if (use_segments){
        map->S=std::move(S);
        map->S_dist=std::move(S_dist);
        map->S_dist_3D=std::move(S_dist_3D);
        if (use_turns){
            map->S_dual_ang=std::move(S_dual_ang);
            map->S_dual_turn=std::move(S_dual_turn);
        }
        
        map->points_2_segs=std::move(points_2_segs);
    }
    
    
    set_cache_arrays(map,use_segments,use_turns);
    
    Fl::awake(program->awake_map_warning_off,program);

    program->push_time();  
    program->push_output("done.\n");
    
    root->current->map_valid=1;
    if (use_segments){
        root->current->segs_valid=1;
    }
    else{
        root->current->segs_valid=0;   
    }
    if (use_turns){
        root->current->turns_valid=1;
    }
    else{
        root->current->turns_valid=0;   
    }
    root->calculating=FREE;
    program->calc_message_off();
    Fl::lock();
    
    if (program->floor_choice->value()>0){
        program->zfloor->activate();
    }
    Fl::unlock();
        
        
    return NULL;

  
}

void map_structures::set_cache_arrays(map_structures* map,int use_segments,int use_turns){
    
    std::vector<graph_routes> angular_array,turns_array,distance_array,distance_3d_array,seg_angular_array,seg_turns_array,seg_distance_array,seg_distance_3d_array,segments_array,seg_segments_array,seg_crow_3d_array,seg_crow_array,crow_array,crow_3d_array;
    
    
    for (int i=0;i<map->num_points;i++){
        graph_routes temp;
        temp.exist=0;
        distance_array.push_back(temp);
        distance_3d_array.push_back(temp);
        if (use_turns){
            angular_array.push_back(temp);
            turns_array.push_back(temp);
        }
        segments_array.push_back(temp);
        crow_array.push_back(temp);
        crow_3d_array.push_back(temp);
    }
    
    if (use_segments){

        for (int i=0;i<map->num_links;i++){
            graph_routes temp;
            temp.exist=0;
            seg_distance_array.push_back(temp);
            seg_distance_3d_array.push_back(temp);
            if (use_turns){
                seg_angular_array.push_back(temp);
                seg_turns_array.push_back(temp);
            }
            seg_segments_array.push_back(temp);
            seg_crow_array.push_back(temp);
            seg_crow_3d_array.push_back(temp);
        }
    }
    
    map->angular_array=angular_array;
    map->turns_array=turns_array;
    map->distance_array=distance_array;
    map->distance_3d_array=distance_3d_array;
    map->segments_array=segments_array;
    map->crow_array=crow_array;
    map->crow_3d_array=crow_3d_array;
    
    map->seg_angular_array=seg_angular_array;
    map->seg_turns_array=seg_turns_array;
    map->seg_distance_array=seg_distance_array;
    map->seg_distance_3d_array=seg_distance_3d_array;
    map->seg_segments_array=seg_segments_array;
    map->seg_crow_array=seg_crow_array;
    map->seg_crow_3d_array=seg_crow_3d_array;
 
    map->choice_cache_G_dist.clear();
    map->choice_cache_G_dist_3D.clear();
    map->choice_cache_G.clear();
    map->choice_cache_G_ang.clear();
    map->choice_cache_G_turn.clear();
    map->choice_cache_S_dist.clear();
    map->choice_cache_S_dist_3D.clear();
    map->choice_cache_S.clear();
    map->choice_cache_S_ang.clear();
    map->choice_cache_S_turn.clear();
    
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_G_dist);
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_G_dist_3D);
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_G);
        std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_G_ang);
        std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_G_turn);
    
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_S_dist);
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_S_dist_3D);
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_S);
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_S_ang);
    std::vector<std::vector<route_weight_number> >().swap(map->choice_cache_S_turn);
   
}

void* map_structures::calculate_lines_static(void* data){

    base* program = static_cast<base*>(data);
    program->root.current->map.calculate_lines(data);

    return NULL;
}

void* map_structures::calculate_lines(void* /*data*/){

    
    
    std::vector<int> line_segments;
    
    
    program->push_time();
    program->push_output("Starting line identification...\n");
    
    program->output_mutex.lock();
    program->buffer_point = program->text->buff->length();
    program->output_mutex.unlock();

    std::stringstream output;
    int size = program->root.current->map.links.size();
    
    int old_count=0;
    
    int size_out;
    
    size_out = ((0.01*static_cast<double>(size))<1) ? 1 : 0.01*size;
    
    std::vector<int> to_do;
    
    for (int i=0;i<size;i++){
        to_do.push_back(i);
    }
    
    
    graph_routes gr_temp;
    std::vector<double> weight_d;
    std::vector<int> visited_d;
    
    for (unsigned int i=0;i<to_do.size();i++){
        
        
        if (program->root.killthread.get()){
            program->push_time();
            program->push_output("Cancelled.\n");
            program->root.current->lines_valid=0;
            program->root.calculating=FREE;
            program->calc_message_off();
            
            return NULL;
        }
                
        
        if (program->root.current->map.seg_turns_array[to_do[i]].exist==1){
            gr_temp = program->root.current->map.seg_turns_array[to_do[i]];
        }
        else{
                     
            dijkstra_reference_list_heap(&gr_temp,program->root.current->map.S_dual_turn,to_do[i],1,program->root.current->map.links.size(),weight_d,visited_d);
            
        }
        
        
        int found=0;
        
        for (unsigned int j=0;j<line_segments.size();j++){
            
            int end_node = line_segments[j];
            
            if (gr_temp.weight[end_node]<1){
                found=1;
            }
        }
        
        if (!found){
            line_segments.push_back(to_do[i]);
            
            for (int j=i+1;j<size;j++){
                if (gr_temp.weight[to_do[j]]==0){
                    to_do.erase(to_do.begin()+j);
                    --size;
                }
            }
        }
        
        if ((static_cast<int>(i-old_count)>size_out)||(static_cast<int>(i+1)==size)){
            old_count=i;
            
            output.str("");
            output.precision(3);
            output.setf( std::ios::fixed, std:: ios::floatfield );
            output<<"------------------- >> "<<100*(static_cast<double>(i+1)/static_cast<double>(size))<<"% calculated"<<std::endl;
            program->output_mutex.lock();
            program->output_string = output.str();
            program->output_mutex.unlock();
            Fl::awake(&base::reset_console_buffer_and_push,program);
            
        }
        
    }

    program->push_time();
    program->push_output("...done.\n");
    
    program->root.current->map.line_segments=line_segments;
    
    program->root.current->lines_valid=1;
    program->root.calculating=FREE;
    program->calc_message_off();

    
    return NULL;
    
}

