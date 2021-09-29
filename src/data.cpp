/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/base.hpp"

void base::push_data_output(std::string str){
    
    Fl::lock();
    data_text->buff->append(str.c_str());
    data_text->display->buffer(data_text->buff);
    data_text->display->redraw();
    data_text->display->insert_position(data_text->buff->line_start(data_text->buff->length()));//moves insert position to the character corresponding to the start of the line that the last character is on
    data_text->display->show_insert_position(); //scrolls to that point
    
    Fl::unlock();
    Fl::awake();
    
}

void base::clear_data_output(){
    
    Fl::lock();    
    data_text->buff->remove(0,data_text->buff->length());
    data_text->display->buffer(data_text->buff);
    data_text->display->redraw();
    
    Fl::unlock();
    Fl::awake();
    
}

void base::data_null(){
    std::stringstream s;
    s<<"No data selected."<<std::endl;
    push_data_output(s.str());
}

void base::data_points(){
    
    container* root = &(this->root);
    std::vector<vec>* points = &(root->current_data->points);
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    s<<std::setw(10);
    
    if (!points->size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_data_output(s.str());
        return;
    }
    

    if (data_format->value()<2){
        s<<"=================== Vertices ===================="<<std::endl<<std::setw(10)<<"Index"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;
        push_data_output(s.str());
    }
    if (data_format->value()!=1){
        s.str("");
        for (unsigned int i=0;i<points->size();i++){
            s<<std::setw(10)<<i<<","<<std::setw(10)<<(*points)[i].x<<","<<std::setw(10)<<(*points)[i].y<<","<<std::setw(10)<<(*points)[i].z<<","<<std::setw(10)<<(*points)[i].floor<<std::endl;
        }
        push_data_output(s.str());
    }
}
void base::data_matrix(){
    
    container* root = &(this->root);
    std::vector<vertex>* adj_list = &(root->current_data->adj_list);

    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!adj_list->size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_data_output(s.str());
        return;
    }
    
    if (data_format->value()<2){
        s<<"=================== Edges (adjacency list) ===================="<<std::endl<<std::setw(10)<<"Index"<<","<<std::setw(10)<<"Vertex 1"<<","<<std::setw(10)<<"Vertex 2"<<std::endl<<std::endl;
    
        push_data_output(s.str());
    }
    if (data_format->value()!=1){
        s.str("");
        
        for (int i=0;i<static_cast<int>(adj_list->size());i++){
            s<<std::setw(10)<<i<<","<<std::setw(10)<<(*adj_list)[i].start<<","<<std::setw(10)<<(*adj_list)[i].end<<std::endl;
        }
        
        push_data_output(s.str());
    }
}

void base::data_segments(){
    
    container* root = &(this->root);
    map_structures& map = root->current_data->map;
    std::vector<vec>& points = (root->current_data->points);
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!map.links.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_data_output(s.str());
        return;
    }
    if (data_format->value()<2){
        s<<"=================== Edges/adjacency list ===================="<<std::endl<<std::setw(10)<<"Index,"<<std::setw(10)<<"Vertex 1,"<<std::setw(10)<<"Vertex 2,"<<"Vertex 1 x,"<<std::setw(10)<<"Vertex 1 y,"<<std::setw(10)<<" Vertex  1 z,"<<std::setw(10)<<" Vertex  2 x,"<<std::setw(10)<<"Vertex 2 y,"<<std::setw(10)<<"Vertex 2 z"<<std::endl<<std::endl;
        push_data_output(s.str());
    }
    if (data_format->value()!=1){
        for (unsigned int i=0;i<map.links.size();i++){
            s.str("");
            
            int st = map.links[i].start;
            int e = map.links[i].end;
            
            s<<std::setw(10)<<i<<","<<std::setw(10)<<st<<","<<std::setw(10)<<e<<","<<std::setw(10)<<points[st].x<<","<<std::setw(10)<<points[st].y<<","<<std::setw(10)<<points[st].z<<","<<std::setw(10)<<points[e].x<<","<<std::setw(10)<<points[e].y<<","<<std::setw(10)<<points[e].z<<std::endl;
            
           
            push_data_output(s.str());
        }
    }
}


