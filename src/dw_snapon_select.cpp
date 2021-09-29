/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/draw_window.hpp"
#include "./include/base.hpp"

#ifndef M_PI
    #define M_PI 3.141592653589793238
#endif

#undef min
#undef max

void draw_window::draw_group_select(double fore_z){

    double x_mouse_coord2 = (static_cast<double>(xdown)  /  static_cast<double> (this->w()) )*(maxx-minx)+minx;
    double y_mouse_coord2 = (static_cast<double>(ydown)  /  static_cast<double> (this->h()) )*(miny-maxy)+maxy;


    if (root->bow){
        glColor4f((255.0/256.0),(255.0/256.0),(0.0/256.0),0.2);
    }
    else{
        glColor4f((255.0/256.0),(255.0/256.0),(0.0/256.0),0.15);
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_BLEND );
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    glBegin(GL_TRIANGLES);

    glVertex3d(x_mouse_coord,y_mouse_coord,fore_z);
    glVertex3d(x_mouse_coord,y_mouse_coord2,fore_z);
    glVertex3d(x_mouse_coord2,y_mouse_coord2,fore_z);

    glVertex3d(x_mouse_coord,y_mouse_coord,fore_z);
    glVertex3d(x_mouse_coord2,y_mouse_coord,fore_z);
    glVertex3d(x_mouse_coord2,y_mouse_coord2,fore_z);

    glEnd();

    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
}

