/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/desk.hpp"
#include "./include/draw_window.hpp"
#include "./include/base.hpp"

#ifndef M_PI
    #define M_PI 3.141592653589793238
#endif

#undef min
#undef max

void draw_window::draw_locations(const std::vector<double>& desk_metric,int /*metric*/){
    
    
    int i;
    
    if ((program->show_locations[0]->value())||((program->show_visibility->value())&&(root->current->vis_valid))){
        
        glLineWidth(1.0);
        
        if (selector==1){
            i=1;
        }
        else{
            i=0;
        }
        
        if (!root->desk_model){
            
            if ((root->draw_metric)&&(metric_type==1)&&(desk_metric.size())){
              

                const double &min = program->root.current->min_plot_metric;
                const double &max = program->root.current->max_plot_metric;

                draw_locations(&current->locations[0],&desk_metric,min,max,i);
            }
            else{
                if (root->bow){
                    draw_locations(&current->locations[0],100,100,100,i);
                }
                else{
                    draw_locations(&current->locations[0],210,210,210,i);   
                }
            }
        }
        else{
            draw_desks_model(i);
        }
        
    }

    for (unsigned int loc_index =1;loc_index<9;loc_index++){
        if (program->show_locations[loc_index]->value()){
            if (selector==static_cast<int>(loc_index)+1){
                i=1;
            }
            else{
                i=0;
            }
            draw_locations(&current->locations[loc_index],locR[loc_index-1],locG[loc_index-1],locB[loc_index-1],i);
            draw_location_links(&current->locations[loc_index],locR[loc_index-1],locG[loc_index-1],locB[loc_index-1]);
        }
    }
    
}


void draw_window::draw_locations(const std::vector<location>* loc,const std::vector<double>* metric,double min,double max, int selection){ //metric version
    
    double xx,yy;
    double pixelx,pixely;
    int draw=0;
    
    
    xx= x_mouse_coord;
    yy= y_mouse_coord;                
    
    pixelx=(maxx-minx)/static_cast<double>(this->w());
    pixely=(maxy-miny)/static_cast<double>(this->h());
    
    //connect lcation to graph

    glBegin(GL_LINES);

    for (unsigned int i=0;i<loc->size();i++){
        
        
        if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
            if ((i<metric->size())){
                
                if ((*metric)[i]>=0){
                    glColor3ub(static_cast<int>(255*(((*metric)[i]-min)/(max-min))),static_cast<int>(255-(255*(((*metric)[i]-min)/(max-min)))),0);
                    if (equal(min,max)){
                        glColor3ub(127,127,0);
                    }
                }
                else{
                    if (root->bow){
                        glColor3ub(0,0,0);
                    }
                    else{
                        glColor3ub(200,200,200);
                    }
                }
            }
            else{
                if (root->bow){
                    glColor3ub(0,0,0);
                }
                else{
                    glColor3ub(200,200,200);
                }
            }
            
            if ((selection)&&(static_cast<int>(i)==selected)){ // selection highlighing
                glColor3ub(255,255,0);
            }
            
            if (program->show_map->value()){
                
                glLineWidth(std::max(1,static_cast<int>(program->line_thickness->value())));
                
                glVertex3d(current->points[(*loc)[i].point].x,current->points[(*loc)[i].point].y,current->floor_index[current->points[(*loc)[i].point].floor]);
                glVertex3d((*loc)[i].loc.x,(*loc)[i].loc.y,current->floor_index[(*loc)[i].loc.floor]);
                
                //glLineWidth(1.0);
            }
        }
    }

    glEnd();
    
    //draw location

    int metric_info_index = -1;

    glBegin(GL_TRIANGLES);

    for (unsigned int i=0;i<loc->size();i++){
        
        
        if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
            if ((i<metric->size())){
                
                if ((*metric)[i]>=0){
                    glColor3ub(static_cast<int>(255*(((*metric)[i]-min)/(max-min))),static_cast<int>(255-(255*(((*metric)[i]-min)/(max-min)))),0);
                    if (equal(min,max)){
                        glColor3ub(127,127,0);
                    }
                }
                else{
                    if (root->bow){
                        glColor3ub(0,0,0);
                    }
                    else{
                        glColor3ub(200,200,200);
                    }
                }
            }
            else{
                if (root->bow){
                    glColor3ub(0,0,0);
                }
                else{
                    glColor3ub(200,200,200);
                }
            }
            
            if ((selection)&&(static_cast<int>(i)==selected)){ // selection highlighing
                glColor3ub(255,255,0);
            }

            double size = LOCATION_SIZE_METRIC;
            drawcube_no_begin_end((*loc)[i].loc.x,(*loc)[i].loc.y,(*loc)[i].loc.z,size);            
            
            
            if ((!draw)&&(i<metric->size())&&(!program->three_d_choice->value())){
                
                if ((fabs(xx-(*loc)[i].loc.x)<0.5*size)&& (fabs(yy-(*loc)[i].loc.y)<0.5*size)){

                    metric_info_index = i;
                    draw=1;
                    
                }   
            }
        }
    }

    glEnd();

    //draw metric info window

    if (metric_info_index>-1){

        int i=metric_info_index;
        double sx,sy,sz;
        
        double lx=70*pixelx;
        double ly=40*pixely;
        
        if ((*loc)[i].loc.x<0.5*(minx+maxx)){
            sx=(*loc)[i].loc.x+30*pixelx;
        }
        else{
            sx=(*loc)[i].loc.x-30*pixelx-lx;
        }
        
        if ((*loc)[i].loc.y<0.5*(miny+maxy)){
            sy=(*loc)[i].loc.y+30*pixely;
        }
        else{
            sy=(*loc)[i].loc.y-30*pixely-ly;
        }

        sz=100;
        
        
        glColor3ub(255,255,255);
        
        glBegin(GL_TRIANGLES);
        
        glVertex3d(sx,sy,sz);
        glVertex3d(sx+lx,sy,sz);
        glVertex3d(sx,sy+ly,sz);

        glVertex3d(sx+lx,sy,sz);
        glVertex3d(sx,sy+ly,sz);
        glVertex3d(sx+lx,sy+ly,sz);
        glEnd();
        
        
        glColor3ub(0,0,0);
        glLineWidth(1.0);
        
        glBegin(GL_LINES);
        
        glVertex3d((*loc)[i].loc.x,(*loc)[i].loc.y,sz);
        glVertex3d(sx,sy,sz);
        
        glVertex3d((*loc)[i].loc.x,(*loc)[i].loc.y,sz);
        glVertex3d(sx+lx,sy,sz);
        
        glVertex3d((*loc)[i].loc.x,(*loc)[i].loc.y,sz);
        glVertex3d(sx,sy+ly,sz);
        
        glVertex3d((*loc)[i].loc.x,(*loc)[i].loc.y,sz);
        glVertex3d(sx+lx,sy+ly,sz);
        
        glVertex3d(sx,sy,sz+0.1);
        glVertex3d(sx+lx,sy,sz+0.1);

        glVertex3d(sx,sy,sz+0.1);
        glVertex3d(sx,sy+ly,sz+0.1);
        
        glVertex3d(sx+lx,sy,sz+0.1);
        glVertex3d(sx+lx,sy+ly,sz+0.1);
        
        glVertex3d(sx,sy+ly,sz+0.1);
        glVertex3d(sx+lx,sy+ly,sz+0.1);
        
        glEnd();
        
        glLineWidth(1.0);
       
        glColor3ub(0,0,0);

        std::stringstream s;
        
        
        s<<"metric:";
        
        std::string t = s.str();
        
        glPushMatrix();
        
        glTranslatef(sx+8*pixelx,sy+23*pixely,sz+0.1);//0
        glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
        
        glDisable(GL_DEPTH_TEST);
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
        
        s.str("");
        s<<std::setprecision(2)<<std::fixed;
        
        if (((*metric)[i]>1000)||((*metric)[i]<0.1)){
            s<<std::scientific;
        }
        
        if ((*metric)[i]>=0){
            s<<(*metric)[i];
        }
        else if ((*metric)[i]>-1.5){
            s<<"N/A";
        }
        else{
            s<<"inf";
        }

        t = s.str();
        
        glPushMatrix();
        
        glTranslatef(sx+8*pixelx,sy+8*pixely,sz+0.1);//0
        glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
        
        glDisable(GL_DEPTH_TEST);
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
    }
    
}