void base::data_location(int loc_index){
    

    if ((loc_index<0)||(static_cast<unsigned int>(loc_index)>= root.current_data->locations.size())){
        return;
    }

    container* root = &(this->root);
    std::vector<location>* loc = &(root->current_data->locations[loc_index]);
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    
    if (!loc->size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_data_output(s.str());
        return;
    }

    if (data_format->value()<2){

        if (loc_index == 0){
            s<<"=================== Origin location marker information ===================="<<std::endl<<std::setw(10)<<"Index"<<","<<std::setw(10)<<"Label"<<","<<std::setw(10)<<"vertex"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;
        }
        else{
            s<<"=================== Destination "<< loc_index <<" location marker information ===================="<<std::endl<<std::setw(10)<<"Index"<<","<<std::setw(10)<<"Label"<<","<<std::setw(10)<<"vertex"<<","<<std::setw(10)<<"x"<<","<<std::setw(10)<<"y"<<","<<std::setw(10)<<"z"<<","<<std::setw(10)<<"floor"<<std::endl<<std::endl;   
        }

        push_data_output(s.str());
    }
    if (data_format->value()!=1){
        for (unsigned int i=0;i<loc->size();i++){
            s.str("");
            s<<std::setw(10)<<i<<","<<std::setw(10);
            
            if ((*loc)[i].label.length()>0){
                s<<(*loc)[i].label;
            }
            else{
                s<<"NULL";
            }

            s<<","<<std::setw(10)<<(*loc)[i].point<<","<<std::setw(10)<<(*loc)[i].loc.x<<","<<std::setw(10)<<(*loc)[i].loc.y<<","<<std::setw(10)<<(*loc)[i].loc.z<<","<<std::setw(10)<<(*loc)[i].loc.floor<<std::endl;
            push_data_output(s.str());
        }
    }
        
}