void draw_window::draw_snapon(double fore_z){
    
    if (snapon){
        
        double dx=7.0*(maxx-minx)/static_cast<double>(this->w());     

        glColor3ub(255,0,0);
        glLineWidth(1.0);
        glBegin(GL_LINES);
        glVertex3d(snapx-dx,snapy,fore_z);
        glVertex3d(snapx+dx,snapy,fore_z);
        glVertex3d(snapx,snapy-dx,fore_z);
        glVertex3d(snapx,snapy+dx,fore_z);
        glEnd();
        circle temp;
        temp.x=snapx;
        temp.y=snapy;
        temp.r=dx;
        temp.color=1;//red
        
        glBegin(GL_LINES);
        DrawCircle(temp,50,0.0,0.0,-1);        
        glEnd();
        
        if ((mark_locations_choice->value()==12)&&(snap_point>-1)&&(snap_point<static_cast<int>(current->points.size()))){  //snap line when drawing current->points
                        
            if (root->bow){
                glColor3ub(0,0,0);
            }
            else{
                glColor3ub(200,200,200);
            }
            glBegin(GL_LINES);
            glVertex3d(current->points[snap_point].x,current->points[snap_point].y,current->points[snap_point].z);
            glVertex3d(snapx,snapy,0);
            //glEnd();
            
            double pixels=35*(maxx-minx)/static_cast<double>(this->w());

            glColor3ub(255,0,0);
            //glBegin(GL_LINES);
            glVertex3d(current->points[snap_point].x,current->points[snap_point].y,current->points[snap_point].z);
            glVertex3d(current->points[snap_point].x+pixels*cos(ang_snap),current->points[snap_point].y+pixels*sin(ang_snap),0);

            glVertex3d(current->points[snap_point].x,current->points[snap_point].y,current->points[snap_point].z);
            glVertex3d(current->points[snap_point].x+pixels*cos(ang_snap+0.5*M_PI),current->points[snap_point].y+pixels*sin(ang_snap+0.5*M_PI),0);
            glEnd();
            
        }
        else if((mark_locations_choice->value()==1)&&(snap_point>-1)&&(snap_point<static_cast<int>(current->points.size()))){
            
            if (root->bow){
                glColor3ub(0,0,0);
            }
            else{
                glColor3ub(200,200,200);
            }
            glBegin(GL_LINES);
            glVertex3d(current->locations[0][snap_point].loc.x,current->locations[0][snap_point].loc.y,current->locations[0][snap_point].loc.z);
            glVertex3d(snapx,snapy,0);
            //glEnd();
            
            double pixels=35*(maxx-minx)/static_cast<double>(this->w());
            
            glColor3ub(255,0,0);
            //glBegin(GL_LINES);
            glVertex3d(current->locations[0][snap_point].loc.x,current->locations[0][snap_point].loc.y,current->locations[0][snap_point].loc.z);
            glVertex3d(current->locations[0][snap_point].loc.x+pixels*cos(ang_snap),current->locations[0][snap_point].loc.y+pixels*sin(ang_snap),0);
            
            glVertex3d(current->locations[0][snap_point].loc.x,current->locations[0][snap_point].loc.y,current->locations[0][snap_point].loc.z);
            glVertex3d(current->locations[0][snap_point].loc.x+pixels*cos(ang_snap+0.5*M_PI),current->locations[0][snap_point].loc.y+pixels*sin(ang_snap+0.5*M_PI),0);
            glEnd();
            
        }
        else if((mark_locations_choice->value()==14)&&(snap_point>-1)&&(snap_point<static_cast<int>(current->floorarea_polygons.size()))){
            
            if (snap_point_plus<static_cast<double>(current->floorarea_polygons[snap_point].size())){

                if (alt_on){

                    double xi = current->floorarea_polygons[snap_point][snap_point_plus].x;
                    double yi = current->floorarea_polygons[snap_point][snap_point_plus].y;
                    double zi = current->floorarea_polygons[snap_point][snap_point_plus].z;

                    if (root->bow){
                        glColor3ub(0,0,0);
                    }
                    else{
                        glColor3ub(200,200,200);
                    }
                    glBegin(GL_LINES);
                    glVertex3d(xi,yi,zi);
                    glVertex3d(snapx,snapy,0);
                    //glEnd();
                    
                    double pixels=35*(maxx-minx)/static_cast<double>(this->w());    

                    glColor3ub(255,0,0);
                    //glBegin(GL_LINES);
                    glVertex3d(xi,yi,zi);
                    glVertex3d(xi+pixels*cos(ang_snap),yi+pixels*sin(ang_snap),0);
                    
                    glVertex3d(xi,yi,zi);
                    glVertex3d(xi+pixels*cos(ang_snap+0.5*M_PI),yi+pixels*sin(ang_snap+0.5*M_PI),0);
                    glEnd();
                }

            }
            
        }
        else if(((mark_locations_choice->value()==11)||(mark_locations_choice->value()==10)) &&(snap_point>-1)){

            std::vector<wall> *wall_ptr = NULL;

            if (snap_wall){
                wall_ptr = &current->walls_standing;
            }
            else{
                wall_ptr = &current->walls_sitting;   
            }

            std::vector<wall> &w = *wall_ptr;

            if (snap_point <= static_cast<int> (w.size())){
        
            
                if (alt_on){
                
                    double xi=0,yi=0,zi=0;


                    if (snap_point < static_cast<int>(w.size())){

                        if (snap_point_plus==1){
                            xi=w[snap_point].end.x;
                            yi=w[snap_point].end.y;        
                        }
                        else if (snap_point_plus==0){
                            xi=w[snap_point].start.x;
                            yi=w[snap_point].start.y;           
                        }
                    }
                    else if (snap_point == static_cast<int>(w.size())){
                        xi=temp_wall.start.x;        
                        yi=temp_wall.start.y;        
                    }
                    else{
                        return;
                    }

                    if (root->bow){
                        glColor3ub(0,0,0);
                    }
                    else{
                        glColor3ub(200,200,200);
                    }
                    glBegin(GL_LINES);
                    glVertex3d(xi,yi,zi);
                    glVertex3d(snapx,snapy,0);
                    //glEnd();
                    
                    double pixels=35*(maxx-minx)/static_cast<double>(this->w());

                    glColor3ub(255,0,0);
                    //glBegin(GL_LINES);
                    glVertex3d(xi,yi,zi);
                    glVertex3d(xi+pixels*cos(ang_snap),yi+pixels*sin(ang_snap),0);
                    
                    glVertex3d(xi,yi,zi);
                    glVertex3d(xi+pixels*cos(ang_snap+0.5*M_PI),yi+pixels*sin(ang_snap+0.5*M_PI),0);
                    glEnd();

                }
            }
         
        }
        
        
        if (
            (snap_point>-1)
            &&
            ( 
                ((mark_locations_choice->value()==12)  && (snap_point<static_cast<int>(current->points.size())))
                || 
                ((mark_locations_choice->value()==1) && (snap_point<static_cast<int>(current->locations[0].size())))
                || 
                ((mark_locations_choice->value()==11) && (snap_point<=static_cast<int>(current->walls_standing.size())))  
            )   
           )
        {
            
            if (mark_locations_choice->value()==14){
                if ((snap_point_plus>static_cast<int>(current->floorarea_polygons[snap_point].size()))||(snap_point_plus<0)){
                    return;
                }
            }

            double xx,yy;
            switch (mark_locations_choice->value()){
                case 1:
                    xx=current->locations[0][snap_point].loc.x;
                    yy=current->locations[0][snap_point].loc.y;
                    break;
                case 12:
                    xx=current->points[snap_point].x;
                    yy=current->points[snap_point].y;
                    break;                
                case 11:

                    if (!alt_on){
                        return;
                    }

                    if (snap_point < static_cast<int>(current->walls_standing.size())){
                        if (snap_point_plus==1){
                            xx=current->walls_standing[snap_point].end.x;
                            yy=current->walls_standing[snap_point].end.y;
                        }
                        else if (snap_point_plus==0){
                            xx=current->walls_standing[snap_point].start.x;
                            yy=current->walls_standing[snap_point].start.y;
                        }
                        else{
                            return;
                        }
                    }    
                    else if  (snap_point == static_cast<int>(current->walls_standing.size())){
                        xx=temp_wall.start.x;
                        yy=temp_wall.start.y;
                    }
                    else{
                        return;
                    }
                    break;
            }
            
            std::stringstream s;
            s.precision(2);
            s<<std::fixed<<ang_snap*180.0/M_PI<<"°";
            
            gl_font(1, 10);
            double xwrite,ywrite,pixel;
            pixel=(maxx-minx)/static_cast<double>(this->w());
            xwrite =xx+ 7*pixel;
            ywrite = yy+ 7*pixel;
            glRasterPos3f(xwrite,ywrite,fore_z);
                       
            std::string t = s.str();
            
            glPushMatrix();
            
            glTranslatef(xwrite,ywrite,fore_z);
            glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
            glLineWidth(1.0);
            
            for(std::string::iterator c=t.begin() ; c!=t.end(); ++c)
            {
                glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
            }
            glPopMatrix();
            
        }
            
    }
    
}

