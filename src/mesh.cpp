/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/base.hpp"

int base::gcd(int a, int b) { 
   if (b == 0) 
      return a; 
   return gcd(b, a % b);  
}

int base::pnpoly(double testx, double testy,const std::vector<vec>& vert)
{
    int nvert=vert.size();
    int i, j, c = 0;
    for (i = 0, j = nvert-1; i < nvert; j = i++) {
        if ( ((vert[i].y>=testy) != (vert[j].y>=testy)) &&
            (testx <= (vert[j].x-vert[i].x) * (testy-vert[i].y) / (vert[j].y-vert[i].y) + vert[i].x) ) // inequalities replaced with strict inequalities
            c = !c;
    }
    return c;
}


void base::areas_to_points_cb_static(Fl_Widget*,void* data){
    base* program = static_cast<base*> (data);

    if (program->root.calculating){
        program->push_time();
        program->push_output("Can't add spatial graph features while calculating!\n");
        program->calc_cb();
        return;
    }

    program->mesh_win->show();
    program->mesh_win->resize(program->win->x()+0.5*program->win->w()-250,program->win->y()+0.5*program->win->h()-255-25-55,500,545+50+15+45);

}

void base::areas_to_points_cancel_cb_static(Fl_Widget*,void* data){
    base* program = static_cast<base*> (data);
    program->mesh_win->hide();
}


void base::areas_to_points_ok_cb_static(Fl_Widget*,void* data){
    base* program = static_cast<base*> (data);
    program->areas_to_points_cb();
    program->mesh_win->hide();
}


void* base::areas_to_points_cb(){

    if (root.calculating){
        push_time();
        push_output("Can't add spatial graph features while calculating!\n");
        calc_cb();
        return NULL;
    }


    if (root.draw_route){
        Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[SHOW_ROUTE_INDEX]);   
        temp->do_callback(menu,this);
        temp->clear(); 
    }

    if (root.draw_metric){
        Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[SHOW_METRIC_INDEX]);   
        temp->do_callback(menu,this);
        temp->clear(); 
    }



    mark_locations_choice->value(0);
    mark_locations_choice->do_callback();

    root.calculating=FLOORPLAN_CONVERT;
    calc_message_on();

    draw->deselect();

    metrics_mutex.lock();
    root.current->plot_metric.clear();
    root.current->route.clear();
    root.current->max_plot_metric = 0.0;
    root.current->min_plot_metric = 0.0;
    metrics_mutex.unlock();



    std::stringstream s;
    s<<mesh_density->value();
    std::string str1;
    str1 = s.str();
    char * e;
    errno = 0;
    double density = 1.0/std::strtod(str1.c_str(), &e);

    if (*e != '\0' ||  // error, we didn't consume the entire string
        errno != 0 )   // error, overflow or underflow
    {   
        
        push_time();
        push_output("Error reading vertex spacing.\n");  
        Fl::lock();    
        root.calculating=FREE;
        Fl::unlock();
        calc_message_off();
        return NULL;  
    }


    if (density <= 0.0){
        push_time();
        push_output("Vertex spacing is less than or equal to zero. Cancelling.\n");  
        Fl::lock();    
        root.calculating=FREE;
        Fl::unlock();
        calc_message_off();
        return NULL;   
    }

    double limit = -1.0;
    int use_limit = limit_mesh_edge->value();
    
    if (use_limit){
        s.str("");
        s<<mesh_edge_max->value();
        std::string str1;
        str1 = s.str();
        char * e;
        errno = 0;
        limit = std::strtod(str1.c_str(), &e);

        if (*e != '\0' ||  // error, we didn't consume the entire string
            errno != 0 )   // error, overflow or underflow
        {   
            
            push_time();
            push_output("Error reading edge length limit.\n");  
            Fl::lock();    
            root.calculating=FREE;
            Fl::unlock();
            calc_message_off();
            return NULL;  
        }

        if (limit <= 1.0/density){
            push_time();
            push_output("Edge length limit is less than vertex spacing. Cancelling mesh generation.\n");  
            Fl::lock();    
            root.calculating=FREE;
            Fl::unlock();
            calc_message_off();
            return NULL;   
        }

    }

    int hex = lattice_choice->value();


    test_thread = std::thread(&base::areas_to_points,this,density,use_limit, limit,hex);
    test_thread.detach();

    return NULL;

}


int base::get_line_intersection_fast(double p0_x, double p0_y, double p1_x, double p1_y, 
    double p2_x, double p2_y, double p3_x, double p3_y)
{
    double s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    double det = (-s2_x * s1_y + s1_x * s2_y);
    double sign_det = (det>0) ? 1.0 : -1.0;
    det = fabs(det);

    double eps = 1e-8;

    if (fabs(det)<eps){//parallel or colinear

        //area of triangle formed from three points - zero area indicates colinearity
       return 0;

    }

    double s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) * sign_det;
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) * sign_det;

    if ((s >= 0) && (s <= det) && (t >= 0) && (t <= det))

    {

        return 1;
    }

    return 0; // No collision
}



