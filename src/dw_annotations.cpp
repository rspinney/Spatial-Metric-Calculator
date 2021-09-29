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

void draw_window::showcoords(double fore_z){
    
    if ((!program->three_d_choice->value())&&(context_valid())&&(!first_draw)){
    
        double xx,yy;
        xx= x_mouse_coord;
        yy= y_mouse_coord;
        
        glPushMatrix();  
        
        draw_snapon(fore_z); //use mouse call to draw snap on...
           
        if (xx>maxx)xx=maxx;
        if (xx<minx)xx=minx;
        if (yy>maxy)yy=maxy;
        if (yy<miny)yy=miny;
            
        if (snapon){
            xx=snapx;
            yy=snapy;
        }
               
        gl_font(1, 10);
        
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(200,200,200);
        }

        std::stringstream s;
        s<<std::setprecision(2);

        double off =0;

        double width_leg=6;
        
        
        if (fabs(xx)<=1){
            width_leg+=0.7;
        }
        
        if (fabs(yy)<=1){
            width_leg+=0.7;
        }
        
        if ((maxx-minx)>10000){
            s<<std::scientific;
            s<<std::setprecision(2);
            off = 5;
        }
        else if ((maxx-minx)<2){
            int i = static_cast<int> (fabs(log10(maxx-minx)));
            s<<std::fixed;
            s<<std::setprecision(i+3);
            off = 2+ 0.7*(i);
            width_leg+=0.7*(floor(log10(fabs(xx)))+1);
            width_leg+=0.7*(floor(log10(fabs(yy)))+1);
        }
        else{
            s<<std::fixed;
            off=1;
            width_leg+=0.7*(floor(log10(fabs(xx)))+1);
            width_leg+=0.7*(floor(log10(fabs(yy)))+1);
        }

        s<<xx<<", "<<yy;
        
        double xwrite,ywrite;
        
        double pixel=(maxx-minx)/static_cast<double>(this->w());
           
        if (xx<0)width_leg+=1.5;
        if (yy<0)width_leg+=1.5;
        
        
        xwrite =maxx-(2*off+width_leg)*10*pixel;
        ywrite = miny+ 5*pixel;
        glRasterPos3f(xwrite,ywrite,fore_z);
       
        std::string t = s.str();
        
        glPushMatrix();
        
        glTranslatef(xwrite,ywrite,0);
        glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
       
        glLineWidth(1.0);
       
        glDisable(GL_DEPTH_TEST);
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
        
        glPopMatrix();    
        
    }
}

void draw_window::drawcursor(double fore_z){
    
    if ((root->graphic_cursor)&&(Fl::event_inside(this))){

        const double &xx = x_mouse_coord;
        const double &yy = y_mouse_coord;
        
        double pixel=(maxx-minx)/static_cast<double>(this->w());
        
        glRasterPos3f(xx-pixel,yy-7*pixel,fore_z);
        glDrawPixels(3, 15, GL_RGB, GL_FLOAT, cursor1);
        glRasterPos3f(xx-7*pixel,yy-pixel,fore_z);
        glDrawPixels(15, 3, GL_RGB, GL_FLOAT, cursor2);

        return;

    }
             
}


void draw_window::show_FOV(double fore_z){
    
    if ((program->three_d_choice->value()>1)&&(root->show_scale)){
        std::stringstream s;
        s.precision(2);
        if (program->three_d_choice->value()==3){
            s<<std::fixed<<"x: "<<xfp<<" y: "<<yfp<<" z: "<<zfp<<" ";
        }
        else{
            if (z_dist> 1e5){
                s<<std::scientific<<"distance: "<<z_dist<<" ";
            }
            else{
                s<<std::fixed<<"distance: "<<z_dist<<" ";
            }
        }
        
        s<<std::fixed<<"FOV: "<<180.0*(2.0*(atan(0.5*(maxx-minx)/(maxx_overall-minx_overall)))/M_PI)<<"°";      
        
        gl_font(1, 10);
        double xwrite,ywrite,pixel;
        pixel=(maxx-minx)/static_cast<double>(this->w());
        xwrite =minx+ 10*pixel;
        ywrite = miny+ 5*pixel;
        glRasterPos3f(xwrite,ywrite,fore_z);

        std::string t= s.str();
        
        glPushMatrix();
        glTranslatef(xwrite, ywrite, 0);
        glScalef(0.1*pixel, 0.1*pixel, 1/152.38);
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) 
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
              
    }
}