void draw_window::test_snapon(){
    
    plan *floor_plan_ptr;
    if (program->floor_choice->value()>0){
        floor_plan_ptr=&current->floor_plans[program->floor_choice->value()-1];
    }
    else{
        return;
    }
    const plan &floor_plan = *floor_plan_ptr;

    double xx,yy;
    xx= x_mouse_coord;
    yy= y_mouse_coord;
    
    if ((shift_on)&&(!program->three_d_choice->value())){
        
        if ((mark_locations_choice->value()==12)&&(snap_point>-1)&&(snap_point<static_cast<int>(current->points.size()))){ 
                       
            snapon=0;
            
            double min=9e99;
            
            double x1=current->points[snap_point].x;
            double y1=current->points[snap_point].y;
            
            
            double x2= x1+cos(ang_snap);
            double y2= y1+sin(ang_snap);
            
            double px=x2-x1;
            double py=y2-y1;
            double s=px*px+py*py;
            double u=((xx-x1)*px+(yy-y1)*py)/s;
            double dx=x1+u*px-xx;
            double dy=y1+u*py-yy;
            double dist=dx*dx+dy*dy;
            
            
            if ((dist<min)){
                snapon=1;
                snapx=xx+dx;
                snapy=yy+dy;
                min=dist;
            }
            
            
            x2= x1+cos(ang_snap+0.5*M_PI);
            y2= y1+sin(ang_snap+0.5*M_PI);
            
            px=x2-x1;
            py=y2-y1;
            s=px*px+py*py;
            u=((xx-x1)*px+(yy-y1)*py)/s;
            dx=x1+u*px-xx;
            dy=y1+u*py-yy;
            dist=dx*dx+dy*dy;
            
            
            if ((dist<min)){
                snapon=1;
                snapx=xx+dx;
                snapy=yy+dy;            
                min=dist;
            }
                                
        }
        else if ((mark_locations_choice->value()==13)&&(snap_point>-1)&&(snap_point<static_cast<int>(current->points.size()))){ 
            snapx = current->points[snap_point].x;
            snapy = current->points[snap_point].y;
            snapon = 1;

        }
        else if ((mark_locations_choice->value()==1)&&(snap_point>-1)&&(snap_point < static_cast<int>(current->locations[0].size()))){ //drawing current->desks
            
            snapon=0;
            
            double min=9e99;
            
            double x1=current->locations[0][snap_point].loc.x;
            double y1=current->locations[0][snap_point].loc.y;
            
            
            double x2= x1+cos(ang_snap);
            double y2= y1+sin(ang_snap);
            
            double px=x2-x1;
            double py=y2-y1;
            double s=px*px+py*py;
            double u=((xx-x1)*px+(yy-y1)*py)/s;
            double dx=x1+u*px-xx;
            double dy=y1+u*py-yy;
            double dist=dx*dx+dy*dy;
            
            
            if ((dist<min)){
                snapon=1;
                snapx=xx+dx;
                snapy=yy+dy;                
                min=dist;
            }
            
            
            x2= x1+cos(ang_snap+0.5*M_PI);
            y2= y1+sin(ang_snap+0.5*M_PI);
            
            px=x2-x1;
            py=y2-y1;
            s=px*px+py*py;
            u=((xx-x1)*px+(yy-y1)*py)/s;
            dx=x1+u*px-xx;
            dy=y1+u*py-yy;
            dist=dx*dx+dy*dy;
            
            
            if ((dist<min)){
                snapon=1;
                snapx=xx+dx;
                snapy=yy+dy;               
                min=dist;
            }                        
            
        }
        else if (alt_on){ //walls and floor plans snapping onto previously existing constructs


            if ((mark_locations_choice->value()==14)&&(snap_point>-1)&&(snap_point < static_cast<int>(current->floorarea_polygons.size()))){

                if (snap_point_plus<static_cast<int>(current->floorarea_polygons[snap_point].size())){

                    snapon=0;
                
                    double min=9e99;
                    
                    double x1=current->floorarea_polygons[snap_point][snap_point_plus].x;
                    double y1=current->floorarea_polygons[snap_point][snap_point_plus].y;
                    
                    
                    double x2= x1+cos(ang_snap);
                    double y2= y1+sin(ang_snap);
                    
                    double px=x2-x1;
                    double py=y2-y1;
                    double s=px*px+py*py;
                    double u=((xx-x1)*px+(yy-y1)*py)/s;
                    double dx=x1+u*px-xx;
                    double dy=y1+u*py-yy;
                    double dist=dx*dx+dy*dy;
                    
                    
                    if ((dist<min)){
                        snapon=1;
                        snapx=xx+dx;
                        snapy=yy+dy;                
                        min=dist;
                    }
                    
                    
                    x2= x1+cos(ang_snap+0.5*M_PI);
                    y2= y1+sin(ang_snap+0.5*M_PI);
                    
                    px=x2-x1;
                    py=y2-y1;
                    s=px*px+py*py;
                    u=((xx-x1)*px+(yy-y1)*py)/s;
                    dx=x1+u*px-xx;
                    dy=y1+u*py-yy;
                    dist=dx*dx+dy*dy;
                    
                    
                    if ((dist<min)){
                        snapon=1;
                        snapx=xx+dx;
                        snapy=yy+dy;               
                        min=dist;
                    } 

                }

            }

            if (((mark_locations_choice->value()==11)||(mark_locations_choice->value()==10))&&(snap_point>-1)){

                std::vector<wall> *wall_ptr = NULL;

                if (snap_wall){
                    wall_ptr = &current->walls_standing;
                }
                else{
                    wall_ptr = &current->walls_sitting;
                }

                std::vector<wall> &w = *wall_ptr;

                if (snap_point <= static_cast<int>(w.size())){

                    snapon=0;
                
                    double min=9e99;

                    double x1=0,y1=0;

                    if (snap_point < static_cast<int>(w.size())){

                        if (snap_point_plus==1){
                            x1=w[snap_point].end.x;
                            y1=w[snap_point].end.y;        
                        }
                        else if (snap_point_plus==0){
                            x1=w[snap_point].start.x;
                            y1=w[snap_point].start.y;           
                        }
                    }
                    else if ((snap_point == static_cast<int>(w.size()))&&(snap_wall)){
                        x1=temp_wall.start.x;        
                        y1=temp_wall.start.y;        
                    }
                                            
                    
                    double x2= x1+cos(ang_snap);
                    double y2= y1+sin(ang_snap);
                    
                    double px=x2-x1;
                    double py=y2-y1;
                    double s=px*px+py*py;
                    double u=((xx-x1)*px+(yy-y1)*py)/s;
                    double dx=x1+u*px-xx;
                    double dy=y1+u*py-yy;
                    double dist=dx*dx+dy*dy;
                    
                    
                    if ((dist<min)){
                        snapon=1;
                        snapx=xx+dx;
                        snapy=yy+dy;                
                        min=dist;
                    }
                    
                    
                    x2= x1+cos(ang_snap+0.5*M_PI);
                    y2= y1+sin(ang_snap+0.5*M_PI);
                    
                    px=x2-x1;
                    py=y2-y1;
                    s=px*px+py*py;
                    u=((xx-x1)*px+(yy-y1)*py)/s;
                    dx=x1+u*px-xx;
                    dy=y1+u*py-yy;
                    dist=dx*dx+dy*dy;
                    
                    
                    if ((dist<min)){
                        snapon=1;
                        snapx=xx+dx;
                        snapy=yy+dy;               
                        min=dist;
                    }
                } 

            }

        }
        else{ //regular snap on: onto floor plans and walls
            
           
            snapon=0;
            
            double min=9e99;

            double cutoff=0.03*(maxx-minx)*0.03*(maxx-minx);

            if ((mark_locations_choice->value()==10)||(mark_locations_choice->value()==11)){//if editing walls:snap on to walls too
                for (unsigned int i=0;i<current->walls_sitting.size();i++){
                    
                    double x1=current->walls_sitting[i].start.x;
                    double x2=current->walls_sitting[i].end.x;
                    double y1=current->walls_sitting[i].start.y;
                    double y2=current->walls_sitting[i].end.y;
                    
                    double px=x2-x1;
                    double py=y2-y1;
                    double s=px*px+py*py;
                    double u=((xx-x1)*px+(yy-y1)*py)/s;
                    if (u>1)u=1;
                    if (u<0)u=0;
                    double dx=x1+u*px-xx;
                    double dy=y1+u*py-yy;
                    double dist=dx*dx+dy*dy;
                    
                    //end distance to segement
                    
                    if ((dist<min)&&(dist<cutoff)){
                        snapon=1;
                        snapx=xx+dx;
                        snapy=yy+dy;
                        min=dist;
                    }                    
                    
                }
                for (unsigned int i=0;i<current->walls_standing.size();i++){
                    
                    
                    double x1=current->walls_standing[i].start.x;
                    double x2=current->walls_standing[i].end.x;
                    double y1=current->walls_standing[i].start.y;
                    double y2=current->walls_standing[i].end.y;
                    
                    double px=x2-x1;
                    double py=y2-y1;
                    double s=px*px+py*py;
                    double u=((xx-x1)*px+(yy-y1)*py)/s;
                    if (u>1)u=1;
                    if (u<0)u=0;
                    double dx=x1+u*px-xx;
                    double dy=y1+u*py-yy;
                    double dist=dx*dx+dy*dy;
                    
                    //end distance to segement
                    
                    if ((dist<min)&&(dist<cutoff)){
                        snapon=1;
                        snapx=xx+dx;
                        snapy=yy+dy;
                        min=dist;
                    }
                    
                }
            }//end snap on to walls

            if (floor_plan.fast_plan){
                find_plan_snap_point_fast(floor_plan,min,xx,yy,cutoff,false);
            }
            else{
                find_plan_snap_point(floor_plan,min,xx,yy,cutoff,false);
            }

            if (root->snap_to_point){
                min=9e99;
                if (floor_plan.fast_plan){
                    find_plan_snap_point_fast(floor_plan,min,xx,yy,cutoff,true);                
                }
                else{
                    find_plan_snap_point(floor_plan,min,xx,yy,cutoff,true);                
                }
            }
            
        }
        
    }
    else{
        snapon=0;
    }   
    
}