int base::get_line_intersection(double p0_x, double p0_y, double p1_x, double p1_y, 
    double p2_x, double p2_y, double p3_x, double p3_y, double *i_x, double *i_y, double *i2_x, double *i2_y)
{
    double s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    double det = (-s2_x * s1_y + s1_x * s2_y);
    double sign_det = (det>0) ? 1.0 : -1.0;
    det = fabs(det);

    double eps = 1e-8;

    if (fabs(det)<eps){//parallel or colinear

        //area of triangle formed from three points - zero area indicates colinearity
        double triple_det = p0_x*p1_y + p1_x*p2_y + p2_x*p0_y - p0_x*p2_y - p1_x*p0_y - p2_x*p1_y;


        if (fabs(triple_det)<eps){//colinear

            if (
                ((p2_x<=p1_x)&&(p2_x>=p0_x))||
                ((p2_x<=p0_x)&&(p2_x>=p1_x))||
                ((p3_x<=p1_x)&&(p3_x>=p0_x))||
                ((p3_x<=p0_x)&&(p3_x>=p1_x))||

                ((p0_x<=p3_x)&&(p0_x>=p2_x))||
                ((p0_x<=p2_x)&&(p0_x>=p3_x))||
                ((p1_x<=p3_x)&&(p1_x>=p2_x))||
                ((p1_x<=p2_x)&&(p1_x>=p3_x))
                ){//overlap


                if (((p2_y<=p1_y)&&(p2_y>=p0_y))||
                    ((p2_y<=p0_y)&&(p2_y>=p1_y))||
                    ((p3_y<=p1_y)&&(p3_y>=p0_y))||
                    ((p3_y<=p0_y)&&(p3_y>=p1_y))||

                    ((p0_y<=p3_y)&&(p0_y>=p2_y))||
                    ((p0_y<=p2_y)&&(p0_y>=p3_y))||
                    ((p1_y<=p3_y)&&(p1_y>=p2_y))||
                    ((p1_y<=p2_y)&&(p1_y>=p3_y))
                    )
                {//overlap

                    std::vector<double> xvals = {p0_x,p1_x,p2_x,p3_x};
                    std::vector<double> yvals = {p0_y,p1_y,p2_y,p3_y};
                    std::vector<size_t> index = {0,1,2,3};
                    std::sort(index.begin(), 
                              index.end(),
                              [&xvals](size_t i1, size_t i2) {return xvals[i1] < xvals[i2];}
                              );


                    *i_x = xvals.at(index[1]);
                    *i_y = yvals.at(index[1]);
                    *i2_x = xvals.at(index[2]);
                    *i2_y = yvals.at(index[2]);

                    if (fabs(xvals.at(index[0])-xvals.at(index[3]))<eps){
                        std::sort(index.begin(), 
                                  index.end(),
                                  [&yvals](size_t i1, size_t i2) {return yvals[i1] < yvals[i2];}
                                  );


                        *i_x = xvals.at(index[1]);
                        *i_y = yvals.at(index[1]);
                        *i2_x = xvals.at(index[2]);
                        *i2_y = yvals.at(index[2]);
                    }                    

                    return 2; //colinear overlap - no overlapping "vertex"

                }

            }
            
            
            return 0;
            

        }
        else{ //parallel
            return 0;
        }

    }

    double s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) * sign_det;
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) * sign_det;

    if ((s >= 0) && (s <= det) && (t >= 0) && (t <= det))
    {
        
        // Collision detected
        if (i_x != NULL)
            *i_x = p0_x + (t * s1_x)/det;
        if (i_y != NULL)
            *i_y = p0_y + (t * s1_y)/det;

        return 1;
    }

    return 0; // No collision
}