void draw_window::draw_rot_axis(const double& back_z,const double& fore_z){
    if (root->show_rot_axis){

        if ((program->three_d_choice->value()==1)||(program->three_d_choice->value()==2)){

            glColor3ub(255,50,50);
            glLineWidth(1.0);
            glBegin(GL_LINES);
            glVertex3d(current->mid_x,current->mid_y,back_z);
            glVertex3d(current->mid_x,current->mid_y,fore_z);
            glEnd();
        }
        else if(program->three_d_choice->value()==0){

            current->mid_x=0.5*(minx+maxx);
            current->mid_y=0.5*(miny+maxy);

            double dx=7.0*(maxx-minx)/static_cast<double>(this->w());
            glColor3ub(255,50,50);
            glLineWidth(1.0);
            glBegin(GL_LINES);
            glVertex3d(current->mid_x-dx,current->mid_y,fore_z);
            glVertex3d(current->mid_x+dx,current->mid_y,fore_z);
            glVertex3d(current->mid_x,current->mid_y-dx,fore_z);
            glVertex3d(current->mid_x,current->mid_y+dx,fore_z);
            glEnd();
        }
    }
}

void draw_window::showpoints(){

    if ((root->draw_points_label)&&(program->show_map->value())){//show current->points
        if (program->three_d_choice->value()<2){
            
            glPushMatrix();
            //glDisable(GL_DEPTH_TEST);
            
            
            double pixel=(maxx-minx)/static_cast<double>(this->w());
            glLineWidth(1.0);
            
            for (unsigned int i=0;i<current->points.size();i++){
                
                
                if ((all_flrs)||(current->points[i].floor==flr_choice)){
                    
                   
                    gl_font(1, 14);
                    if (root->bow){
                        glColor3ub(0,0,0);
                    }
                    else{
                        glColor3ub(200,200,200);
                    }
                    glRasterPos2f(current->points[i].x+0.01,current->points[i].y+0.01);
                    std::stringstream s;
                    s<<i;
                    std::string t = s.str();
                   
                    glPushMatrix();
                    
                    double dd = 0.525 * program->point_size->value();
                    
                    glTranslatef(current->points[i].x+dd,current->points[i].y+dd, current->points[i].z+dd);
                    glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
                    
                    if ((program->three_d_choice->value()>0)){
                        glRotatef(-ang_z,0,0,1);
                        glRotatef(-ang_x,1,0,0);
                    }
                    
                    for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                    {
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                    }
                    glPopMatrix();
                    
                    
                }
            }
            
            glPopMatrix();
            
            
        }
    }
}

void draw_window::showlocations(){

    if ((root->draw_locations_label)&&(program->show_map->value())){//show current->points
        if (program->three_d_choice->value()<2){
            for (int ii=0;ii<9;ii++){
                                        
                 if (program->show_locations[ii]->value()){

                    glPushMatrix();
                    
                    double pixel=(maxx-minx)/static_cast<double>(this->w());
                    glLineWidth(1.0);

                    for (unsigned int i=0;i<current->locations[ii].size();i++){
                    
                    
                        if ((all_flrs)||(current->locations[ii][i].loc.floor==flr_choice)){
                            
                           
                            gl_font(1, 14);
                            if (root->bow){
                                glColor3ub(0,0,0);
                            }
                            else{
                                glColor3ub(200,200,200);
                            }
                            glRasterPos2f(current->locations[ii][i].loc.x+0.01,current->locations[ii][i].loc.y+0.01);
                            std::stringstream s;
                            s<<i;
                            std::string t = s.str();
                           
                            glPushMatrix();
                            
                            double dd = 0.525*LOCATION_SIZE;

                            if (root->draw_metric){
                                dd = 0.525 * LOCATION_SIZE_METRIC;
                            }
                            
                            glTranslatef(current->locations[ii][i].loc.x+dd,current->locations[ii][i].loc.y+dd, current->floor_index[current->locations[ii][i].loc.floor]+dd);
                            glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
                            
                            if ((program->three_d_choice->value()>0)){
                                glRotatef(-ang_z,0,0,1);
                                glRotatef(-ang_x,1,0,0);
                            }
                            
                            for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) //(p ; *p; p++)
                            {
                                glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                            }
                            glPopMatrix();
                        
                        }
                        
                    }
                    glPopMatrix();
                }
                
                
            }
            
        }
    }
}