void base::data_visibility(){
    
    container* root = &(this->root);
    std::vector<location>& origin = root->current_data->locations[0];

    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    
    if (!origin.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_data_output(s.str());
        return;
    }
    else if (static_cast<int>(origin[origin.size()-1].quick_vis.size())!=9){
        s<<"Data not available/not yet generated/out of date. To generate data (re-)calculate visibility by selecting an option in the \"show visibility\" selector."<<std::endl;
        push_data_output(s.str());
        return;
    }
    else if (static_cast<int>(origin[origin.size()-1].quick_vis[8].size())!=4){
        s<<"Data not available/not yet generated/out of date. To generate data (re-)calculate visibility by selecting an option in the \"show visibility\" selector."<<std::endl;
        push_data_output(s.str());
        return;
    }
    
    if (data_format->value()<2){
    
        s<<"=================== Visibility information ===================="<<std::endl<<"Remember: each marked origin is a single location with the position determining its unique visibility information."<<std::endl<<"All other locations can have multiple visibility vertices indicated by the marked points. This is achieved by having only one location per spatial graph vertex."<<std::endl<<"Key:"<<std::endl<<"    sto = standing opaque barriers"<<std::endl<<"    sta = standing all barriers"<<std::endl<<"    sio = sitting opaque barriers"<<std::endl<<"    sia = sitting all barriers"<<std::endl<<std::endl<<
        std::setw(11)<<"Origin index"<<","<<std::setw(22)<<"Origin Label"<<","<<
        std::setw(22)<<"# origins (sia)"<<","<<std::setw(22)<<"# origins (sta)"<<","<<std::setw(22)<<"# origins (sio)"<<","<<std::setw(22)<<"# origins (sto)"<<","<<
        std::setw(22)<<"# dest. 1 (sia)"<<","<<std::setw(22)<<"# dest. 1 (sta)"<<","<<std::setw(22)<<"# dest. 1 (sio)"<<","<<std::setw(22)<<"# dest. 1 (sto)"<<","<<
        std::setw(22)<<"# dest. 2 (sia)"<<","<<std::setw(22)<<"# dest. 2 (sta)"<<","<<std::setw(22)<<"# dest. 2 (sio)"<<","<<std::setw(22)<<"# dest. 2 (sto)"<<","<<
        std::setw(22)<<"# dest. 3 (sia)"<<","<<std::setw(22)<<"# dest. 3 (sta)"<<","<<std::setw(22)<<"# dest. 3 (sio)"<<","<<std::setw(22)<<"# dest. 3 (sto)"<<","<<
        std::setw(22)<<"# dest. 4 (sia)"<<","<<std::setw(22)<<"# dest. 4 (sta)"<<","<<std::setw(22)<<"# dest. 4 (sio)"<<","<<std::setw(22)<<"# dest. 4 (sto)"<<","<<
        std::setw(22)<<"# dest. 5 (sia)"<<","<<std::setw(22)<<"# dest. 5 (sta)"<<","<<std::setw(22)<<"# dest. 5 (sio)"<<","<<std::setw(22)<<"# dest. 5 (sto)"<<","<<
        std::setw(22)<<"# dest. 6 (sia)"<<","<<std::setw(22)<<"# dest. 6 (sta)"<<","<<std::setw(22)<<"# dest. 6 (sio)"<<","<<std::setw(22)<<"# dest. 6 (sto)"<<","<<
        std::setw(22)<<"# dest. 7 (sia)"<<","<<std::setw(22)<<"# dest. 7 (sta)"<<","<<std::setw(22)<<"# dest. 7 (sio)"<<","<<std::setw(22)<<"# dest. 7 (sto)"<<","<<
        std::setw(22)<<"# dest. 8 (sia)"<<","<<std::setw(22)<<"# dest. 8 (sta)"<<","<<std::setw(22)<<"# dest. 8 (sio)"<<","<<std::setw(22)<<"# dest. 8 (sto)"<<std::endl<<std::endl;

        push_data_output(s.str());
    }
    
    
    if (data_format->value()!=1){

        s.str("");

        for (unsigned i=0;i<origin.size();i++){
            
            s<<std::setw(11)<<i<<","<<std::setw(22)<<origin[i].label;
            for (unsigned j=0;j<9;j++){
                for (unsigned k=0;k<4;k++){
                    s<<","<<std::setw(22)<<origin[i].quick_vis[j][k];
                }
            }
            s<<std::endl;
            
        }
        push_data_output(s.str());
    }
        
}

