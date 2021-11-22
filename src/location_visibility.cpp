/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/base.hpp"

void base::visibility_cb_static(Fl_Widget*, void* data) {
    base* program = static_cast<base*> (data);
    program->visibility_cb();
}

void base::visibility_cb(){
    
    THREAD_STATE& calculating = root.calculating;   
    
    if (calculating == FREE){
        calculating=CALC_VIS;
        calc_message_on();

        test_thread = std::thread(&base::visibility_calc1_static,this);
        test_thread.detach();
        
    }
    else{
        calc_cb();
    }
    
}

void base::vis_calc_for_desks(int id, 
                              int j, 
                              std::vector<int> *vis_array, 
                              std::vector<wall> *w, 
                              std::vector<location> &origins,
                              std::vector<location> *loc,
                              int no_walls,
                              long int &count,
                              long int count_tot,
                              int start, 
                              int end, 
                              int ,  //start_buff
                              int &flag)
{

    std::stringstream output;
    int interval = 1;

    boost::posix_time::ptime time =boost::posix_time::microsec_clock::local_time();
    boost::posix_time::ptime time_inter =boost::posix_time::microsec_clock::local_time();

    for (int i=start;i<end;i++){            
                
        if ((*vis_array)[i]==1){
            
            if (origins[i].loc.floor==(*loc)[j].loc.floor){
                
                
                double x1,x2,y1,y2,x_1,x_2,y_1,y_2;
                
                x1=origins[i].loc.x;
                y1=origins[i].loc.y;
                x2=(*loc)[j].loc.x;
                y2=(*loc)[j].loc.y;                                                        
                
                int visible=1;
                
                for (unsigned int ii=0;ii<w->size();ii++){
                    
                    if ((*w)[ii].start.floor==origins[i].loc.floor){
                        
                        x_1=(*w)[ii].start.x;
                        x_2=(*w)[ii].end.x;
                        y_1=(*w)[ii].start.y;
                        y_2=(*w)[ii].end.y;

                        double a,b,a1,b1;
                        if (get_line_intersection(x1,y1,x2,y2,
                                     x_1,y_1,x_2,y_2,&a,&b,&a1,&b1)){
                            visible = 0;
                        }
                        
                    }
                }
                locations_mutex.lock();
                (*vis_array)[i]=visible;
                locations_mutex.unlock();
            }
            else{
                locations_mutex.lock();
                (*vis_array)[i]=0;
                locations_mutex.unlock();
            }
            
        }
        
        if (!id){

            if (no_walls){                
                count++;
            }
            else{
                count=count+w->size();
            }
        }


        if ((count%interval==0)||(i==start)){
                
            boost::posix_time::ptime time_new = boost::posix_time::microsec_clock::local_time();
            boost::posix_time::time_duration dur = time_new - time;
            boost::posix_time::time_duration dur_inter = time_new - time_inter;
            time_inter = time;
            double dt = dur.total_milliseconds();
            double dt_inter = dur_inter.total_milliseconds();

            
            interval = std::max(1,static_cast<int>(100/std::max(1.0,dt_inter))*interval);   
    

            if ((dt>100)||(i==start)){

                time = time_new;

                if (root.killthread.get(id)){ //only check this threads atomic varible
                    flag=1;
                    stop_console_output();
                    return;
                } 

                calc_progress.set(id, 100.0*(static_cast<double>(count)/static_cast<double>(count_tot)));
            }
            
        }
     
    }
    
}

void* base::visibility_calc1_static(void* data){
    base* program = static_cast<base*> (data);
    program->visibility_calc1();

    return NULL;
}