void draw_window::find_snap_point(){

    if ((shift_on)&&(!program->three_d_choice->value())&&(snap_point==-1)){

        if ((mark_locations_choice->value()==12)||(mark_locations_choice->value()==13)){ //points or edge
            
            double min=9e99,dist;
            double xx,yy;
            xx= x_mouse_coord;
            yy= y_mouse_coord;
            
            program->points_mutex.lock();

            for (unsigned int i=0;i<current->points.size();i++){
                if (static_cast<int>(i) != drag_point){
                    dist=(xx-current->points[i].x)*(xx-current->points[i].x)+(yy-current->points[i].y)*(yy-current->points[i].y);
                    if ((dist<min)&&(current->points[i].floor==flr_choice)){
                        min=dist;
                        snap_point=i;
                    }
                }
            }

            program->points_mutex.unlock();
            
        }
        else if (mark_locations_choice->value()==1){ //origins
            double min=9e99,dist;
            double xx,yy;
            xx= x_mouse_coord;
            yy= y_mouse_coord;

            program->locations_mutex.lock();
            
            for (unsigned int i=0;i<current->locations[0].size();i++){
                if (static_cast<int>(i) != drag_point){
                    dist=(xx-current->locations[0][i].loc.x)*(xx-current->locations[0][i].loc.x)+(yy-current->locations[0][i].loc.y)*(yy-current->locations[0][i].loc.y);
                    if ((dist<min)&&(current->locations[0][i].loc.floor==flr_choice)){
                        min=dist;
                        snap_point=i;
                    }
                }
                
            }

            program->locations_mutex.unlock();                        
        }
        else if (mark_locations_choice->value()==14){  //floor areas

            double min=9e99,dist;
            double xx,yy;
            xx= x_mouse_coord;
            yy= y_mouse_coord;

            program->areas_mutex.lock();

            for (unsigned int i=0;i<current->floorarea_polygons.size();i++){

                for (unsigned int j=0;j<current->floorarea_polygons[i].size();j++){

                    if (current->floorarea_polygons[i][0].floor==flr_choice){
                        dist = (xx-current->floorarea_polygons[i][j].x)*(xx-current->floorarea_polygons[i][j].x)+(yy-current->floorarea_polygons[i][j].y)*(yy-current->floorarea_polygons[i][j].y);
                        if (dist<min){
                            min=dist;
                            snap_point = i;
                            snap_point_plus = j;
                        }

                    }

                }

            }

            program->areas_mutex.unlock();

        }
        else if ((mark_locations_choice->value()==10)||(mark_locations_choice->value()==11)){  //walls

            program->walls_mutex.lock();

            double min=9e99,dist;
            double xx,yy;
            xx= x_mouse_coord;
            yy= y_mouse_coord;

            if ((mark_locations_choice->value()==11)||(program->show_walls_standing->value())){

                for (unsigned int i=0;i<current->walls_standing.size();i++){

                    
                    if (current->walls_standing[i].start.floor==flr_choice){
                        dist = (xx-current->walls_standing[i].start.x)*(xx-current->walls_standing[i].start.x)+(yy-current->walls_standing[i].start.y)*(yy-current->walls_standing[i].start.y);
                        if (dist<min){
                            min=dist;
                            snap_point = i;
                            snap_point_plus = 0;
                            snap_wall = 1;
                        }

                        dist = (xx-current->walls_standing[i].end.x)*(xx-current->walls_standing[i].end.x)+(yy-current->walls_standing[i].end.y)*(yy-current->walls_standing[i].end.y);
                        if (dist<min){
                            min=dist;
                            snap_point = i;
                            snap_point_plus = 1;
                            snap_wall = 1;
                        }

                    }                

                }
            }

            if ((mark_locations_choice->value()==10)||(program->show_walls_sitting->value())){

                for (unsigned int i=0;i<current->walls_sitting.size();i++){

                    
                    if (current->walls_sitting[i].start.floor==flr_choice){
                        dist = (xx-current->walls_sitting[i].start.x)*(xx-current->walls_sitting[i].start.x)+(yy-current->walls_sitting[i].start.y)*(yy-current->walls_sitting[i].start.y);
                        if (dist<min){
                            min=dist;
                            snap_point = i;
                            snap_point_plus = 0;
                            snap_wall = 0;
                        }

                        dist = (xx-current->walls_sitting[i].end.x)*(xx-current->walls_sitting[i].end.x)+(yy-current->walls_sitting[i].end.y)*(yy-current->walls_sitting[i].end.y);
                        if (dist<min){
                            min=dist;
                            snap_point = i;
                            snap_point_plus = 1;
                            snap_wall = 0;
                        }

                    }                

                }
            }

            program->walls_mutex.unlock();

            if (set_first){
                if (temp_wall.start.floor==flr_choice){
                    dist = (xx-temp_wall.start.x)*(xx-temp_wall.start.x)+(yy-temp_wall.start.y)*(yy-temp_wall.start.y);
                    if (dist<min){
                        min=dist;
                        snap_point = current->walls_standing.size(); //arbitrary - just bigger than possible -used as code for temp wall
                        snap_point_plus = 0;
                        snap_wall = 1; //arbitrary to match snap_point
                    }
                }            
            }            

        }
        

    }
}