void draw_window::showsegments(){
   
   if ((root->draw_segments_label)&&(program->show_map->value())){
       
       if (program->three_d_choice->value()<2){
   
        gl_font(1, 14);
       if (root->bow){
           glColor3ub(0,0,0);
       }
       else{
           glColor3ub(200,200,200);
       }
       
        double pixel=(maxx-minx)/static_cast<double>(this->w());
        glLineWidth(1.0);
        
        
        for (unsigned int i=0;i<current->adj_list.size();i++){
            
            unsigned int i1=current->adj_list[i].start;
            unsigned int i2=current->adj_list[i].end;
            
            if ((i1<current->adj_list.size())&&(i2<current->adj_list.size())){
            
                if ((all_flrs)||((current->points[i1].floor==flr_choice)&&(current->points[i2].floor==flr_choice))){

                    
                    std::stringstream s;
                    s<<i;
                    std::string t=s.str();
                    
                    glPushMatrix();
                    
                    glTranslatef(0.5*(current->points[i1].x+current->points[i2].x)+0.1,0.5*(current->points[i1].y+current->points[i2].y)+0.1, 0.5*(current->points[i1].z+current->points[i2].z)+0.1);
                    glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
                    
                    
                    if (program->three_d_choice->value()>0){
                        glRotatef(-ang_z,0,0,1);
                        glRotatef(-ang_x,1,0,0);
                    }
                    
                    for(std::string::iterator c=t.begin() ; c!=t.end(); ++c)
                    {
                        glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
                    }
                    glPopMatrix();
                    
                }
            }
            
        }
         
      
    }//end show segments
   }
}