void draw_window::draw_visibility(){
    
    if ((program->show_visibility->value())&&(root->current->vis_valid)){//(program->show_visibility->value()){
        
        
        double height;
      
        if ((program->show_visibility->value()==1)||(program->show_visibility->value()==2)||(program->show_visibility->value()==5)||(program->show_visibility->value()==6)){
            height=0.8;
            
        }
        else{
            height=1.2;
            
        }
        
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(200,200,200);
        }
        glLineWidth(1.0);
        glBegin(GL_LINES);
        
        for (unsigned int i=0;i<current->locations[0].size();i++){  //draw sticks at origins
            if ((all_flrs)||(current->locations[0][i].loc.floor==flr_choice)){
                    
                glVertex3d(current->locations[0][i].loc.x,current->locations[0][i].loc.y,current->locations[0][i].loc.z);
                glVertex3d(current->locations[0][i].loc.x,current->locations[0][i].loc.y,current->locations[0][i].loc.z+height);
                
            }
                        
        }
        glEnd();
        
        std::vector<location>* loc = NULL;
        int draw=0;
        
        for (int place_loop=0;place_loop<9;place_loop++){
            
            switch (place_loop){
                case 0:loc=&current->locations[0];draw=program->show_locations[0]->value();break;
                case 1:loc=&current->locations[1];draw=program->show_locations[1]->value();break;
                case 2:loc=&current->locations[2];draw=program->show_locations[2]->value();break;
                case 3:loc=&current->locations[3];draw=program->show_locations[3]->value();break;
                case 4:loc=&current->locations[4];draw=program->show_locations[4]->value();break;
                case 5:loc=&current->locations[5];draw=program->show_locations[5]->value();break;
                case 6:loc=&current->locations[6];draw=program->show_locations[6]->value();break;
                case 7:loc=&current->locations[7];draw=program->show_locations[7]->value();break;
                case 8:loc=&current->locations[8];draw=program->show_locations[8]->value();break;
            }
            
            if (draw){   
                
                for (unsigned int i=0;i<loc->size();i++){                                      
                    
                    std::vector<int> *vis_ptr = NULL;
                    
                    if (program->show_visibility->value()==1){
                        vis_ptr=&(*loc)[i].vis_sitting;
                        height=0.8;
                    }
                    else if (program->show_visibility->value()==2){
                        vis_ptr=&(*loc)[i].vis_sitting_trans;
                        height=0.8;
                    }
                    else if (program->show_visibility->value()==3){
                        vis_ptr=&(*loc)[i].vis_standing;
                        height=1.2;
                    }
                    else if (program->show_visibility->value()==4){
                        vis_ptr=&(*loc)[i].vis_standing_trans;
                        height=1.2;
                    }
                    else{
                        break; //must not reach dereference of null pointer
                    }
                    
                    std::vector<int>& vis = *vis_ptr;

                    if (root->bow){
                        glColor3ub(0,0,0);
                    }
                    else{
                        glColor3ub(200,200,200);
                    }
                    
                    glLineWidth(1.0);
                    glBegin(GL_LINES);
                    
                        //draw sticks at locations
                    for (unsigned int ii=0;ii<loc->size();ii++){
                        if ((all_flrs)||((*loc)[ii].loc.floor==flr_choice)){
                            
                            
                            glVertex3d((*loc)[ii].loc.x,(*loc)[ii].loc.y,(*loc)[ii].loc.z);
                            glVertex3d((*loc)[ii].loc.x,(*loc)[ii].loc.y,(*loc)[ii].loc.z+height);
                            
                        }         
                        
                    }
                    glEnd();
                                     
                    
                    glColor3ub(220,100,220);
                    glLineWidth(1.0);
                    glBegin(GL_LINES);
                    
                    //display visibility lines : all or singly selected
                    // for (unsigned int j=0;j<current->locations[0].size();j++){
                       
                    //     if ((program->show_visibility->value()<5)||((program->show_visibility->value()>4)&&(selector==1)&&(selected==static_cast<int>(j)))){
                       
                    //         if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
                                
                                
                    //             if (vis.size()>j){
                    //                 if (vis[j]){
                    //                     glVertex3d((*loc)[i].loc.x,(*loc)[i].loc.y,(*loc)[i].loc.z+height);
                    //                     glVertex3d(current->locations[0][j].loc.x,current->locations[0][j].loc.y,current->locations[0][j].loc.z+height);
                    //                 }
                    //             }
                                
                    //         }
                    //     }
                    // }

                    if ( (selector==1) && ( (group_selection_list.size()) || (selected>-1) ) ){ 
                        for (unsigned int j=0;j<group_selection_list.size();j++){ //display group selected visibility
                       
                            int k = group_selection_list[j];
   
                            if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
                                
                                
                                if (static_cast<int>(vis.size())>k){
                                    if (vis[k]){
                                        glVertex3d((*loc)[i].loc.x,(*loc)[i].loc.y,(*loc)[i].loc.z+height);
                                        glVertex3d(current->locations[0][k].loc.x,current->locations[0][k].loc.y,current->locations[0][k].loc.z+height);
                                    }
                                }
                                
                            }
                        
                        } //end group selected

                        if ((selected>-1)&&(selected<static_cast<int>(current->locations[0].size()))){ // single selected


                            int k = selected;

                            if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
                                
                                
                                if (static_cast<int>(vis.size())>k){
                                    if (vis[k]){
                                        glVertex3d((*loc)[i].loc.x,(*loc)[i].loc.y,(*loc)[i].loc.z+height);
                                        glVertex3d(current->locations[0][k].loc.x,current->locations[0][k].loc.y,current->locations[0][k].loc.z+height);
                                    }
                                }
                                
                            }

                        }

                    }
                    else{ //no selection
                        for (unsigned int j=0;j<current->locations[0].size();j++){
                          
                            if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){         
                                
                                if (vis.size()>j){
                                    if (vis[j]){
                                        glVertex3d((*loc)[i].loc.x,(*loc)[i].loc.y,(*loc)[i].loc.z+height);
                                        glVertex3d(current->locations[0][j].loc.x,current->locations[0][j].loc.y,current->locations[0][j].loc.z+height);
                                    }
                                }
                                
                            }
                        
                        }
                    }

                    glEnd();
                    
                }
                
            }
            
        }
    }
}