void draw_window::find_plan_snap_point_circle(const circle &data,double &running_min,const double& xx,const double& yy,const double &cutoff,const matrix_2d & mat,const double &shift_x,const double &shift_y,const double &xs,const double &ys, bool snap_to_point_priority){

    if (snap_to_point_priority){

       return; // no points in a circle

    }

    const double &cx=data.x;
    const double &cy=data.y;
    const double &r=data.r;
    
    int num_segments=20+abs(static_cast<int>(40*((2.0*M_PI*r)/(maxx-minx))));
    
    
    double xo= r; 
    double yo= 0;

    auto xyo = mat.mult_vec({xo + cx + xs,yo + cy + ys});
 
    matrix_2d eye;

    double d_ang = 2.0*M_PI/(static_cast<double>(num_segments)-1.0);

    line_in line;

    for (int i=0;i<num_segments;i++){
        
        double x= r*cos(i*d_ang); //watch for specific fasb(r) vs not behaviour is x and y: ridiculous format
        double y= r*sin(i*d_ang);

        auto xyn = mat.mult_vec({x+cx+xs,y+cy+ys});
                
        line.start.x=xyo[0];
        line.end.x=xyn[0];
        line.start.y=xyo[1];
        line.end.y=xyn[1];

        find_plan_snap_point_line(line,running_min,xx,yy,cutoff,eye,shift_x,shift_y,0,0,false);

        xyo=xyn;

    }

}