void* base::visibility_calc1(){
        
    container* root = &(this->root); //shadowing "container root"
    
    std::vector<location>& loc0 = root->current->locations[0];
    std::vector<location>& loc1 = root->current->locations[1];
    std::vector<location>& loc2 = root->current->locations[2];
    std::vector<location>& loc3 = root->current->locations[3];
    std::vector<location>& loc4 = root->current->locations[4];
    std::vector<location>& loc5 = root->current->locations[5];
    std::vector<location>& loc6 = root->current->locations[6];
    std::vector<location>& loc7 = root->current->locations[7];
    std::vector<location>& loc8 = root->current->locations[8];

    
    std::vector<wall>& walls_sitting = root->current->walls_sitting;
    std::vector<wall>& walls_standing = root->current->walls_standing;
    THREAD_STATE& calculating = root->calculating;
    
    std::stringstream output;
    
    if (static_cast<int>(loc0.size())==0){
        
        
        output<<"No origin locations to calculate visibility for! Add origin locations or disable calculation of visibility constructs to proceed."<<std::endl;
        
        push_time();
        push_output(output.str());
        Fl::lock();
        calculating=FREE;
        Fl::unlock();
        Fl::awake(calc_message_off_cb,this);
     
     
        return NULL;
    }
    
    output<<"Starting visibility calculation for "<<loc0.size()<<" origin locations..."<<std::endl;
    
    push_time();
    push_output(output.str());
    
    std::vector<wall> w_sit_o,w_sit_t,w_sitstand_t,w_stand_o,w_stand_t;

    for (unsigned int i=0;i<walls_standing.size();i++){
        
        wall temp = walls_standing[i];

        // begin floating point nonsense
        if (fabs(temp.start.x-temp.end.x)<1e-11){
            temp.start.x+=1e-10;

        }

        if (fabs(temp.start.y-temp.end.y)<1e-11){
            temp.start.y+=1e-10;
        }

        //end floating point nonsense

        if (walls_standing[i].transparent){
            w_stand_t.push_back(temp);
        }
        else{
            w_stand_o.push_back(temp);
        }
    }
    
    
    w_sitstand_t=w_stand_t;
    
    for (unsigned int i=0;i<walls_sitting.size();i++){
        
        wall temp = walls_sitting[i];

        // begin floating point nonsense
        if (fabs(temp.start.x-temp.end.x)<1e-11){
            temp.start.x+=1e-10;

        }

        if (fabs(temp.start.y-temp.end.y)<1e-11){
            temp.start.y+=1e-10;
        }

        //end floating point nonsense


        if (walls_sitting[i].transparent){
            w_sitstand_t.push_back(temp);
            w_sit_t.push_back(temp);
        }
        else{
            w_sit_o.push_back(temp);
        }
    }
    
    long int count=0;
    long int count_tot=(w_stand_o.size()+w_sit_o.size()+w_stand_t.size()+w_sitstand_t.size())*loc0.size()*(loc0.size()+loc1.size()+loc2.size()+loc3.size()+loc4.size()+loc5.size()+loc6.size()+loc7.size()+loc8.size());
    
    
    int no_walls=0;
    if (count_tot==0){
        count_tot = 4*loc0.size()*(loc0.size()+loc1.size()+loc2.size()+loc3.size()+loc4.size()+loc5.size()+loc6.size()+loc7.size()+loc8.size());
        no_walls=1;
    }
    
    unsigned int nt = std::max(static_cast<unsigned int>(MIN_THREAD),std::thread::hardware_concurrency());


    if (loc0.size()<nt){
        nt=loc0.size();
    }

    count_tot/=nt;    
    count_tot=std::max(static_cast<long int>(1),count_tot);

    // output_mutex.lock();
    // buffer_point=text->buff->length();
    // output_mutex.unlock();

    output_mutex.lock();
    calc_progress.reset(nt);
    root->killthread.reset(nt);
    output_mutex.unlock();

    if (calc_progress.size()<static_cast<int>(nt)) nt = calc_progress.size();
    if (root->killthread.size()<static_cast<int>(nt)) nt = root->killthread.size(); 
    
    Fl::awake(add_console_updater,this);
    wait_for_console();
    
    std::vector<wall>* w = NULL;
    

    for (int choice_v=0;choice_v<4;choice_v++){
        
        
        for (int place_loop=0;place_loop<9;place_loop++){
            
            std::vector<location>* loc = NULL;
            
            switch (place_loop){
                case 0:loc=&loc0;break;
                case 1:loc=&loc1;break;
                case 2:loc=&loc2;break;
                case 3:loc=&loc3;break;
                case 4:loc=&loc4;break;
                case 5:loc=&loc5;break;
                case 6:loc=&loc6;break;
                case 7:loc=&loc7;break;
                case 8:loc=&loc8;break;
            }
            
            
            for (unsigned int j=0;j<loc->size();j++){                                
                
                std::vector<int>* vis_array = NULL;
                
                locations_mutex.lock();

                switch(choice_v){
                    case 0:
                        vis_array=&((*loc)[j].vis_standing_trans);
                        vis_array->clear();
                        std::vector<int>().swap(*vis_array);
                        {
                            std::vector<int> temp(loc0.size(),1);
                            (*vis_array)=temp;
                        }
                        w=&w_stand_o;
                        break;
                    case 1:
                        vis_array=&((*loc)[j].vis_sitting_trans);
                        (*vis_array)=(*loc)[j].vis_standing_trans;
                        w=&w_sit_o;
                        break;
                    case 2:
                        vis_array=&((*loc)[j].vis_standing);
                        (*vis_array)=(*loc)[j].vis_standing_trans;
                        w=&w_stand_t;
                        break;
                    case 3:
                        vis_array=&((*loc)[j].vis_sitting);
                        (*vis_array)=(*loc)[j].vis_sitting_trans;
                        w=&w_sitstand_t;
                        break;
                }
                
                locations_mutex.unlock();

                std::vector<int> index_points(nt,0);                
                index_points[0]=0;
                for (unsigned int i=1;i<nt;i++){
                    
                    index_points[i]=index_points[i-1] + std::max(1,static_cast<int>(loc0.size())/static_cast<int>(nt));
                }
                
                std::vector<int> flags(nt,0);
                std::vector<std::thread> threads(nt);


                //locations_mutex.lock(); //this is ok as the only locks inside are output_mutex which is the correct lock/unlock order -everything else is atomic and/or lock free versions

                for (unsigned int i=0;i<nt;i++){
                    int start = index_points[i];
                    int end;
                    if (i<nt-1){
                        end = index_points[i+1];
                    }
                    else{
                        end = loc0.size();
                    }
                    threads[i] = std::thread(&base::vis_calc_for_desks,
                                             this,
                                             i,
                                             j,
                                             vis_array,
                                             w,
                                             std::ref(loc0),
                                             loc,
                                             no_walls,
                                             std::ref(count),
                                             count_tot,
                                             start,
                                             end,
                                             buffer_point,                                            
                                             std::ref(flags[i]));

                }

                for (unsigned int i=0;i<nt;i++){
                    threads[i].join();
                }

                //locations_mutex.unlock();



                for (unsigned int i=0;i<nt;i++){

                    if (flags[i]){

                        stop_console_output();
                        std::stringstream output;
                        output<<"...cancelled."<<std::endl;
                        push_time();
                        push_output(output.str());

                        Fl::lock();
                        root->current->vis_valid=0;
                        root->killthread.reset();
                        calculating=FREE;
                        Fl::unlock();
                        Fl::awake(calc_message_off_cb,this);
                        
                        return NULL;
                    }

                }
                
            }
                
        }
        
    }
    stop_console_output();              
    
    //QUICK VIS
    
    locations_mutex.lock();
    
    for (unsigned int i=0;i<loc0.size();i++){
        
        loc0[i].quick_vis.clear();
        
        for (unsigned int k=0;k<9;k++){
            
            std::vector<int> temp;
            
            loc0[i].quick_vis.push_back(temp);
            
            std::vector<location>* loc = NULL;
            
            switch(k){
                case 0:
                    loc = &loc0;
                    break;
                case 1:
                    loc = &loc1;
                    break;
                case 2:
                    loc = &loc2;
                    break;
                case 3:
                    loc = &loc3;
                    break;
                case 4:
                    loc = &loc4;
                    break;
                case 5:
                    loc = &loc5;
                    break;
                case 6:
                    loc = &loc6;
                    break;
                case 7:
                    loc = &loc7;
                    break;
                case 8:
                    loc = &loc8;
                    break;
                    
            }
            
            std::vector<int> vis_found;
            
            for (unsigned j=0;j<loc->size();j++){
                
                if ((k>0)||(i!=j)){
                    
                    if ((*loc)[j].vis_sitting[i]){
                        
                        int found=0;
                        
                        for (unsigned jj=0;jj<vis_found.size();jj++){
                            
                            if (vis_found[jj]==(*loc)[j].point){
                                found=1;
                                break;
                            }
                            
                        }
                        
                        if (k==0){
                            found=0;
                        }
                        
                        if (!found){
                            vis_found.push_back((*loc)[j].point);
                        }
                        
                    }
                }
                
            }
            
            loc0[i].quick_vis[k].push_back(vis_found.size());
            
            
            vis_found.clear();
            
            for (unsigned j=0;j<loc->size();j++){
                
                if ((k>0)||(i!=j)){
                    
                    if ((*loc)[j].vis_standing[i]){
                        
                        int found=0;
                        
                        for (unsigned jj=0;jj<vis_found.size();jj++){
                            
                            if (vis_found[jj]==(*loc)[j].point){
                                found=1;
                                break;
                            }
                            
                        }
                        
                        if (k==0){
                            found=0;
                        }
                        
                        if (!found){
                            vis_found.push_back((*loc)[j].point);
                        }
                        
                    }
                    
                }
                
            }
            
            loc0[i].quick_vis[k].push_back(vis_found.size());
            
            vis_found.clear();
            
            for (unsigned j=0;j<loc->size();j++){
                
                if ((k>0)||(i!=j)){
                    
                    if ((*loc)[j].vis_sitting_trans[i]){
                        
                        int found=0;
                        
                        for (unsigned jj=0;jj<vis_found.size();jj++){
                            
                            if (vis_found[jj]==(*loc)[j].point){
                                found=1;
                                break;
                            }
                            
                        }
                        
                        if (k==0){
                            found=0;
                        }
                        
                        if (!found){
                            vis_found.push_back((*loc)[j].point);
                        }
                        
                    }
                    
                }
                
            }
            
            loc0[i].quick_vis[k].push_back(vis_found.size());
            
            vis_found.clear();
            
            for (unsigned j=0;j<loc->size();j++){
                
                if ((k>0)||(i!=j)){                                       
                    
                    if ((*loc)[j].vis_standing_trans[i]){
                        
                        int found=0;
                        
                        for (unsigned jj=0;jj<vis_found.size();jj++){
                            
                            if (vis_found[jj]==(*loc)[j].point){
                                found=1;
                                break;
                            }
                            
                        }
                        
                        if (k==0){
                            found=0;
                        }
                        
                        if (!found){
                            vis_found.push_back((*loc)[j].point);
                        }
                        
                    }
                    
                }
                
            }
            
            loc0[i].quick_vis[k].push_back(vis_found.size());
            
        }
        
        
    }
    
    
    locations_mutex.unlock();


    
    output.str("");
    output.precision(3);
    output.setf( std::ios::fixed, std:: ios::floatfield );
    output<<"------------------- >> "<<100<<"% calculated"<<std::endl;
    output_mutex.lock();
    output_string = output.str();
    output_mutex.unlock();
    Fl::awake(&reset_console_buffer_and_push,this);

    
    output.str("");
    output<<"...done."<<std::endl;
    push_time();
    push_output(output.str());
    
    Fl::lock();

    root->current->vis_valid=1;
    calculating=FREE;
    Fl::unlock();

    Fl::awake(calc_message_off_cb,this);
    Fl::awake(awake_vis_warning_off,this);
    
    Fl::awake(&check_from_thread,this);
    Fl::awake(&redraw_from_thread,this);

    return NULL;   
    
}