void draw_window::draw_location_links(const std::vector<location>* loc,int c1,int c2, int c3){
    
    glColor3ub(c1,c2,c3);
    glLineWidth(program->line_thickness->value());
    glBegin(GL_LINES);
    for (unsigned int i=0;i<loc->size();i++){
        for (unsigned int j=0;j<loc->size();j++){
            
            if ((*loc)[i].point==(*loc)[j].point){
                
                if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
                    
                    glVertex3d((*loc)[i].loc.x,(*loc)[i].loc.y,current->floor_index[(*loc)[i].loc.floor]);
                    glVertex3d((*loc)[j].loc.x,(*loc)[j].loc.y,current->floor_index[(*loc)[j].loc.floor]);
                
                }
                
                
            }
            
            
        }
    }
    glEnd();
}

void draw_window::draw_locations(const std::vector<location>* loc,int c1,int c2, int c3, int selection){
    
    
    if (program->show_map->value()){
        glLineWidth(program->line_thickness->value());

        int current_selected = 0;

        glBegin(GL_LINES);

        

        for (unsigned int i=0;i<loc->size();i++){
            
            
            if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
                
                glColor3ub(c1,c2,c3);
                
                if ((selection)&&(static_cast<int>(i)==selected)){ // selection highlighing
                    glColor3ub(255,255,0);
                }
                
                if ((selection)&&(group_selection_list.size()>0)){

                    if (static_cast<int>(i) == group_selection_list[current_selected]){
                    
                        glColor3ub(255,255,0);

                        if (current_selected < static_cast<int>(group_selection_list.size())-1){
                            current_selected++;
                        }
                    
                    }
                }
                    
                glVertex3d(current->points[(*loc)[i].point].x,current->points[(*loc)[i].point].y,current->floor_index[current->points[(*loc)[i].point].floor]);
                glVertex3d((*loc)[i].loc.x,(*loc)[i].loc.y,current->floor_index[(*loc)[i].loc.floor]);
                           
            }
        }

        glEnd();

        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(50,50,50);
        }
        
        glLineWidth(1.0);
        glBegin(GL_LINES);
        
        for (unsigned int i=0;i<loc->size();i++){
            
            if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
                drawcubeedge_no_begin_end((*loc)[i].loc.x,(*loc)[i].loc.y,current->floor_index[(*loc)[i].loc.floor],LOCATION_SIZE,0); 
            }
        }


        glEnd();
    }


    int current_selected = 0;

    glColor3ub(c1,c2,c3);
    glBegin(GL_TRIANGLES);

    int highlighted = 0;

    for (unsigned int i=0;i<loc->size();i++){
        
        
        if ((all_flrs)||((*loc)[i].loc.floor==flr_choice)){
            
            
            
            if ((selection)&&(static_cast<int>(i)==selected)){ // selection highlighing
                glColor3ub(255,255,0);
                highlighted = 1;
            }

            if ((selection)&&(group_selection_list.size()>0)){

                if (static_cast<int>(i) == group_selection_list[current_selected]){
                
                    glColor3ub(255,255,0);
                    highlighted=1;

                    if (current_selected < static_cast<int>(group_selection_list.size())-1){
                        current_selected++;
                    }
                
                }
            }

            drawcube_no_begin_end((*loc)[i].loc.x,(*loc)[i].loc.y,current->floor_index[(*loc)[i].loc.floor],LOCATION_SIZE);

            if (highlighted){//((selection)&&(static_cast<int>(i)==selected)){ // selection highlighing
                glColor3ub(c1,c2,c3);
                highlighted=0;
            }
                  
            
        }
    }


    glEnd();
    
}