void draw_window::find_plan_snap_point_arc(const arc &data,double &running_min,const double& xx,const double& yy,const double &cutoff,const matrix_2d & mat,const double &shift_x,const double &shift_y,const double &xs,const double &ys, bool snap_to_point_priority){

    const double &cx=data.x;
    const double &cy=data.y;
    const double &r=data.r;
    const double &ang1=data.ang1;
    const double &ang2=data.ang2;
    int num_segments=20+abs(static_cast<int>(40*(((ang2-ang1)*r)/(maxx-minx))));
    
    
    double xo= r*cos(ang1);
    double yo= r*sin(ang1);

    auto xyo = mat.mult_vec({xo + cx + xs,yo + cy + ys});
        
    if (snap_to_point_priority){
       
        if ((ang2-ang1)>= 1.99*M_PI){
            return;
        }

        xyo[0] += shift_x;
        xyo[1] += shift_y;

        double dist1 = (xx-xyo[0])*(xx-xyo[0])+(yy-xyo[1])*(yy-xyo[1]);

        double xl = r*cos(ang2); 
        double yl = r*sin(ang2);        

        double x= xs + cx + xl;
        double y= ys + cy + yl;

        auto xyn = mat.mult_vec({x,y});
    
        xyn[0] += shift_x;
        xyn[1] += shift_y;

        double dist2 =  (xx-xyn[0])*(xx-xyn[0])+(yy-xyn[1])*(yy-xyn[1]);        

        if ((dist1<running_min)&&(dist1<cutoff)){
            snapon=1;
            snapx=xyo[0];
            snapy=xyo[1];
            running_min=dist1;
        }

        if ((dist2<running_min)&&(dist2<cutoff)){
            snapon=1;
            snapx=xyn[0];
            snapy=xyn[1];
            running_min=dist2;     
        }

    }
    else{

        matrix_2d eye;

        double d_ang = (ang2-ang1)/(static_cast<double>(num_segments)-1.0);

        line_in line;

        for (int i=0;i<num_segments;i++){
            
            double x= r*cos(ang1+i*d_ang);
            double y= r*sin(ang1+i*d_ang);

            auto xyn = mat.mult_vec({x+cx+xs,y+cy+ys});
            
            line.start.x=xyo[0];
            line.end.x=xyn[0];
            line.start.y=xyo[1];
            line.end.y=xyn[1];

            find_plan_snap_point_line(line,running_min,xx,yy,cutoff,eye,shift_x,shift_y,0,0,false);

            xyo=xyn;

        }
    }


}

void draw_window::find_plan_snap_point_ellipse(const ellipse &data,double &running_min,const double& xx,const double& yy,const double &cutoff,const matrix_2d & mat,const double &shift_x,const double &shift_y,const double &xs,const double &ys, bool snap_to_point_priority){


    //dont bother with the transform here - just approximate with lines for which we already have the appropriate matrix adjusted function for

    const double &cx=data.x;
    const double &cy=data.y;
    const double &r1=data.r1;
    const double &r2=data.r2;
    const double &cos_phi = data.cos_phi;
    const double &sin_phi = data.sin_phi;
    const double &ang1 = data.ang1;
    const double &ang2 = data.ang2;
    
    double xr = cos(ang1); 
    double yr = sin(ang1);

    double xo= xs + cx +(r1*xr*cos_phi-r2*yr*sin_phi);
    double yo= ys + cy +(r1*xr*sin_phi+r2*yr*cos_phi);

    auto xy = mat.mult_vec({xo,yo});
    xo = xy[0];
    yo = xy[1];


    matrix_2d eye;

    if (snap_to_point_priority){

        if (ang2-ang1>= 1.99*M_PI ){
            return;
        }

        xo += shift_x;
        yo += shift_y;

        double dist1 = (xx-xo)*(xx-xo)+(yy-yo)*(yy-yo);

        double xl = cos(ang2); 
        double yl = sin(ang2);        

        double x= xs + cx +(r1*xl*cos_phi-r2*yl*sin_phi);
        double y= ys + cy +(r1*xl*sin_phi+r2*yl*cos_phi);

        auto xyn = mat.mult_vec({x,y});
        xyn[0] += shift_x;
        xyn[1] += shift_y;
    
        double dist2 = (xx-xyn[0])*(xx-xyn[0])+(yy-xyn[1])*(yy-xyn[1]);        

        if ((dist1<running_min)&&(dist1<cutoff)){
            snapon=1;
            snapx=xo;
            snapy=yo;
            running_min=dist1;
        }

        if ((dist2<running_min)&&(dist2<cutoff)){
            snapon=1;
            snapx=xyn[0];
            snapy=xyn[1];
            running_min=dist2;
        }

    }
    else{
       
        int num_segments=20+abs(static_cast<int>(40*(((ang2-ang1)*r1)/(maxx-minx))));
        double d_ang = (ang2-ang1)/(static_cast<double>(num_segments)-1.0);

        line_in line;

        for (int i=1;i<num_segments;i++){

            double xl = cos(ang1+i*d_ang); 
            double yl = sin(ang1+i*d_ang);        

            double x= xs + cx +(r1*xl*cos_phi-r2*yl*sin_phi);
            double y= ys + cy +(r1*xl*sin_phi+r2*yl*cos_phi);

            auto xy = mat.mult_vec({x,y});
            x = xy[0];
            y = xy[1];

            
            line.start.x=xo;
            line.end.x=x;
            line.start.y=yo;
            line.end.y=y;


            find_plan_snap_point_line(line,running_min,xx,yy,cutoff,eye,shift_x,shift_y,0,0,false);


            xo=x;
            yo=y;
            
        }
    }


}