int base::visible_test(double x1,double x2,double y1,double y2,double x_1,double x_2,double y_1,double y_2){
    
    
    if (fabs(x2-x1)<0.0000001){x2+=0.001;x1-=0.001;}
    if (fabs(x_2-x_1)<0.0000001){x_2+=0.001;x_1-=0.001;}
    
    int visible=1;
    double m1,m2,c1,c2;
    
    m1=(y2-y1)/(x2-x1);
    c1=y1-(m1*x1);
    
    if ((std::isnan(m1))||(std::isinf(m1))){
        c1=x1;
    }
    if (m1==0){
        c1=y1;
    }
    
    m2=(y_2-y_1)/(x_2-x_1);
    c2=y_1-(m2*x_1);
    
    if ((std::isnan(m2))||(std::isinf(m2))){
        c2=x_1;
    }
    if (m2==0){
        c2=y_1;
    }
    
    if (fabs(y2-y1)<0.0000001){m1=0;y2+=0.01;y1-=0.01;}
    if (fabs(y_2-y_1)<0.0000001){m2=0;y_2+=0.01;y_1-=0.01;}
    
    double x=0,y=0;
    
    if (((std::isnan(m2))||(std::isinf(m2)))&&((std::isnan(m1))||(std::isinf(m1)))){
        if ((c2==c1)&&    (  ((y1>=std::min(y_1,y_2))&&(y1<=std::max(y_1,y_2))) || ((y2>=std::min(y_1,y_2))&&(y2<=std::max(y_1,y_2)))  )  ){
            visible=0;
        }
    }
    else if ((m2==0)&&(m1==0)){
        if ((c2==c1)&&    (  ((x1>=std::min(x_1,x_2))&&(x1<=std::max(x_1,x_2))) || ((x2>=std::min(x_1,x_2))&&(x2<=std::max(x_1,x_2)))  )  ){
            visible=0;
        }
    }
    else if ((std::isnan(m1))||(std::isinf(m1))){
        x=c1;
        y=m2*c1+c2;
    }
    else if ((std::isnan(m2))||(std::isinf(m2))){
        x=c2;
        y=m1*c2+c1;
    }
    else if (m1==0){
        y=c1;
        x=(c1-c2)/m2;
    }
    else if (m2==0){
        y=c2;
        x=(c2-c1)/m1;
        
    }
    else{
        x=(c1-c2)/(m2-m1);
        y=m1*((c1-c2)/(m2-m1))+c1;
    }
    
    if ((!std::isnan(m1))&&(!std::isnan(m2))){
        
        if ((y>=std::min(y1,y2))&&(y<=std::max(y1,y2))&&(x>=std::min(x1,x2))&&(x<=std::max(x1,x2))&&(y>=std::min(y_1,y_2))&&(y<=std::max(y_1,y_2))&&(x>=std::min(x_1,x_2))&&(x<=std::max(x_1,x_2))){
            visible=0;
        }
        
    }
    
    return visible;
    
}