void draw_window::drawroute(){
    
    double size = 0.0;
    
    if (!((metric_type==3)&&(root->draw_metric))){ //if displaying points raises route to the point size
        size=0.5*program->point_size->value();
    }

    glColor3ub(255,0,0);
    glLineWidth(program->line_thickness->value());
    glBegin(GL_LINES);
    
    double dz =0.5/static_cast<double>(current->route.size());
    
    for (unsigned int i=0;i<current->route.size();i++){
        
        int ii=(current->route.size()-1);
        
        double x=static_cast<double>(i)/static_cast<double>(ii);
        
        glColor3ub(255-static_cast<int>(255*x),static_cast<int>(255*x),0);
        
        
        if (current->dual_route){
        
            int j=0;
            while ((j+1)<static_cast<int>(current->route[i].size())){
                unsigned int start = current->route[i][j];
                unsigned int end = current->route[i][j+1];
                if ((start<current->points.size())&&(end<current->points.size())){//protect
                    glVertex3d(current->points[start].x+0.01,current->points[start].y,current->points[start].z+dz*(i+1)+size);
                    glVertex3d(current->points[end].x+0.01,current->points[end].y,current->points[end].z+dz*(i+1)+size);
                }
                j+=2;
            }
        }
        else{
            int j=0;
            while (j<static_cast<int>(current->route[i].size()-1)){
                unsigned int start = current->route[i][j];
                unsigned int end = current->route[i][j+1];
                if ((start<current->points.size())&&(end<current->points.size())){//protect
                    glVertex3d(current->points[start].x+0.01,current->points[start].y,current->points[start].z+dz*(i+1)+size);
                    glVertex3d(current->points[end].x+0.01,current->points[end].y,current->points[end].z+dz*(i+1)+size);
                }
                j++;
            }
            
            
            
        }
      
    }
    glEnd();
    
    
}