void base::data_custom_metric(){
    
    container* root = &(this->root);
    
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (!root->current_data->metrics.size()){
        s<<"Data not available/not yet generated."<<std::endl;
        push_data_output(s.str());
        return;
    }
    
    if (data_format->value()<2){
    
        s<<"=================== Metric information ===================="<<std::endl<<std::endl;
        push_data_output(s.str());
        
        for (unsigned i=0;i<root->current_data->metric_des.size();i++){
        
            metric_description temp_des = root->current_data->metric_des[i];
            
        
            s.str("");
            s<<"Metric: "<<i+1<<std::endl;
            s<<"Metric basis:"<<std::endl;
            switch (temp_des.basis){
                case 0:
                    s<<"\tOrigin location based (value for each origin marker)"<<std::endl;
                    break;
                case 1:
                    s<<"\tVertex based (value for each spatial graph vertex)"<<std::endl;
                    break;
                case 2:
                    s<<"\tEdge based (value for each spatial graph edge)"<<std::endl;
                    break;
            }
            s<<"Metric measure: "<<std::endl;
            s<<"\tQuantity type: ";
            switch (temp_des.metric){
                case 0:
                    s<<"Euclidian distance (x,y), non-spatial graph distance"<<std::endl;
                    break;
                case 1:
                    s<<"Euclidian distance (x,y,z), non-spatial graph distance"<<std::endl;
                    break;
                case 2:
                    s<<"(x,y) graph distance"<<std::endl;
                    break;
                case 3:
                    s<<"(x,y,z) graph distance"<<std::endl;
                    break;
                case 4:
                    s<<"Angular distance"<<std::endl;
                    break;
                case 5:
                    s<<"Number of edges"<<std::endl;
                    break;
                case 6:
                    s<<"Number of turns"<<" (Turn definition: individual deviation of more than "<<temp_des.metric_turn<<" degrees)"<<std::endl;
                    break;
                case 7:
                    s<<"Number of destinations"<<std::endl;
                    break;
                case 8:
                    s<<"Number of shortest paths to qualifying destination(s)"<<std::endl;
                    break;
                case 9:
                    s<<"Number of shortest paths through basis between qualifying destination(s) (reverse route NOT double counted)"<<std::endl;
                    break;
                case 10:
                    s<<"Betweenness centrality (normalised to [0,1]) and generalised so as measured between qualifying destinations"<<std::endl;
                    break;
                case 11:
                    s<<"Length of subgraph, in metres, comprised of all shortest paths between all qualifying destinations"<<std::endl;
                    break;
                case 12:
                    s<<"Eigenvector centrality"<<std::endl;
                    break;
            }
            
            if (temp_des.eig_tolerance>-1){
                s<<"\tEigenvector centrality calculation accuracy tolerance: "<<temp_des.eig_tolerance<<" (defined as maximum fractional difference between minimum and maximum estimate of eigenvalue)"<<std::endl;
            }
            if (temp_des.stat>-1){
                s<<"\tStatistic of metric of qualifying destinations: ";
                switch(temp_des.stat){
                    case 0:
                        s<<"Minimum"<<std::endl;
                        break;
                    case 1:
                        s<<"Maximum"<<std::endl;
                        break;
                    case 2:
                        s<<"Mean"<<std::endl;
                        break;
                    case 3:
                        s<<"Median"<<std::endl;
                        break;
                    case 4:
                        s<<"Geometric mean"<<std::endl;
                        break;
                    case 5:
                        s<<"Harmonic mean"<<std::endl;
                        break;
                    case 6:
                        s<<"Variance"<<std::endl;
                        break;
                }
            }
            if (temp_des.inverse){
                s<<"\tInverse taken";
                
                if (temp_des.stat>-1){
                    s<<" (after statistic calculated)";
                }
                
                s<<std::endl;
            }
            if (temp_des.destination>-1){
                s<<"Destination selection:"<<std::endl;
                s<<"\tDestination type: ";
                if (temp_des.basis==2){
                    switch (temp_des.destination){
                        case 0:
                            s<<"Edges"<<std::endl;
                            break;
                        case 1:
                            s<<"Turnless lines"<<std::endl;
                            break;
                    }
                }
                else{
                    switch (temp_des.destination){
                        case 0:
                            s<<"Origins"<<std::endl;
                            break;
                        case 1:
                            s<<"Destination type 1"<<std::endl;
                            break;
                        case 2:
                            s<<"Destination type 2"<<std::endl;
                            break;
                        case 3:
                            s<<"Destination type 3"<<std::endl;
                            break;
                        case 4:
                            s<<"Destination type 4"<<std::endl;
                            break;
                        case 5:
                            s<<"Destination type 5"<<std::endl;
                            break;
                        case 6:
                            s<<"Destination type 6"<<std::endl;
                            break;
                        case 7:
                            s<<"Destination type 7"<<std::endl;
                            break;
                        case 8:
                            s<<"Destination type 8"<<std::endl;
                            break;
                        case 9:
                            s<<"Vertices"<<std::endl;
                            break;
                    }
                }
                if (temp_des.range_option>-1){
                    s<<"\tInclude destinations: ";
                                       
                    switch (temp_des.range_option){
                        case 0:
                            s<<"All"<<std::endl;
                            break;
                        case 1:
                            s<<"Minimum of selection measure"<<std::endl;
                            break;
                        case 2:
                            s<<"Maximum of selection measure"<<std::endl;
                            break;
                        case 3:
                            s<<"Less than or equal to value "<<temp_des.metric_range<<" away from desk/vertex/edge according to selection measure"<<std::endl;
                            break;
                        case 4:
                            s<<"Greater than or equal to value "<<temp_des.metric_range<<" away from desk/vertex/edge according to selection measure"<<std::endl;
                            break;
                        case 5:
                            s<<"Maximum selection measure whilst being less than or equal to value "<<temp_des.metric_range<<" away from desk/vertex/edge according to selection measure"<<std::endl;
                            break;
                        case 6:
                            s<<"Minimum selection measure whilst being greater than or equal to value "<<temp_des.metric_range<<" away from desk/vertex/edge according to selection measure"<<std::endl;
                            break;
                        case 7:
                            s<<"Equal to value "<<temp_des.metric_range<<" away from desk/vertex/edge according to selection measure"<<std::endl;
                            break;
                        case 8:
                            s<<"Lie between values "<<temp_des.metric_range<<" and "<<temp_des.metric_range2<<" away from desk/vertex/edge according to selection measure"<<std::endl;
                            break;
                        case 9:
                            s<<"Lie outside of values "<<temp_des.metric_range<<" and "<<temp_des.metric_range2<<" away from desk/vertex/edge according to selection measure"<<std::endl;
                            break;
                        case 10:
                            s<<"Maximise selection measure whilst lying between values "<<temp_des.metric_range<<" and "<<temp_des.metric_range2<<" away from desk/vertex/edge according to selection measure"<<std::endl;
                            break;
                        case 11:
                            s<<"Minimise selection measure whilst lying between values "<<temp_des.metric_range<<" and "<<temp_des.metric_range2<<" away from desk/vertex/edge according to selection measure"<<std::endl;
                            break;
                    }
                }
                
                if (temp_des.destination_crit>-1){
                    s<<"\tSelection measure: ";
                    switch (temp_des.destination_crit){
                        case 0:
                            s<<"Euclidian distance (x,y)"<<std::endl;
                            break;
                        case 1:
                            s<<"Euclidian distance (x,y,z)"<<std::endl;
                            break;
                        case 2:
                            s<<"Least (x,y) graph distance"<<std::endl;
                            break;
                        case 3:
                            s<<"Least (x,y,z) graph distance"<<std::endl;
                            break;
                        case 4:
                            s<<"Least angular distance"<<std::endl;
                            break;
                        case 5:
                            s<<"Least number of edges"<<std::endl;
                            break;
                        case 6:
                            s<<"Least number of turns"<<" (Turn definition: individual deviation of more than "<<temp_des.metric_turn<<" degrees)"<<std::endl;
                            break;
                    }
                }
                    if (temp_des.floor==1){
                        s<<"\tFloor constraint: Destinations must be on the same floor"<<std::endl;
                    }
                    else if (temp_des.floor==0){
                        s<<"\tFloor constraint: No floor constraint"<<std::endl;
                    }
                
                    if (temp_des.visible>-1){
                        s<<"\tVisibility constraint: ";
                        switch (temp_des.visible){
                            case 0:
                                s<<"No visibility constraint"<<std::endl;
                                break;
                            case 1:
                                s<<"Destinations must be visible while standing counting both transparent and opaque barriers"<<std::endl;
                                break;
                            case 2:
                                s<<"Destinations must be visible while sitting counting both transparent and opaque barriers"<<std::endl;
                                break;
                            case 3:
                                s<<"Destinations must be visible while standing counting only opaque barriers"<<std::endl;
                                break;
                            case 4:
                                s<<"Destinations must be visible while sitting counting only opaque barriers"<<std::endl;
                                break;
                                
                        }
                    }
                    if (temp_des.self_count>-1){
                        switch (temp_des.self_count){
                            case 0:
                                s<<"\tBasis element not included in destinations"<<std::endl;
                                break;
                            case 1:
                                s<<"\tBasis element included in destinations IF it matches destination type AND passes floor/selection measure/visibility constraints"<<std::endl;
                                break;
                            case 2:
                                s<<"\tBasis element added to destinations regardless of destination type or constraints"<<std::endl;
                                break;
                            
                        }
                    }
                
            }
         
            if (temp_des.route>-1){
                s<<"Route selection:"<<std::endl;
                s<<"\tRoute defined by minimising measure: ";
                switch (temp_des.route) {
                    case 0:
                        s<<"(x,y) graph distance"<<std::endl;
                        break;
                    case 1:
                        s<<"(x,y,z) graph distance"<<std::endl;
                        break;
                    case 2:
                        s<<"angular distance"<<std::endl;
                        break;
                    case 3:
                        s<<"number of edges"<<std::endl;
                        break;
                    case 4:
                        s<<"number of turns"<<" (Turn definition: individual deviation of more than "<<temp_des.metric_turn<<" degrees)"<<std::endl;
                        break;
                }
                if (temp_des.iterations>-1){
                    s<<"\tMaximum number of shortest paths considered: "<<temp_des.iterations<<std::endl;
                    // if (temp_des.error){
                    //     s<<"\tWARNING: Maximum number of attempts reached for at least one origin/destination pair"<<std::endl;
                    // }
                    // else{
                    //     s<<"\tNo reported warnings regarding missing paths"<<std::endl;
                    // }
                }
                if (temp_des.route_break>-1){
                    s<<"\tStatistic over routes: ";
                    
                    switch (temp_des.route_break){
                        case 0:
                            s<<"Minimum metric measure of paths";
                            break;
                        case 1:
                            s<<"Maximum metric measure of paths";
                            break;
                        case 2:
                            s<<"Mean of metric measures of paths";
                            break;
                    }
                    s<<" (When different paths have different final metric measure values associated with them utilise this value)"<<std::endl;
                }
            }
            
            s<<"Warnings/errors generated:"<<std::endl;
            if (temp_des.errors.is_error()){
                std::vector<std::string> error_msgs = temp_des.errors.get_error_messages();
                for (unsigned int i=0;i<error_msgs.size();i++){
                    s<<"\t"<<error_msgs[i];    
                }
                s<<std::endl;
                
            }
            else{
                s<<"\tNone."<<std::endl;
            }
            s<<std::endl<<std::endl;
            push_data_output(s.str());
        }
        
        s.str("");
        s<<std::setw(15)<<"Index"<<","<<std::setw(15)<<" label";
        for (unsigned int i=0;i<root->current_data->metrics.size();i++){
            std::stringstream ss;
            ss<<"Metric "<<i+1;
            s<<","<<std::setw(15)<<ss.str();
        }
        s<<std::endl<<std::endl;
        push_data_output(s.str());
    
    }

    
    if (data_format->value()!=1){

        unsigned max=0;
        for (unsigned int i=0;i<root->current_data->metrics.size();i++){
            
            if (root->current_data->metrics[i].size()>max){
                max=root->current_data->metrics[i].size();
            }
        }
        
        s.str("");
        for (unsigned int i=0;i<max;i++){
            
            s<<std::setw(15)<<i;
            if (i<root->current_data->locations[0].size()){
                s<<","<<std::setw(15)<<root->current_data->locations[0][i].label;
            }
            else{
                s<<","<<std::setw(15)<<"N/A";
            }
            
            for (unsigned int j=0;j<root->current_data->metrics.size();j++){
                
                if (i<root->current_data->metrics[j].size()){
                    
                    double m =root->current_data->metrics[j][i];
                    if ((m<1000)&&(m>0.1)){
                        if ((m<0)&&(m>-1.5)){
                            s<<","<<std::setw(15)<<"N/A";
                        }
                        else if (m<0){
                            s<<","<<std::setw(15)<<"inf.";   
                        }
                        else{
                            s<<","<<std::setw(15)<<std::fixed << std::setprecision(6)<<m;
                        }
                    }
                    else{
                        if ((m<0)&&(m>-1.5)){
                            s<<","<<std::setw(15)<<"N/A";
                        }
                        else if (m<0){
                            s<<","<<std::setw(15)<<"inf.";   
                        }
                        else{
                            s<<","<<std::setw(15)<<std::scientific<<std::fixed << std::setprecision(6)<<m<<std::fixed;
                        }
                    }
                }
                else{
                    s<<","<<std::setw(15)<<"N/A";
                }
                
            }
            s<<std::endl;
        }
        push_data_output(s.str());

    }

    
}