void draw_window::find_plan_snap_point_line(const line_in &data,double &running_min,const double& xx,const double& yy,const double &cutoff,const matrix_2d & mat,const double &shift_x,const double &shift_y,const double &xs,const double &ys, bool snap_to_point_priority){

    const double &x1p=data.start.x;
    const double &x2p=data.end.x;
    const double &y1p=data.start.y;
    const double &y2p=data.end.y;


    auto xy1 = mat.mult_vec({x1p+xs,y1p+ys});
    auto xy2 = mat.mult_vec({x2p+xs,y2p+ys});

    xy1[0] += shift_x;
    xy1[1] += shift_y;
    xy2[0] += shift_x;
    xy2[1] += shift_y;

    if (snap_to_point_priority){

        double dist1 = (xx-xy1[0])*(xx-xy1[0])+(yy-xy1[1])*(yy-xy1[1]);
        double dist2 = (xx-xy2[0])*(xx-xy2[0])+(yy-xy2[1])*(yy-xy2[1]);

        if ((dist1<running_min)&&(dist1<cutoff)){
            snapon=1;
            snapx=xy1[0];
            snapy=xy1[1];
            running_min=dist1;   
        }

        if ((dist2<running_min)&&(dist2<cutoff)){
            snapon=1;
            snapx=xy2[0];
            snapy=xy2[1];
            running_min=dist2;
        }

    }
    else{
        
        double px=xy2[0]-xy1[0];
        double py=xy2[1]-xy1[1];
        double s=px*px+py*py;
        double u=((xx-xy1[0])*px+(yy-xy1[1])*py)/s;
        if (u>1)u=1;
        if (u<0)u=0;
        double dx=xy1[0]+u*px-xx;
        double dy=xy1[1]+u*py-yy;
        double dist=dx*dx+dy*dy;
        
        //end distance to segement
        
        if ((dist<running_min)&&(dist<cutoff)){
            snapon=1;
            snapx=xx+dx;
            snapy=yy+dy;
            running_min=dist;
        }
    }

}

inline void draw_window::find_plan_snap_point_spline(const spline &data,double &running_min,const double& xx,const double& yy,const double &cutoff,const matrix_2d & mat,const double &shift_x,const double &shift_y,const double &xs,const double &ys, bool snap_to_point_priority){
    
    double xo = data.get_x(data.t_start);
    double yo = data.get_y(data.t_start);
    auto xyo = mat.mult_vec({xo+xs,yo+ys});

    matrix_2d eye;

    if (snap_to_point_priority){

        xo = xyo[0]+shift_x;
        yo = xyo[1]+shift_y;

        double dist1 = (xx-xo)*(xx-xo)+(yy-yo)*(yy-yo);

        double xn = data.get_x(data.t_end);
        double yn = data.get_y(data.t_end);
        
        auto xy2 = mat.mult_vec({xn+xs,yn+ys});
        
        xn = xy2[0] + shift_x;
        yn = xy2[1] + shift_y;

        double dist2 = (xx-xn)*(xx-xn)+(yy-yn)*(yy-yn);

        if ((dist1<running_min)&&(dist1<cutoff)){
            snapon=1;
            snapx=xo;
            snapy=yo;
            running_min=dist1;
        }

        if ((dist2<running_min)&&(dist2<cutoff)){
            snapon=1;
            snapx=xn;
            snapy=yn;
            running_min=dist2;
        }

    }
    else{

        int n = std::max(static_cast<int>(data.knots.size()),abs(static_cast<int>(40*data.approx_size/(maxx-minx))));
        double dt = data.t_range/static_cast<double>(n);

        for (int i=0;i<n;i++){
            double t = data.t_start + (i+1)*dt;
            double xn = data.get_x(t);
            double yn = data.get_y(t);    

            auto xyn = mat.mult_vec({xn+xs,yn+ys});
            
            line_in line;
            line.start.x=xyo[0];
            line.end.x=xyn[0];
            line.start.y=xyo[1];
            line.end.y=xyn[1];

            find_plan_snap_point_line(line,running_min,xx,yy,cutoff,eye,shift_x,shift_y,0,0,false);

            xyo=xyn;
            
        }

    }

    
    
}

void draw_window::find_plan_snap_point_block(const block_entry &block,const plan &floor_plan,double &running_min,const double& xx,const double& yy,const double &cutoff,const matrix_2d & mat,const double &shift_x,const double &shift_y,const double &tot_xs,const double &tot_ys, bool snap_to_point_priority){

    const std::vector<block_entry> &blocks=floor_plan.block_definitions[block.list_no].nested_blocks; //use data sytructures in relevant block definition
    const std::vector<line_in> &lines=floor_plan.block_definitions[block.list_no].lines;
    const std::vector<arc> &arcs=floor_plan.block_definitions[block.list_no].arcs;
    const std::vector<circle> &circles=floor_plan.block_definitions[block.list_no].circles;
    const std::vector<ellipse> &ellipses=floor_plan.block_definitions[block.list_no].ellipses;
    const std::vector<spline> &splines=floor_plan.block_definitions[block.list_no].splines;
    const double xs = tot_xs-floor_plan.block_definitions[block.list_no].start_x;//insert_x +ii*rs-start_x;
    const double ys = tot_ys-floor_plan.block_definitions[block.list_no].start_y;//;insert_y + jj*cs-start_y;

    for (const arc &x :arcs){
        
        find_plan_snap_point_arc(x,running_min,xx,yy,cutoff,mat,shift_x,shift_y,xs,ys,snap_to_point_priority);
        
    }
    
    for (const circle &x : circles){
                   
        find_plan_snap_point_circle(x,running_min,xx,yy,cutoff,mat,shift_x,shift_y,xs,ys,snap_to_point_priority);
                
    }

    for (const ellipse &x : ellipses){
            
        find_plan_snap_point_ellipse(x,running_min,xx,yy,cutoff,mat,shift_x,shift_y,xs,ys,snap_to_point_priority);
        
    }
        
    for (const line_in &x : lines){
            
        find_plan_snap_point_line(x,running_min,xx,yy,cutoff,mat,shift_x,shift_y,xs,ys,snap_to_point_priority);

    }

    if (use_splines){
        for (const spline &x : splines){
                
            find_plan_snap_point_spline(x,running_min,xx,yy,cutoff,mat,shift_x,shift_y,xs,ys,snap_to_point_priority);

        }
    }


    for (const auto &x : blocks){
        
        if (floor_plan.blocks_active[x.list_no]){

            double ix = x.insert_x + tot_xs;
            double iy = x.insert_y + tot_ys;

                    
            // we unavoidably have to caculate the matix here as it depends on the insertion point of the parent block.
            // the data we have is for inserts into a generic block and so unless we duplicate tons of data we don't have
            // an associated previous matrix with the current matrix to be calculated

            matrix_2d mat1;
            rotate_matrix rot(x.rotation*M_PI/180.0);
            translate_matrix t1(ix,iy);
            translate_matrix t2(-ix,-iy);
            scale_matrix s(x.scale_x,x.scale_y);

            mat1 = mat1.mult(t2);
            mat1 = mat1.mult(s);
            mat1 = mat1.mult(rot);
            mat1 = mat1.mult(t1);

            
            mat1 = mat1.mult(mat);

            find_plan_snap_point_block(x,floor_plan,running_min,xx,yy,cutoff,mat1,shift_x,shift_y,ix,iy,snap_to_point_priority);
        
        }
        
    }

}