void draw_window::drawaxialmap(const std::vector<double> &metric,int metric_type){
       

    int link_skip = 1;

    if ((current->adj_list.size()>MAX_DISPLAY_LINKS)&&(root->limit_edge)){
        link_skip = current->adj_list.size()/MAX_DISPLAY_LINKS;
    }
    
    double xx,yy;
    double pixelx,pixely;
    
    xx= x_mouse_coord;
    yy= y_mouse_coord;
    
    pixelx=(maxx-minx)/static_cast<double>(this->w());
    pixely=(maxy-miny)/static_cast<double>(this->h());
    
    const double &min = program->root.current->min_plot_metric;
    const double &max = program->root.current->max_plot_metric;
    
    ///BEGIN DRAWING OF ACTUAL GRAPH
    
    double size=program->point_size->value();
                    
    
    if ((metric_type==2)&&(root->draw_metric)&&(metric.size()>0)){
        drawpoints(metric);
     
    }
    else if (!((metric_type==3)&&(root->draw_metric))){ //don't plot points if displaying segment metric

        int current_selected = 0;

        glBegin(GL_TRIANGLES);
        for (unsigned int i=0;i<current->points.size();i++){  //draw the current->points as cubes
            
            glColor3ub(map_color[0],map_color[1],map_color[2]);//mapcolor
            if ((selector==12)&&(selected==static_cast<int>(i))){
                glColor3ub(255,255,0);
            }

            if ((selector==12)&&(group_selection_list.size()>0)){

                if (static_cast<int>(i) == group_selection_list[current_selected]){
                
                    glColor3ub(255,255,0);

                    if (current_selected < static_cast<int>(group_selection_list.size())-1){
                        current_selected++;
                    }
                
                }
            }

            if ((all_flrs)||((current->points[i].floor==flr_choice))){
                
                if (program->three_d_choice->value()){
                    drawcube_no_begin_end(current->points[i].x,current->points[i].y,current->points[i].z,size);
                }
                else{
                    drawcube_no_begin_end(current->points[i].x,current->points[i].y,current->points[i].z,size);
                    
                }
                
            }
        }
        glEnd();
   
    }
    
    if ((program->line_thickness->value()>0.1)||(selector==13)){
       
        int current_selected = 0;

        int index_i=-1;
        int index_j=-1;
        int index_seg = -1;

        glLineWidth(program->line_thickness->value());
        glBegin(GL_LINES);

        double min_dist = 9e99;

        for (unsigned int ii=0;ii<current->adj_list.size();ii+=link_skip){
            
            int i = current->adj_list[ii].start;
            int j = current->adj_list[ii].end;
            
                            
                if ((all_flrs)||((current->points[i].floor==flr_choice)&&(current->points[j].floor==flr_choice))){
                    
                    
                    if ((metric_type==3)&&(root->draw_metric)&&(metric.size()>0)){
                        
                        double metric_mean;
                        
                        int iii=-1;
                
                        iii=ii;
                        
                        if ((iii>=0)&&(iii<static_cast<int>(metric.size()))){
                            metric_mean=metric[iii];
                            
                            
                            if ((!program->three_d_choice->value())){
                                //start distance to segement
                                if ((i<static_cast<int>( current->points.size()) ) && (j<static_cast<int>( current->points.size()) )){//protect
                                    double x1=current->points[i].x;
                                    double x2=current->points[j].x;
                                    double y1=current->points[i].y;
                                    double y2=current->points[j].y;
                                                                            
                                    double px=x2-x1;
                                    double py=y2-y1;
                                    double s=px*px+py*py;
                                    double u=((xx-x1)*px+(yy-y1)*py)/s;
                                    if (u>1)u=1;
                                    if (u<0)u=0;
                                    double dx=x1+u*px-xx;
                                    double dy=y1+u*py-yy;
                                    double dist=sqrt(dx*dx+dy*dy);
                                                                        
                                    //end distance to segement
                                    
                                    if ((dist<0.5)&&(dist<min_dist)){
                                        
                                        index_i = i;
                                        index_j = j;
                                        index_seg = iii;
                                        min_dist = dist;
                                        
                                    }
                                }
                                                                
                            }
                                                                                                                
                            
                            glColor3ub(static_cast<int>(255*((metric_mean-min)/(max-min))),static_cast<int>(255-(255*((metric_mean-min)/(max-min)))),0);
                            
                            if (equal(min,max)){
                                glColor3ub(127,127,0);
                            }
                            
                            
                            if (metric_mean<0){
                                if (root->bow){
                                    glColor3ub(0,0,0);
                                }
                                else{
                                    glColor3ub(200,200,200);
                                }
                            }
                            
                            
                        }
                        else{
                            
                            if (root->bow){
                                glColor3ub(0,0,0);
                            }
                            else{
                                glColor3ub(200,200,200);
                            }
                        }
                             
                        
                    }
                    else if ((metric_type==2)&&(root->draw_metric)&&(metric.size()>0)){
                        
                        if (root->bow){
                            glColor3ub(0,0,0);
                        }
                        else{
                            glColor3ub(200,200,200);
                        }
                    }
                    else{
                        glColor3ub(map_color[0],map_color[1],map_color[2]);//mapcolor
                    }
                    
                    double dz = 0.0;
                    
                    
                    if ((selector==13)&&(selected==static_cast<int>(ii))){
                        glColor3ub(255,255,0);
                        dz=0.05;
                    }

                    if ((selector==13)&&(group_selection_list.size()>0)){

                        if (static_cast<int>(ii) == group_selection_list[current_selected]){
                        
                            glColor3ub(255,255,0);
                            dz=0.05;

                            if (current_selected < static_cast<int>(group_selection_list.size())-1){
                                current_selected++;
                            }
                        
                        }
                    }
                    
                    {
                        const double &x1 = current->points[i].x;
                        const double &x2 = current->points[j].x;
                        const double &y1 = current->points[i].y;
                        const double &y2 = current->points[j].y;

                        if (!(program->three_d_choice->value())){
                            if ((((x1>maxx)&&(x2>maxx))  ||  ((x1<minx)&&(x2<minx))) ||  (((y1>maxy)&&(y2>maxy))  ||  ((y1<miny)&&(y2<miny)))){
                                continue;
                            }
                        }

                    }

                    
                    if (program->three_d_choice->value()){
                        glVertex3d(current->points[i].x,current->points[i].y,current->points[i].z+dz);
                        glVertex3d(current->points[j].x,current->points[j].y,current->points[j].z+dz);
                    }
                    else{
                        glVertex3d(current->points[i].x,current->points[i].y,dz);
                        glVertex3d(current->points[j].x,current->points[j].y,dz);
                        
                    }
                    
                }  
            
        }


        if (link_skip>1){ //cant do efficient color swap if skipping links
            if (selector==13){
                glColor3ub(255,255,0);
                for (unsigned int ii=0;ii<group_selection_list.size();ii++){
                
                    int i = current->adj_list[group_selection_list[ii]].start;
                    int j = current->adj_list[group_selection_list[ii]].end;
                    glVertex3d(current->points[i].x,current->points[i].y,current->points[i].z);
                    glVertex3d(current->points[j].x,current->points[j].y,current->points[i].z);
                }
            }
        }
        
        glEnd();

        if ((index_i>-1)&&(index_j>-1)){

            int i = index_i;
            int j = index_j;
            int iii = index_seg;

            xx= x_mouse_coord;
            yy= y_mouse_coord;

            double x1=current->points[i].x;
            double x2=current->points[j].x;
            double y1=current->points[i].y;
            double y2=current->points[j].y;
            
            double px=x2-x1;
            double py=y2-y1;
            double ss=px*px+py*py;
            double u=((xx-x1)*px+(yy-y1)*py)/ss;
            if (u>1)u=1;
            if (u<0)u=0;

            double cx=x1+u*px;  
            double cy=y1+u*py;
            
            double sx,sy,sz;
            
            double lx=70*pixelx;
            double ly=40*pixely;
            
            if (current->points[i].x<0.5*(minx+maxx)){
                sx=cx+30*pixelx;
            }
            else{
                sx=cx-30*pixelx-lx;
            }
            
            if (current->points[i].y<0.5*(miny+maxy)){
                sy=cy+30*pixely;
            }
            else{
                sy=cy-30*pixely-ly;
            }
            
           
            sz=100;

            glColor3ub(240,240,240);
            
            glBegin(GL_TRIANGLES);
            
            glVertex3d(sx,sy,sz);
            glVertex3d(sx+lx,sy,sz);
            glVertex3d(sx,sy+ly,sz);
            
            glVertex3d(sx+lx,sy,sz);
            glVertex3d(sx,sy+ly,sz);
            glVertex3d(sx+lx,sy+ly,sz);
            glEnd();
            

            glColor3ub(0,0,0);
            
            glLineWidth(1.0);
            
            glBegin(GL_LINES);
            
            glVertex3d(cx,cy,sz);
            glVertex3d(sx,sy,sz);
            
            glVertex3d(cx,cy,sz);
            glVertex3d(sx+lx,sy,sz);
            
            glVertex3d(cx,cy,sz);
            glVertex3d(sx,sy+ly,sz);
            
            glVertex3d(cx,cy,sz);
            glVertex3d(sx+lx,sy+ly,sz);
            
            glVertex3d(sx,sy,sz+0.1);
            glVertex3d(sx+lx,sy,sz+0.1);
            
            glVertex3d(sx,sy,sz+0.1);
            glVertex3d(sx,sy+ly,sz+0.1);
            
            glVertex3d(sx+lx,sy,sz+0.1);
            glVertex3d(sx+lx,sy+ly,sz+0.1);
            
            glVertex3d(sx,sy+ly,sz+0.1);
            glVertex3d(sx+lx,sy+ly,sz+0.1);
            
            glEnd();                                                                        
            
            glLineWidth(1.0);
            
        
            
            glColor3ub(0,0,0);

            std::stringstream s;
            
            
            s<<"metric:";
            
            std::string t = s.str();
            
            glPushMatrix();
            
            glTranslatef(sx+8*pixelx,sy+23*pixely,sz+0.1);
            glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
            
            glDisable(GL_DEPTH_TEST);
            for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) 
            {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
            }
            glEnable(GL_DEPTH_TEST);
            glPopMatrix();
            
            s.str("");
            s<<std::setprecision(2)<<std::fixed;
            
            if ((metric[iii]>1000)||(metric[iii]<0.1)){
                s<<std::scientific;
            }


            if (metric[i]>=0){
                s<<metric[iii];
            }
            else if (metric[iii]>-1.5){
                s<<"N/A";
            }
            else{
                s<<"inf";
            }


            t = s.str();
            
            glPushMatrix();
            
            glTranslatef(sx+8*pixelx,sy+8*pixely,sz+0.1);
            glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
            
            glDisable(GL_DEPTH_TEST);
            for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) 
            {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
            }
            glEnable(GL_DEPTH_TEST);
            glPopMatrix();
            
            glColor3ub(map_color[0],map_color[1],map_color[2]);//mapcolor
        }

    }

    //draw unfinished link/edge
     if ((!program->three_d_choice->value())&&(start_link>-1)&&(end_link<0)){
        
        double xx,yy;
        xx= x_mouse_coord;
        yy= y_mouse_coord;

        if (snapon){
            xx=snapx;
            yy=snapy;
        }
        glColor3ub(255,0,0);
        glLineWidth(program->line_thickness->value());
        glBegin(GL_LINES);
        glVertex3d(current->points[start_link].x,current->points[start_link].y,0);
        glVertex3d(xx,yy,0);
        glEnd();
    }
    /// END DRAWING OF ACTUAL GRAPH
}