void draw_window::showscale(double fore_z){
              
    if ((!program->three_d_choice->value())&&(root->show_scale)){
        

        double pixel=(maxx-minx)/static_cast<double>(this->w()); //should be the same for y!!!

        double scale_a = 150*pixel;
        scale_a = pow(10.0,static_cast<int>(floor(log10(scale_a))));
        if (scale_a<35*pixel){
            scale_a *= 5;
        }
        else if(scale_a >250*pixel){
            scale_a *=0.5;
        }

        double xwrite,ywrite;
        
        xwrite =minx+ 20*pixel;
        ywrite = miny+ 20*pixel;
        glPushMatrix();
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(200,200,200);
        }
        glLineWidth(1.0);
        glBegin(GL_LINES);
        glVertex3d(xwrite,ywrite,fore_z);
        glVertex3d(xwrite+scale_a,ywrite,fore_z);
        
        glVertex3d(xwrite,ywrite-5*pixel,fore_z);
        glVertex3d(xwrite,ywrite+5*pixel,fore_z);
        
        
        glVertex3d(xwrite+0.2*scale_a,ywrite-3*pixel,fore_z);
        glVertex3d(xwrite+0.2*scale_a,ywrite+3*pixel,fore_z);
        
        glVertex3d(xwrite+0.4*scale_a,ywrite-3*pixel,fore_z);
        glVertex3d(xwrite+0.4*scale_a,ywrite+3*pixel,fore_z);
        
        glVertex3d(xwrite+0.6*scale_a,ywrite-3*pixel,fore_z);
        glVertex3d(xwrite+0.6*scale_a,ywrite+3*pixel,fore_z);
        
        glVertex3d(xwrite+0.8*scale_a,ywrite-3*pixel,fore_z);
        glVertex3d(xwrite+0.8*scale_a,ywrite+3*pixel,fore_z);
        
        glVertex3d(xwrite+scale_a,ywrite-5*pixel,fore_z);
        glVertex3d(xwrite+scale_a,ywrite+5*pixel,fore_z);
        
        
        //yaxis
        
        
        glVertex3d(xwrite,ywrite,fore_z);
        glVertex3d(xwrite,ywrite+scale_a,fore_z);
        
        glVertex3d(xwrite-5*pixel,ywrite,fore_z);
        glVertex3d(xwrite+5*pixel,ywrite,fore_z);
        
        
        glVertex3d(xwrite-3*pixel,ywrite+0.2*scale_a,fore_z);
        glVertex3d(xwrite+3*pixel,ywrite+0.2*scale_a,fore_z);
        
        glVertex3d(xwrite-3*pixel,ywrite+0.4*scale_a,fore_z);
        glVertex3d(xwrite+3*pixel,ywrite+0.4*scale_a,fore_z);
        
        glVertex3d(xwrite-3*pixel,ywrite+0.6*scale_a,fore_z);
        glVertex3d(xwrite+3*pixel,ywrite+0.6*scale_a,fore_z);
        
        glVertex3d(xwrite-3*pixel,ywrite+0.8*scale_a,fore_z);
        glVertex3d(xwrite+3*pixel,ywrite+0.8*scale_a,fore_z);
        
        glVertex3d(xwrite-5*pixel,ywrite+scale_a,fore_z);
        glVertex3d(xwrite+5*pixel,ywrite+scale_a,fore_z);
        
        
        glEnd();
        
        glPopMatrix();
        
        
        gl_font(1, 10);
        //const char *p;
        std::stringstream s;
        s.precision(2);

        s<<std::fixed<<0.0;
        xwrite =minx+ 10*pixel;
        ywrite = miny+ 5*pixel;
       
        std::string t= s.str();
        
        glPushMatrix();
        
        glTranslatef(xwrite,ywrite,0);
        glScalef(0.08*pixel, 0.08*pixel, 0.08*pixel);
        glLineWidth(1.0);
        glDisable(GL_DEPTH_TEST);
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
        
        
        
        s.str("");
        s<<std::setprecision(2);
    
        if (((maxx-minx)>10000)||((maxx-minx)<0.1)){
            s<<std::scientific;
        }
        else{
            s<<std::setprecision(2);
        }

        s<<scale_a;
        xwrite =scale_a+minx+ 10*pixel;
       
        t= s.str();
        
        glPushMatrix();
        
        glTranslatef(xwrite,ywrite,0); 
        glScalef(0.08*pixel, 0.08*pixel, 0.08*pixel);
        glLineWidth(1.0);
        
        glDisable(GL_DEPTH_TEST);
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
                
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
        
        
        
        
    }
    
    
}

void draw_window::showcolorscale(double fore_z){
    
    

    const double &min = program->root.current->min_plot_metric;
    const double &max = program->root.current->max_plot_metric;

    if ((root->color_scale)&&(program->show_map->value()!=0)){//start colour scale
        
        
        glPushMatrix();
        
        glBegin(GL_TRIANGLES);
        
        
        double sx=minx+0.9*(maxx-minx);
        double sy=miny+0.1*(maxy-miny);
        double dx=0.05*(maxy-miny),dy=0.8*(maxy-miny)/(256.0);
        
        gl_font(1, 14);
        
        
        for (int i=0;i<=255;i++){

            
            glColor3ub(i,(255-i),0);            
            
            glVertex3d(sx,sy+(i*dy),fore_z);
            glVertex3d(sx+dx,sy+(i*dy),fore_z);
            glVertex3d(sx,sy+((i+1)*dy),fore_z);
            glVertex3d(sx,sy+((i+1)*dy),fore_z);
            glVertex3d(sx+dx,sy+((i+1)*dy),fore_z);
            glVertex3d(sx+dx,sy+(i*dy),fore_z);
            
            
        }
        glEnd();
        
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(200,200,200);
        }
        
        std::stringstream s;
        s<<min;
        
        glRasterPos3f((sx+1.1*dx),sy+0.8*(maxy-miny),fore_z);
        s.str("");
        s<<max;
        
        double pixel=(maxx-minx)/static_cast<double>(this->w());
        
        glPushMatrix();
        
        s.str("");
        s<<std::setprecision(2)<<std::fixed;
        
        if ((min>1000)||(min<0.1)){
            s<<std::scientific;
        }
            
        
        s<<min;
        
        std::string t = s.str();
        
        
        glTranslatef((sx+1.1*dx),sy,fore_z);
        glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
        
        glLineWidth(1.0);
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) 
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        glPushMatrix();
        
        s.str("");
        s<<std::setprecision(2)<<std::fixed;
        
        if ((max>1000)||(max<0.1)){
            s<<std::scientific;
        }
        
        s<<max;
        
        t=s.str();
        
        
        glTranslatef((sx+1.1*dx),sy+0.8*(maxy-miny),fore_z);
        glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
        
        glLineWidth(1.0);
        
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) 
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        
        glPopMatrix();
        
    }
}