void base::data_floor_info(){
    
    container* root = &(this->root);
    std::vector<location>& origin = root->current_data->locations[0];
    std::vector<double>& floor_index = root->current_data->floor_index;
    std::vector<double>& floor_areas = root->current_data->floor_areas;
    std::vector<std::vector<vec> >& floorarea_polygons = root->current_data->floorarea_polygons;
    std::stringstream s;
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    
    s.precision(3);
    s.setf( std::ios::fixed, std:: ios::floatfield );
    if (static_cast<int>(floorarea_polygons.size())<2){
        s<<"Data not available/not yet generated."<<std::endl;
        push_data_output(s.str());
        return;
    }
    
    std::vector<int> no_origin(floor_index.size(),0);
    
    for (unsigned int i=0;i<origin.size();i++){
        if ((origin[i].loc.floor>=0)&&(origin[i].loc.floor<static_cast<int>(floor_index.size()))){
            no_origin[origin[i].loc.floor]++;
        }
    }
    
    
    if (data_format->value()<2){

        s<<"==================== Floor wide/global info ======================"<<std::endl<<std::endl<<std::setw(10)<<"Floor"<<","<<std::setw(16)<<"Area (units^2)"<<","<<std::setw(22)<<"No. origin locations"<<","<<std::setw(15)<<"Density"<<std::endl<<std::endl;
        push_data_output(s.str());

    }

    if (data_format->value()!=1){

        for (unsigned int i=0;i<no_origin.size();i++){
            if (i<floor_areas.size()){
                s.str("");
                s<<std::setw(10)<<i<<","<<std::setw(16)<<floor_areas[i]<<","<<std::setw(22)<<no_origin[i]<<","<<std::setw(15)<<static_cast<double>(no_origin[i])/floor_areas[i]<<std::endl;
            }
            push_data_output(s.str());
        }
        
        double sum=0;
        for (unsigned int i=0;i<floor_areas.size();i++){
            sum+=floor_areas[i];
        }
        
        s.str("");
        s<<std::setw(10)<<"All"<<","<<std::setw(16)<<sum<<","<<std::setw(22)<<origin.size()<<","<<std::setw(15)<<static_cast<double>(origin.size())/sum<<std::endl;
        push_data_output(s.str());
    }
}