void draw_window::drawpoints(const std::vector<double> &metric){
    

    int point_index = -1;
    int draw=0;
    double xx,yy;
    double pixelx,pixely;
    
    
    xx= x_mouse_coord;
    yy= y_mouse_coord;
    
    pixelx=(maxx-minx)/static_cast<double>(this->w());
    pixely=(maxy-miny)/static_cast<double>(this->h());
    

    
    const double &min = program->root.current->min_plot_metric;
    const double &max = program->root.current->max_plot_metric;

    glBegin(GL_TRIANGLES);

    for (unsigned int i=0;i<current->points.size();i++){
        
        double metric_mean;
        
        if (i<metric.size()){
            metric_mean=metric[i];
            glColor3ub(static_cast<int>(255*((metric_mean-min)/(max-min))),static_cast<int>(255-(255*((metric_mean-min)/(max-min)))),0);
            
            if (equal(min,max)){
                glColor3ub(127,127,0);
            }
            if (metric_mean<0){
                if (root->bow){
                    glColor3ub(0,0,0);
                }
                else{
                    glColor3ub(200,200,200);
                }
            }
        }
        else{
            
            if (root->bow){
                glColor3ub(0,0,0);
            }
            else{
                glColor3ub(200,200,200);
            }
        }
               
        if ((selector==12)&&(static_cast<int>(i)==selected)){ // selection highlighing
            glColor3ub(255,255,0);
        }
        
        if ((all_flrs)||(current->points[i].floor==flr_choice)){
            
            if (program->three_d_choice->value()){
                drawcube_no_begin_end(current->points[i].x,current->points[i].y,current->points[i].z,program->point_size->value());
                if (root->bow){
                    glColor3ub(0,0,0);
                }
                else{
                    glColor3ub(50,50,50);
                }
                
            }
            else{
                drawcube_no_begin_end(current->points[i].x,current->points[i].y,0,program->point_size->value());
                if (root->bow){
                    glColor3ub(0,0,0);
                }
                else{
                    glColor3ub(50,50,50);
                }
                
                if ((!draw)&&(i<metric.size())){
                    
                    double size = program->point_size->value();
                    if ((fabs(xx-current->points[i].x)<0.5*size)&& (fabs(yy-current->points[i].y)<0.5*size)){
                        
                        draw=1;
                        point_index = i;
                        
                    }
                        
                }
                
            }
            
        }
        
    } 

    glEnd();

    if (point_index>-1){
        int i = point_index;
        draw=1;
        
        double sx,sy,sz;
        
        double lx=70*pixelx;
        double ly=40*pixely;
        
        if (current->points[i].x<0.5*(minx+maxx)){
            sx=current->points[i].x+30*pixelx;
        }
        else{
            sx=current->points[i].x-30*pixelx-lx;
        }
        
        if (current->points[i].y<0.5*(miny+maxy)){
            sy=current->points[i].y+30*pixely;
        }
        else{
            sy=current->points[i].y-30*pixely-ly;
        }
                                
        sz=100;
        
        glColor3ub(240,240,240);

        glBegin(GL_TRIANGLES);
        
        glVertex3d(sx,sy,sz);
        glVertex3d(sx+lx,sy,sz);
        glVertex3d(sx,sy+ly,sz);
        
        glVertex3d(sx+lx,sy,sz);
        glVertex3d(sx,sy+ly,sz);
        glVertex3d(sx+lx,sy+ly,sz);
        glEnd();
        

        glColor3ub(0,0,0);
        
        glLineWidth(1.0);
        
        glBegin(GL_LINES);
        
        glVertex3d(current->points[i].x,current->points[i].y,sz);
        glVertex3d(sx,sy,sz);
        
        glVertex3d(current->points[i].x,current->points[i].y,sz);
        glVertex3d(sx+lx,sy,sz);
        
        glVertex3d(current->points[i].x,current->points[i].y,sz);
        glVertex3d(sx,sy+ly,sz);
        
        glVertex3d(current->points[i].x,current->points[i].y,sz);
        glVertex3d(sx+lx,sy+ly,sz);
        
        glVertex3d(sx,sy,sz+0.1);
        glVertex3d(sx+lx,sy,sz+0.1);
        
        glVertex3d(sx,sy,sz+0.1);
        glVertex3d(sx,sy+ly,sz+0.1);
        
        glVertex3d(sx+lx,sy,sz+0.1);
        glVertex3d(sx+lx,sy+ly,sz+0.1);
        
        glVertex3d(sx,sy+ly,sz+0.1);
        glVertex3d(sx+lx,sy+ly,sz+0.1);
        
        glEnd();
        
        
        glLineWidth(1.0);
        
        glColor3ub(0,0,0);

        std::stringstream s;  
        
        s<<"metric:";
        
        std::string t = s.str();
        
        glPushMatrix();
        
        glTranslatef(sx+8*pixelx,sy+23*pixely,sz+0.1);
        glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
        
        glDisable(GL_DEPTH_TEST);
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) 
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
        
        s.str("");
        s<<std::setprecision(2)<<std::fixed;
        
        if ((metric[i]>1000)||(metric[i]<0.1)){
            s<<std::scientific;
        }


        if (metric[i]>=0){
            s<<metric[i];
        }
        else if (metric[i]>-1.5){
            s<<"N/A";
        }
        else{
            s<<"inf";
        }

        t = s.str();
        
        glPushMatrix();
        
        glTranslatef(sx+8*pixelx,sy+8*pixely,sz+0.1);
        glScalef(0.1*pixelx, 0.1*pixely, 0.1*pixelx);
        
        glDisable(GL_DEPTH_TEST);
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
    }
            
            
    
}