void draw_window::showrotatinglegend(double fore_z){
    
    
    
    
    if (program->three_d_choice->value()&&(root->show_scale)){ //3d rotating legend

        glPushMatrix();
        double dy=(maxy-miny)*0.05;
        double startx=2*dy+minx;
        double starty=2*dy+miny;
        double startz=fore_z-dy;
        
        double ang_xx,ang_zz;
     
        
        glTranslatef(startx,starty,startz); //translate back to corner
        if (program->three_d_choice->value()==3){
            glRotatef(ang_xfp,1,0,0);
            glRotatef(ang_zfp,0,0,1);
            ang_xx=(ang_xfp/(360.0))*2.0*M_PI;
            ang_zz=(ang_zfp/(360.0))*2.0*M_PI;
     
        }
        else{
            glRotatef(ang_x,1,0,0);
            glRotatef(ang_z,0,0,1);
            ang_xx=(ang_x/(360.0))*2.0*M_PI;
            ang_zz=(ang_z/(360.0))*2.0*M_PI;
     
        }
        glTranslatef(-startx,-starty,-startz); //translate to 0 in order to rotate
        glLineWidth(1.0);
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(200,200,200);
        }
        drawcubeedge(startx+0.5*dy,starty+0.5*dy,startz+0.5*dy,dy,1,1);
        dy*=1.25;
        glBegin(GL_LINES);
        glColor3ub(255,0,0);
        glVertex3d(startx,starty,startz);glVertex3d(startx+dy,starty,startz);
        glColor3ub(0,255,0);
        glVertex3d(startx,starty,startz);glVertex3d(startx,starty+dy,startz);
        glColor3ub(0,0,255);
        glVertex3d(startx,starty,startz);glVertex3d(startx,starty,startz+dy);
        glEnd();
        
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(200,200,200);
        }
 
        glPopMatrix();
        
        
        double pixel=(maxx-minx)/static_cast<double>(this->w());
        std::stringstream s;
        std::string t;
        
        
        
        
        glPushMatrix();
        
        s.str("");
        s<<"x";
        t = s.str();

        glTranslatef(startx+1.1*dy*cos(ang_zz),starty+1.1*dy*sin(ang_zz)*cos(ang_xx),startz+1.1*dy*sin(ang_zz)*sin(ang_xx)); //this one works
        glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
        glLineWidth(1.0);
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        glPushMatrix();
        s.str("");
        s<<"y";
        t = s.str();
        
        glTranslatef(startx-1.1*dy*sin(ang_zz),starty+1.1*dy*cos(ang_zz)*cos(ang_xx),startz+1.1*dy*cos(ang_zz)*sin(ang_xx));
        glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
        glLineWidth(1.0);
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c)
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        glPopMatrix();
        
        glPushMatrix();
        s.str("");
        s<<"z";
        t = s.str();
        
        glTranslatef(startx,starty-1.1*dy*sin(ang_xx),startz+1.1*dy*sin(ang_xx));
        glScalef(0.1*pixel, 0.1*pixel, 0.1*pixel);
        glLineWidth(1.0);
        
        for(std::string::iterator c=t.begin() ; c!=t.end(); ++c) 
        {
            glutStrokeCharacter(GLUT_STROKE_ROMAN, *c);
        }
        
        glPopMatrix();
        
    }//end 3d rotating legend
    
}