void base::calc_links_with_walls(int id, std::vector<vertex> *output_links, int start, int end, int delete_floor, double density, int &flag, std::vector<wall> &floor_walls,int movement_vis,int sto,int stt,int sio,int sit,int use_limit,double limit){

    output_links->clear();

    container* root = &this->root;
    const std::vector<vec>& points = (root->current->points);
    const std::vector<wall>& walls_sitting = root->current->walls_sitting;
    const std::vector<wall>& walls_standing = root->current->walls_standing;


    long int nn = static_cast<int>(0.5*static_cast<double>( (end - start) * (-1+ 2 * points.size() - start - end) ) );
    long int count =0;
    int interval = 1;

    int dt_goal = 100;

    std::stringstream output;


    boost::posix_time::ptime time =boost::posix_time::microsec_clock::local_time();
    boost::posix_time::ptime time_inter =boost::posix_time::microsec_clock::local_time();


    for (int i= start; i< end;i++){

        for (unsigned int j = i+1; j<points.size();j++){            

            count++;

            if (count%interval==0){
                
                    
                boost::posix_time::ptime time_new = boost::posix_time::microsec_clock::local_time();
                boost::posix_time::time_duration dur = time_new - time;
                boost::posix_time::time_duration dur_inter = time_new - time_inter;
                time_inter = time;
                double dt = dur.total_milliseconds();
                double dt_inter = dur_inter.total_milliseconds();

                interval = std::max(1,static_cast<int>(dt_goal/std::max(1.0,dt_inter))*interval);   
            
                if (dt>dt_goal){

                    time = time_new;

                    if (root->killthread.get(id)){ // limit atomic usage by checking here
                        flag = 1;
                        stop_console_output();
                        return;
                    }

                    calc_progress.set(id, 100.0*(static_cast<double>(count)/static_cast<double>(nn)));
                    
                }
                    
                
            }

            int add =1;

            
            vertex temp;
            temp.start = i;
            temp.end   = j;

           
            //int one_step_edge_only = one_step_edge_only_button->value();


            if (!movement_vis){
                
                // dont add if in same row and more than one point away
                if ((fabs(points[i].x-points[j].x)>1.5/density)&&(fabs(points[i].y-points[j].y)<0.5/density)){
                    add=0;
                }

                // dont add if in same column and more than one point away
                if (add){
                    if ((fabs(points[i].x-points[j].x)<0.5/density)&&(fabs(points[i].y-points[j].y)>1.5/density)){
                        add=0;
                    }
                }

                if (add){
                    int a = std::round ( fabs(points[i].x-points[j].x) * density   );
                    int b = std::round ( fabs(points[i].y-points[j].y) * density   );


                    if (add){
                        if (gcd(a,b)>1){ // if the greatest common denominator > 1 then another vertex in the mesh lies on this proposed edge.
                            add=0;
                        }
                    }
                }
                

                //OR: if round difference in each dimension is not prime add=0;
            }

            if (add){
                if (use_limit){
                    double dist_sq = (points[i].x-points[j].x)*(points[i].x-points[j].x) + (points[i].y-points[j].y)*(points[i].y-points[j].y) ;
                    if (dist_sq >= limit * limit){
                        add=0;
                    }
                }
            }

            int visible = 1;

            if (add){

                // int sto = use_stand_walls_opaque->value();
                // int stt = use_stand_walls_transparent->value();
                // int sio = use_sit_walls_opaque->value();
                // int sit = use_sit_walls_transparent->value();


                for (int ii=0;ii<3;ii++){

                    const std::vector<wall> *walls = &walls_sitting;

                    if (ii==0){
                        walls = &walls_standing;
                    }
                    else if (ii==1){
                        walls = &walls_sitting;
                    }
                    else{
                        walls = &floor_walls;
                    }

                    for (unsigned k=0; k<walls->size();k++){


                        if ((*walls)[k].start.floor!=delete_floor){
                            continue;
                        }

                        wall temp = (*walls)[k];

                        if ((!sto) && (ii==0) && (temp.transparent==0)){
                            continue;
                        }

                        if ((!stt) && (ii==0) && (temp.transparent==1)){
                            continue;
                        }

                        if ((!sio) && (ii==1) && (temp.transparent==0)){
                            continue;
                        }

                        if ((!sit) && (ii==1) && (temp.transparent==1)){
                            continue;
                        }


                        if (get_line_intersection_fast(points[i].x,points[i].y,points[j].x,points[j].y,
                                     temp.start.x,temp.start.y,temp.end.x,temp.end.y)){
                            visible = 0;
                        }

                    }
           
                }
            }

            if ((add)&&(visible)){
            	try{    
			    	output_links->push_back(temp);
			    }
			    catch(...){
			    	flag = 2;
			    	return;
			    }
                
            }

            
        }
    }


    flag = 0;
    return;

}