void draw_window::draw_desks_model(int selection){
    
    int current_selected = 0;

    for (unsigned int i=0;i<current->locations[0].size();i++){
               
        
        if ((all_flrs)||(current->locations[0][i].loc.floor==flr_choice)){
            
            
            glPushMatrix();
            glTranslatef(current->locations[0][i].loc.x,current->locations[0][i].loc.y,0.35+current->floor_index[current->locations[0][i].loc.floor]);
            
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            glEnable(GL_LIGHT1);
            
            int seg=-1;
            
            
            for (unsigned int ii=0;ii<current->adj_list.size();ii++){
                if (current->adj_list[ii].start==current->locations[0][i].point){
                    seg = current->adj_list[ii].end;
                    break;
                }
                if (current->adj_list[ii].end==current->locations[0][i].point){
                    seg = current->adj_list[ii].start;
                    break;
                }
            }                        
            
            double dx,dy;
            
            dx=current->points[current->locations[0][i].point].x-current->points[seg].x;
            dy=current->points[current->locations[0][i].point].y-current->points[seg].y;
            
            if (dx<0){
                dx=-dx;
                dy=-dy;
            }
            
            double angle=180;//angle2;
            angle += atan2(dy,dx)*180.0/M_PI;
                                        
            double Ax=current->points[seg].x;
            double Ay=current->points[seg].y;
            double X =current->locations[0][i].loc.x;
            double Y =current->locations[0][i].loc.y;
            
            int position = ( dx*(Y-Ay) - dy*(X-Ax) );
            
            if (position>0)angle+=180;

            glRotatef(angle,0,0,1);
            glRotatef(90,1,0,0);
            
            glColor3ub(100,100,100);
            
            GLfloat inc_r=0,inc_g=0,inc_b=0;
            if ((selection)&&(static_cast<int>(i)==selected)){
                inc_r=0.2;
                inc_g=0.2;
                inc_b=0.0;
            }

            if ((selector==1)&&(group_selection_list.size()>0)){

                if (static_cast<int>(i) == group_selection_list[current_selected]){
                
                    inc_r=0.2;
                    inc_g=0.2;
                    inc_b=0.0; 

                    if (current_selected < static_cast<int>(group_selection_list.size())-1){
                        current_selected++;
                    }
                
                }
            }

            GLfloat light_diffuse[4];
            light_diffuse[0]=0.35+inc_r;
            light_diffuse[1]=0.35+inc_g;
            light_diffuse[2]=0.35+inc_b;
            light_diffuse[3]=1.0;

            glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
            
            glBegin(GL_TRIANGLES);
            for (int ii=0;ii<2280;ii++){ //760*3
                
                glNormal3f(untitledNormals[ii*3],untitledNormals[ii*3+1],untitledNormals[ii*3+2]);
                glVertex3d(untitledVerts[ii*3],untitledVerts[ii*3+1],untitledVerts[ii*3+2]);
                
            }
            glEnd();
            

            GLfloat light_diffuse1[4];
            light_diffuse1[0]=0.68+inc_r;
            light_diffuse1[1]=0.46+inc_g;
            light_diffuse1[2]=0.31+inc_b;
            light_diffuse1[3]=1.0;

            glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
            glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse1);
            
            glBegin(GL_TRIANGLES);
            for (int ii=2283;ii<static_cast<int>(untitledNumVerts);ii++){ //761*3
                
                glNormal3f(untitledNormals[ii*3],untitledNormals[ii*3+1],untitledNormals[ii*3+2]);
                glVertex3d(untitledVerts[ii*3],untitledVerts[ii*3+1],untitledVerts[ii*3+2]);
                
            }
            glEnd();
            
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            
            glPopMatrix();
  
            
        }
        
    }
    
}