void base::redraw_data_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->redraw_data();
}

void base::redraw_data(){
    
    clear_data_output();
    
    switch (data_choice->value()){
        case 0: data_null(); break;
        case 1: data_points();break;
        case 2: data_matrix();break;
        case 3: data_location(0); break;
        case 4: data_location(1); break;
        case 5: data_location(2); break;
        case 6: data_location(3); break;
        case 7: data_location(4); break;
        case 8: data_location(5); break;
        case 9: data_location(6); break;
        case 10: data_location(7); break;
        case 11: data_location(8); break;
        case 12: data_visibility(); break;
        case 13: data_custom_metric(); break;
        case 14: data_floor_info(); break;
    }
    
    data_text->display->insert_position(0);//moves insert position to the character corresponding to the start of the line that the last character is on
    data_text->display->show_insert_position();
}


void base::save_data_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->save_data();
}

void base::save_data(){
       
    Fl_Native_File_Chooser fnfc;
    fnfc.title("Save file");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    fnfc.options(Fl_Native_File_Chooser::SAVEAS_CONFIRM);
    if ( fnfc.show() ) return;
    
    std::string str=std::string(fnfc.filename());
    
    data_text->buff->outputfile(str.c_str(),0,data_text->buff->length());
    
    push_time();
    std::stringstream s;
    s<<"Exported to ";
    s<<str.c_str();
    s<<"\n";
    push_output(s.str().c_str());
    
}