void base::areas_to_points(double density, int use_limit, double limit,int hex) {


    std::vector<vec>& points = (root.current->points);
    std::vector<vertex>& adj_list = (root.current->adj_list);


    int floor_boundaries = detect_boundaries->value();

    double eps = 1.0/(2.0*density);
    
    const std::vector<double>& floor_index = root.current->floor_index; 
    const std::vector<std::vector<vec> >& floorarea_polygons = root.current->floorarea_polygons;
    const std::vector<int>& floorarea_type = root.current->floorarea_type;
   
    int delete_floor = floor_choice->value()-1;

    if ((delete_floor<0)&&(root.current->floor_index.size()==1)){
    	push_time();
    	push_output("Defaulting to floor 0.\n");
    	Fl::lock();
    	delete_floor = 0;
    	floor_choice->value(1);
    	floor_choice->do_callback();
    	Fl::unlock();
    }
    else if (delete_floor<0){
        
        push_time();
        push_output("Failed. Choose a floor. \n");

        Fl::lock();
        root.calculating=FREE;      
        Fl::unlock();
        calc_message_off();  
        return;
    }

    if (floorarea_polygons.size()<2){
        
        push_time();
        push_output("No floor areas available. Cancelled.\n");
        
        Fl::lock();
        root.calculating=FREE;        
        Fl::unlock();
        calc_message_off();
        
        return;
    }


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

    


    
    push_time();
    push_output("Removing spatial graph from floor...\n");
    push_time();
    push_output("...patience...\n");
    
    remove_map_from_floor(delete_floor);

    calc_message_on();
    
    push_time();
    push_output("...done.\n");


    struct wall_plus : public wall{
        int polygon;
        int type;
    };

    std::vector<wall_plus> floor_edges;

    double max_x = -9e9, max_y = -9e9;
    double min_x = 9e9, min_y = 9e9;    
    

    /* Construct list of edges of floorarea polygons, the polygon number
       and min/max bounds 
       */

    for (unsigned int i=0; i< floorarea_polygons.size();i++){
        if (floorarea_polygons[i].size()>0){ //ignore dangling polygon
            if (floorarea_polygons[i][0].floor==delete_floor){
                for (unsigned int j=0; j<floorarea_polygons[i].size();j++){

                    
                    wall_plus temp;
                    if (j>0){
                        temp.start=floorarea_polygons[i][j-1];
                        temp.end=floorarea_polygons[i][j];
                        temp.polygon=i;
                        temp.type= floorarea_type[i];
                    }
                    else{
                        temp.start=floorarea_polygons[i][0];
                        temp.end=floorarea_polygons[i][floorarea_polygons[i].size()-1];
                        temp.polygon=i;
                        temp.type= floorarea_type[i];
                    }

                    floor_edges.push_back(temp);


                    if (floorarea_polygons[i][j].x > max_x){
                        max_x = floorarea_polygons[i][j].x;
                    }
                    if (floorarea_polygons[i][j].x < min_x){
                        min_x = floorarea_polygons[i][j].x;
                    }
                    if (floorarea_polygons[i][j].y > max_y){
                        max_y = floorarea_polygons[i][j].y;
                    }
                    if (floorarea_polygons[i][j].y < min_y){
                        min_y = floorarea_polygons[i][j].y;
                    }
                }
            }
        }
    }

    // barrier walls start

    std::vector<wall> floor_walls;


    if (floor_boundaries){

        //vector for each edge of floor polygons
        std::vector<std::vector<vec> > overlap_points(floor_edges.size(),std::vector<vec>());


        push_time();
        push_output("Determining floor polygon intersections....\n");


        output_mutex.lock();
        buffer_point=text->buff->length();
        output_mutex.unlock();

        std::stringstream output;
        int nn = floor_edges.size();
        int interval = std::max(1,static_cast<int>(nn*0.001));
            
        /* for each edge list all possible decision points:
            1. start and end points
            2. all intersections
        */

        boost::posix_time::ptime time =boost::posix_time::microsec_clock::local_time();
        boost::posix_time::ptime time_inter =boost::posix_time::microsec_clock::local_time();

        for (unsigned int i=0;i<floor_edges.size();i++){


            if (root.killthread.get()){
                
                push_time();
                push_output("Cancelled.\n");

                Fl::lock();
                root.calculating=FREE;
                root.killthread.reset();           
                Fl::unlock();
                undo(0);
                redo(0);
                calc_message_off();
                return;
            }


            if (i%interval==0){
                    
                boost::posix_time::ptime time_new = boost::posix_time::microsec_clock::local_time();
                boost::posix_time::time_duration dur = time_new - time;
                boost::posix_time::time_duration dur_inter = time_new - time_inter;
                time_inter = time;
                double dt = dur.total_milliseconds();
                double dt_inter = dur_inter.total_milliseconds();

                interval = std::max(1,static_cast<int>(100/std::max(1.0,dt_inter))*interval);   
            
                if (dt>100){

                    time = time_new;
                    
                    output.str("");
                    output.precision(3);
                    output.setf( std::ios::fixed, std:: ios::floatfield );
                    output<<"------------------- >> "<<100*(static_cast<double>(i+1)/static_cast<double>(nn))<<"% completed"<<std::endl;
                    output_mutex.lock();
                    output_string = output.str();
                    output_mutex.unlock();
                    Fl::awake(&reset_console_buffer_and_push,this);
                }
                
            }

            

            if (floor_edges[i].type>0){//negative areas edges are used "as is"
                wall temp;
                temp.start.x = floor_edges[i].start.x;
                temp.start.y = floor_edges[i].start.y;
                temp.start.floor = delete_floor;
                temp.end.x = floor_edges[i].end.x;
                temp.end.y = floor_edges[i].end.y;
                temp.end.floor = delete_floor;
                floor_walls.push_back(temp);
                continue;
            }


            double xx = floor_edges[i].start.x;
            double yy = floor_edges[i].start.y;

            vec temp;
            temp.x=xx;
            temp.y=yy;


            overlap_points[i].push_back(temp);


            for (unsigned int j=0;j<floor_edges.size();j++){

                if (j==i){  //cant intersect itself
                    continue;
                }

                if (floor_edges[j].polygon==floor_edges[i].polygon){ //can't intersect same polygon
                    continue;
                }

                double x1 = floor_edges[i].start.x;
                double y1 = floor_edges[i].start.y;

                double x2 = floor_edges[i].end.x;
                double y2 = floor_edges[i].end.y;

                double x3 = floor_edges[j].start.x;
                double y3 = floor_edges[j].start.y;

                double x4 = floor_edges[j].end.x;
                double y4 = floor_edges[j].end.y;

                double a=0,b=0,a1=0,b1=0;

                int intersect = get_line_intersection(x1,y1,x2,y2,x3,y3,x4,y4,&a,&b,&a1,&b1);

                //not dealing with colinearity yet
                if (intersect==1){

                    vec temp;
                    temp.x=a;
                    temp.y=b;

                    overlap_points[i].push_back(temp); //add basic intersect point

                }
                else if (intersect==2){
                    vec temp;
                    temp.x=a;
                    temp.y=b;
                    overlap_points[i].push_back(temp); //add colinearity points
                    temp.x=a1;
                    temp.y=b1;
                    overlap_points[i].push_back(temp);
                }

            }

            xx = floor_edges[i].end.x;
            yy = floor_edges[i].end.y;

            temp.x=xx;
            temp.y=yy;

            overlap_points[i].push_back(temp);
        }

        output.str("");
        output.precision(3);
        output.setf( std::ios::fixed, std:: ios::floatfield );
        output<<"------------------- >> "<<100<<"% completed"<<std::endl;
        output_mutex.lock();
        output_string = output.str();
        output_mutex.unlock();
        Fl::awake(&reset_console_buffer_and_push,this);

        push_time();
        push_output("...done.\n");


        push_time();
        push_output("Sorting intersections....\n");

        output_mutex.lock(); 
        buffer_point=text->buff->length();
        output_mutex.unlock();

        nn = overlap_points.size();
        interval = std::max(static_cast<int>(nn*0.001),1);

        time =boost::posix_time::microsec_clock::local_time();
        time_inter =boost::posix_time::microsec_clock::local_time();

        // sort each list of decision points

        for (unsigned int i=0; i<overlap_points.size();i++){

            if (i%interval==0){
                    
                boost::posix_time::ptime time_new = boost::posix_time::microsec_clock::local_time();
                boost::posix_time::time_duration dur = time_new - time;
                boost::posix_time::time_duration dur_inter = time_new - time_inter;
                time_inter = time;
                double dt = dur.total_milliseconds();
                double dt_inter = dur_inter.total_milliseconds();

                interval = std::max(1,static_cast<int>(100/std::max(1.0,dt_inter))*interval);   
            
                if (dt>100){

                    time = time_new;

                    
                    output.str("");
                    output.precision(3);
                    output.setf( std::ios::fixed, std:: ios::floatfield );
                    output<<"------------------- >> "<<100*(static_cast<double>(i+1)/static_cast<double>(nn))<<"% completed"<<std::endl;
                    output_mutex.lock();
                    output_string = output.str();
                    output_mutex.unlock();
                    Fl::awake(&reset_console_buffer_and_push,this);
                }
                
            }
            if (overlap_points[i].size()>0){

                std::sort(overlap_points[i].begin(), //sequentual sorting will work as they fall on a line
                          overlap_points[i].end(),
                          [](const vec &i1,const vec &i2){return i1.x < i2.x; });

                std::sort(overlap_points[i].begin(),
                          overlap_points[i].end(),
                          [](const vec &i1,const vec &i2){return i1.y < i2.y; });
            }

            
        }

        output.str("");
        output.precision(3);
        output.setf( std::ios::fixed, std:: ios::floatfield );
        output<<"------------------- >> "<<100<<"% completed"<<std::endl;
        output_mutex.lock();
        output_string = output.str();
        output_mutex.unlock();
        Fl::awake(&reset_console_buffer_and_push,this);

        //// remove duplicates//////

        for (unsigned int i=0;i<overlap_points.size();i++){
            for (int j=overlap_points[i].size()-1;j>=1;j--){
                double x1 = overlap_points[i][j].x;
                double y1 = overlap_points[i][j].y;
                double x2 = overlap_points[i][j-1].x;
                double y2 = overlap_points[i][j-1].y;

                if ((equal(x1,x2))&&(equal(y1,y2))){
                    overlap_points[i].erase(overlap_points[i].begin()+j);
                }
            }
        }

        //// end remove duplaicates

        push_time();
        push_output("...done.\n");

        push_time();
        push_output("Determing floor area boundaries...\n");

        output_mutex.lock(); 
        buffer_point=text->buff->length();
        output_mutex.unlock();

        nn = overlap_points.size();
        
        interval = std::max(1,static_cast<int>(nn*0.001));

        time =boost::posix_time::microsec_clock::local_time();
        time_inter =boost::posix_time::microsec_clock::local_time();

        for (unsigned int i=0; i <overlap_points.size(); i++){ //for each polygon edge


            if (root.killthread.get()){
                
                push_time();
                push_output("Cancelled.\n");

                Fl::lock();
                root.calculating=FREE;
                calc_message_off();
                root.killthread.reset();           
                Fl::unlock();
                undo(0);
                redo(0);

                return;
            }

            if (i%interval==0){
                    
                boost::posix_time::ptime time_new = boost::posix_time::microsec_clock::local_time();
                boost::posix_time::time_duration dur = time_new - time;
                boost::posix_time::time_duration dur_inter = time_new - time_inter;
                time_inter = time;
                double dt = dur.total_milliseconds();
                double dt_inter = dur_inter.total_milliseconds();

                interval = std::max(1,static_cast<int>(100/std::max(1.0,dt_inter))*interval);   
            
                if (dt>100){

                    time = time_new;

                    
                    output.str("");
                    output.precision(3);
                    output.setf( std::ios::fixed, std:: ios::floatfield );
                    output<<"------------------- >> "<<100*(static_cast<double>(i+1)/static_cast<double>(nn))<<"% completed"<<std::endl;
                    output_mutex.lock();
                    output_string = output.str();
                    output_mutex.unlock();
                    Fl::awake(&reset_console_buffer_and_push,this);
                }
                
            }


            for (unsigned int j=1; j <overlap_points[i].size(); j++){ //every segment of the polygon edge

                wall temp;
                temp.start.x = overlap_points[i][j-1].x;
                temp.start.y = overlap_points[i][j-1].y;
                temp.start.floor = delete_floor;
                temp.end.x = overlap_points[i][j].x;
                temp.end.y = overlap_points[i][j].y;
                temp.end.floor = delete_floor;

                double xa = 0.5*(temp.start.x + temp.end.x);
                double ya = 0.5*(temp.start.y + temp.end.y);

                int in_area =0;

                for (unsigned k=0; k< floorarea_polygons.size()-1;k++){ //test if centre of segment lies in a floorarea - if so it isnt a barrier

                    if (k==i){ //cant lie in its own poygon
                        continue;
                    }

                    if (floorarea_polygons[k][0].floor!=delete_floor){ //polygons from other floor are irrelevant
                        continue;
                    }

                    if(pnpoly(xa,ya,floorarea_polygons[k])){

                        if (floor_edges[i].polygon!=static_cast<int>(k) ){
                            in_area=1;
                        }
             
                    } 
                }

                //co-linear

                for (unsigned int k=0;k<floor_edges.size();k++){

                    if (k==i){    //can't be deemed to have overlapped with its own polygon
                        continue;
                    }

                    double x1=temp.start.x;
                    double y1=temp.start.y;
                    double x2=temp.end.x;
                    double y2=temp.end.y;

                    double x3 = floor_edges[k].start.x;
                    double y3 = floor_edges[k].start.y;
                    double x4 = floor_edges[k].end.x;
                    double y4 = floor_edges[k].end.y;

                    double a,b,a1,b1;

                    int intersect = get_line_intersection(x1,y1,x2,y2,x3,y3,x4,y4,&a,&b,&a1,&b1);

                    if (intersect==2){

                        in_area=2; //co-linear

                    }

                }

                if (in_area==2){

                    double  m = (temp.end.y-temp.start.y)/(temp.end.x-temp.start.x);
                    double m1 = -1.0/m;
                    if (fabs(m)<0.0001){
                        m1 = 99999;
                    }

                    double dx = sqrt(0.0001/(1.0+m1*m1));
                    double dy = m1*dx;

                    double xx = xa+dx;
                    double yy = ya+dy;

                    int sum1=0;
                    for (unsigned k=0; k< floorarea_polygons.size();k++){
                       if(pnpoly(xx,yy,floorarea_polygons[k])){

                            sum1++;
             
                        } 
                    }

                    xx = xa-dx;
                    yy = ya-dy;

                    int sum2=0;
                    for (unsigned k=0; k< floorarea_polygons.size();k++){
                       if(pnpoly(xx,yy,floorarea_polygons[k])){

                            sum2++;
             
                        } 
                    }

                    if ((sum1==0)||(sum2==0)){
                        in_area=0;
                    }
                    


                }


                if (!in_area){
                    floor_walls.push_back(temp);
                }

            }
        }

        // barrier walls end
    }


    int start_new = points.size();
   

    std::vector<vec> new_points;
    
    try{    
    	new_points.reserve((max_x-min_x)*(max_y-min_y)/(density*density));
    }
    catch(...){
    	new_points.clear();
    	push_time();
        push_output("Out of memory!\n");
        Fl::lock();
        root.calculating=FREE;
        calc_message_off();
        root.killthread.reset();
        Fl::unlock();
        undo(0);
        redo(0);
        return;
    }

    double xpoint = min_x + eps;

    push_time();
    push_output("Generating vertices...\n");

    int layer = -1;
    if (hex){
        max_x += 0.6 * (1.0/density); 
        max_y += 0.6 * (1.0/density); 
    }
    double hex_offset = 0.5/density;
    double sin60 = sin(2.0*M_PI*60.0/360.0);

    while (xpoint < max_x){


        if (root.killthread.get()){
            
            push_time();
            push_output("Cancelled.\n");

            Fl::lock();
            root.calculating=FREE;
            calc_message_off();
            root.killthread.reset();
            Fl::unlock();
            undo(0);
            redo(0);
            return;
        }

        double ypoint = min_y + eps;

        if (hex){
            layer *= -1;
            if (layer>0){
                ypoint -= hex_offset; 
            }
        }

        while (ypoint < max_y){

            vec temp;
            temp.x = xpoint;
            temp.y = ypoint;
            temp.z = floor_index[delete_floor];
            temp.floor = delete_floor;

            new_points.push_back(temp);
            
            ypoint += 1.0/density;
            
        }

        if (hex){
            xpoint += sin60/density;
        }
        else{
            xpoint += 1.0/density;
        }
    }
    

    push_time();
    push_output("...done.\n");
    push_time();
    push_output("Paring back...\n");
    
    int counter = 0;
    for (int i=new_points.size()-1; i>=0;i--){        

        if (root.killthread.get()){
            
            push_time();
            push_output("Cancelled.\n");

            Fl::lock();
            root.calculating=FREE;
            calc_message_off();
            root.killthread.reset();
            Fl::unlock();
            undo(0);
            redo(0);
            return;
        }

        int keep = 0;

        for (unsigned int j =0; j<floorarea_polygons.size();j++){
            if (floorarea_polygons[j].size()>0){
                if (floorarea_polygons[j][0].floor == delete_floor){
                    if (floorarea_type[j]==0){
                        if(pnpoly(new_points[i].x,new_points[i].y,floorarea_polygons[j])){
                            keep=1;
                        }            
                    }
                    else{
                        if(pnpoly(new_points[i].x,new_points[i].y,floorarea_polygons[j])){
                            keep=0;
                            break;
                        }   
                    }
                    counter++;
                }
            }
        }

        if (!keep){
        	new_points[i]=new_points.back();
        	new_points.pop_back();
        }
        
    }

    
    push_time();
    push_output("...done.\n");
    push_time();
    push_output("Adding to project...\n");
    

    points_mutex.lock();

    try{    
    	points.reserve(points.size() + counter);
    }
    catch(...){
    	push_time();
        push_output("Out of memory!\n");
        Fl::lock();
        root.calculating=FREE;
        calc_message_off();
        root.killthread.reset();
        Fl::unlock();
        undo(0);
        redo(0);
        return;
    }

    // for (unsigned int i=0; i<new_points.size();i++){        
    //     points.push_back(new_points[i]);
    // }

    int num_new_points = new_points.size();

    for (int i=new_points.size()-1; i>=0;i--){        
        points.push_back(new_points[i]);
        new_points.pop_back();
    }

    new_points.clear();

    points_mutex.unlock();

    
    push_time();
    push_output("...done.\n");

    Fl::awake(redraw_from_thread,this);

    push_time();
    push_output("Determining edges...\n");
    
    unsigned int nt = std::max(static_cast<unsigned int>(MIN_THREAD),std::thread::hardware_concurrency());

    if (points.size()<=nt){
        nt=1;
    }

    std::vector<int> index_points(nt,0);

    index_points[0]=0;//start_index;
    for (unsigned int i=1;i<nt;i++){
        int old = index_points[i-1];
        int n = num_new_points;
        index_points[i] = std::max(old+1,static_cast<int>(-1.5 + n - 0.5 * sqrt(  (1+2*old-2*n)*(1+2*old-2*n)  - 4*n*(n-1)/nt  )));
        //index_points[i] =  0.5 - sqrt(nt*(1-2*index_points[i-1])*(1-2*index_points[i-1]))/(2.0*sqrt(nt));      
    }


    std::vector<std::vector<vertex> > to_add_links(nt,std::vector<vertex>(0));
    std::vector<int> flags(nt,0);
    std::vector<std::thread> threads(nt);

    Fl::lock();
    int movement_vis= movement_vis_choice->value();
    int sto = use_stand_walls_opaque->value();
    int stt = use_stand_walls_transparent->value();
    int sio = use_sit_walls_opaque->value();
    int sit = use_sit_walls_transparent->value();
    Fl::unlock();

    output_mutex.lock();
    calc_progress.reset(nt);
    root.killthread.reset(nt);
    output_mutex.unlock();

    if (calc_progress.size()<static_cast<int>(nt)) nt = calc_progress.size();
    if (root.killthread.size()<static_cast<int>(nt)) nt = root.killthread.size(); 

    Fl::awake(add_console_updater,this);
    wait_for_console();

    for (unsigned int i=0;i<nt;i++){
        int start = start_new + index_points[i];
        int end;
        if (i<nt-1){
            end = start_new + index_points[i+1];
        }
        else{
            end = points.size();
        }

        threads[i] = std::thread(&base::calc_links_with_walls,
                                 this,
                                 i,
                                 &to_add_links[i],
                                 start,
                                 end,
                                 delete_floor,
                                 density,
                                 std::ref(flags[i]),
                                 std::ref(floor_walls),
                                 movement_vis,
                                 sto,
                                 stt,
                                 sio,
                                 sit,
                                 use_limit,
                                 limit
                                 );

    }

    for (unsigned int i=0;i<nt;i++){
        threads[i].join();
    }

    stop_console_output();

    for (unsigned int i=0;i<nt;i++){
        
        if (flags[i]==1){
            
            push_time();
            push_output("Cancelled.\n");

            Fl::lock();
            root.calculating=FREE;
            calc_message_off();
            root.killthread.reset();
            Fl::unlock();
            undo(0);
            redo(0);

            return;
        }
        else if (flags[i]==2){
        	push_time();
            push_output("Out of memory!\n");

            Fl::lock();
            root.calculating=FREE;
            calc_message_off();
            root.killthread.reset();
            Fl::unlock();
            undo(0);
            redo(0);

            return;
        }
    }   

    std::stringstream output;
    output.str("");
    output.precision(3);
    output.setf( std::ios::fixed, std:: ios::floatfield );
    output<<"------------------- >> "<<100<<"% completed"<<std::endl;
    output_mutex.lock();
    output_string = output.str();
    output_mutex.unlock();
    Fl::awake(&reset_console_buffer_and_push,this);

    push_time();
    push_output("...done.\n");

    push_time();
    push_output("Adding edges to project...\n");

    

    int to_add_size=0;
    for (unsigned int i=0;i<to_add_links.size();i++){
        to_add_size+=to_add_links[i].size();
    }



    if (!root.limit_edge){
        Fl::lock();
        Fl_Menu_Item  *temp =  (Fl_Menu_Item*)(&menu->menu()[LIMIT_EDGE_INDEX]);   
        temp->do_callback(menu,this);
        temp->set();         
        Fl::unlock();
        
    }


    Fl::lock();

    line_thickness->value(0);

    Fl::unlock();


    adj_mutex.lock();

    try{
    	adj_list.reserve(adj_list.size()+to_add_size);
    }
    catch(...){
    	push_time();
        push_output("Out of memory!\n");

        Fl::lock();
        root.calculating=FREE;
        calc_message_off();
        root.killthread.reset();
        Fl::unlock();
        undo(0);
        redo(0);

        return;
    }

    // for (unsigned int i=0;i<to_add_links.size();i++){
    //     for (unsigned int j=0;j<to_add_links[i].size();j++){
    //         adj_list.push_back(to_add_links[i][j]);       
    //     }
    // }


    for (int i=to_add_links.size()-1;i>=0;i--){
        for (int j=to_add_links[i].size()-1;j>=0;j--){
            adj_list.push_back(to_add_links[i][j]);  
            to_add_links[i].pop_back();     
        }
        to_add_links.pop_back();
    }
    to_add_links.clear();

    adj_mutex.unlock();

    
    push_time();
    push_output("...done.\n");
    push_time();
    push_output("NOTE: Edge visualisation limit has been turned on.\n");
    push_time();
    push_output("NOTE: angular deviation and turn based analysis turned off.\n");
    push_time();
    push_output("NOTE: edge based analysis turned off.\n");
    push_output("------------------- >> It is STRONGLY recommended that edge based analysis is not turned back on when using mesh map functionality.\n");
    //push_output("------------------- >> Usage leads to EXTREMELY high memory demands and edges do not map to locations in a coherent way.\n");
    //push_output("------------------- >> The sole exception is when the mesh is planar such that edge length is no more than sqrt(2) * the vertex spacing.\n");


    prepare_undo();
    calc_message_off();

    Fl::lock();
    root.calculating=FREE;
    draw->show_warning_map();
    Fl::unlock();

    Fl::awake(&redraw_from_thread,this);

    return;


}