void draw_window::find_plan_snap_point(const plan &floor_plan,double &running_min,const double& xx,const double& yy,const double &cutoff, bool snap_to_point_priority){

    //called by test_snapon()

    const double &shift_x=floor_plan.shift_x;
    const double &shift_y=floor_plan.shift_y;
    
    
    matrix_2d eye;

    for (const auto & x : floor_plan.lines){

        if (floor_plan.layers[x.layer]){
            find_plan_snap_point_line(x,running_min,xx,yy,cutoff,eye,shift_x,shift_y,0,0,snap_to_point_priority);
        }
    }

    for (const auto & x : floor_plan.circles){

        if (floor_plan.layers[x.layer]){
            find_plan_snap_point_circle(x,running_min,xx,yy,cutoff,eye,shift_x,shift_y,0,0,snap_to_point_priority);
        }
    }

    for (const auto & x : floor_plan.arcs){

        if (floor_plan.layers[x.layer]){
            find_plan_snap_point_arc(x,running_min,xx,yy,cutoff,eye,shift_x,shift_y,0,0,snap_to_point_priority);
        }
    }

    for (const auto & x : floor_plan.ellipses){

        if (floor_plan.layers[x.layer]){
            find_plan_snap_point_ellipse(x,running_min,xx,yy,cutoff,eye,shift_x,shift_y,0,0,snap_to_point_priority);
        }
    }

    if (use_splines){
        for (const auto & x : floor_plan.splines){

            if (floor_plan.layers[x.layer]){
                find_plan_snap_point_spline(x,running_min,xx,yy,cutoff,eye,shift_x,shift_y,0,0,snap_to_point_priority);
            }
        }
    }

    
    for (const auto & x : floor_plan.blocks){

        if ((floor_plan.layers[x.layer])&&(floor_plan.blocks_active[x.list_no])){
            double ix = x.insert_x;
            double iy = x.insert_y;
            find_plan_snap_point_block(x,floor_plan,running_min,xx,yy,cutoff,x.matrix,shift_x,shift_y,ix,iy,snap_to_point_priority);
        }
    }


}


void draw_window::find_plan_snap_point_line_fast(const floor_line &data,double &running_min,const double& xx,const double& yy,const double &cutoff,bool snap_to_point_priority){

    const double &xy10=data.start.x;
    const double &xy20=data.end.x;
    const double &xy11=data.start.y;
    const double &xy21=data.end.y;

    if (snap_to_point_priority){

        double dist1 = (xx-xy10)*(xx-xy10)+(yy-xy11)*(yy-xy11);
        double dist2 = (xx-xy20)*(xx-xy20)+(yy-xy21)*(yy-xy21);


        if ((data.point == FIRST)||(data.point == BOTH)){

            if ((dist1<running_min)&&(dist1<cutoff)){
                snapon=1;
                snapx=xy10;
                snapy=xy11;
                running_min=dist1;
            }
        }

        if ((data.point == LAST)||(data.point == BOTH)){
            if ((dist2<running_min)&&(dist2<cutoff)){
                snapon=1;
                snapx=xy20;
                snapy=xy21;
                running_min=dist2;
            }
        }

    }
    else{
        
        double px=xy20-xy10;
        double py=xy21-xy11;
        double s=px*px+py*py;
        double u=((xx-xy10)*px+(yy-xy11)*py)/s;
        if (u>1)u=1;
        if (u<0)u=0;
        double dx=xy10+u*px-xx;
        double dy=xy11+u*py-yy;
        double dist=dx*dx+dy*dy;
        
        //end distance to segement
        
        if ((dist<running_min)&&(dist<cutoff)){
            snapon=1;
            snapx=xx+dx;
            snapy=yy+dy;
            running_min=dist;
        }
    }

}

void draw_window::find_plan_snap_point_fast(const plan &floor_plan,double &running_min,const double& xx,const double& yy,const double &cutoff, bool snap_to_point_priority){


    for (unsigned int i=0;i<floor_plan.layers.size();i++){
        if (floor_plan.layers[i]){
            for (auto &x:floor_plan.line_only_version[i]){
                if ((!use_splines)&&(x.is_spline)){
                    continue;
                }

                if (x.block>-1){
                    if (!floor_plan.blocks_active[x.block]){
                        continue;
                    }
                }

                find_plan_snap_point_line_fast(x,running_min,xx,yy,cutoff,snap_to_point_priority);
                
            }
        }   
    }
}