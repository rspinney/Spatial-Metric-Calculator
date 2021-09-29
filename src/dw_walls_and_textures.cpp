/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/floor.hpp"
#include "./include/ceiling.hpp"
#include "./include/wall.hpp"
#include "./include/draw_window.hpp"
#include "./include/base.hpp"

#ifndef M_PI
    #define M_PI 3.141592653589793238
#endif

#undef min
#undef max

void draw_window::load_textures(){

    
    //floor
       
    glGenTextures(1, &floor_texture);
    glBindTexture(GL_TEXTURE_2D, floor_texture);
  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, floor_width1, floor_height1, 0, GL_RGB, GL_FLOAT, floor_pixels1);
    glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, floor_width2, floor_height2, 0, GL_RGB, GL_FLOAT, floor_pixels2);
    glTexImage2D(GL_TEXTURE_2D, 2, GL_RGB, floor_width3, floor_height3, 0, GL_RGB, GL_FLOAT, floor_pixels3);
    glTexImage2D(GL_TEXTURE_2D, 3, GL_RGB, floor_width4, floor_height4, 0, GL_RGB, GL_FLOAT, floor_pixels4);
    glTexImage2D(GL_TEXTURE_2D, 4, GL_RGB, floor_width5, floor_height5, 0, GL_RGB, GL_FLOAT, floor_pixels5);
    glTexImage2D(GL_TEXTURE_2D, 5, GL_RGB, floor_width6, floor_height6, 0, GL_RGB, GL_FLOAT, floor_pixels6);
    glTexImage2D(GL_TEXTURE_2D, 6, GL_RGB, floor_width7, floor_height7, 0, GL_RGB, GL_FLOAT, floor_pixels7);
    glTexImage2D(GL_TEXTURE_2D, 7, GL_RGB, floor_width8, floor_height8, 0, GL_RGB, GL_FLOAT, floor_pixels8);
    glTexImage2D(GL_TEXTURE_2D, 8, GL_RGB, floor_width9, floor_height9, 0, GL_RGB, GL_FLOAT, floor_pixels9);
  
    
    //ceiling
      
    
    glGenTextures(1, &ceiling_texture);
    glBindTexture(GL_TEXTURE_2D, ceiling_texture);
  
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ceiling_width1, ceiling_height1, 0, GL_RGB, GL_FLOAT, ceiling_pixels1);
    glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, ceiling_width2, ceiling_height2, 0, GL_RGB, GL_FLOAT, ceiling_pixels2);
    glTexImage2D(GL_TEXTURE_2D, 2, GL_RGB, ceiling_width3, ceiling_height3, 0, GL_RGB, GL_FLOAT, ceiling_pixels3);
    glTexImage2D(GL_TEXTURE_2D, 3, GL_RGB, ceiling_width4, ceiling_height4, 0, GL_RGB, GL_FLOAT, ceiling_pixels4);
    glTexImage2D(GL_TEXTURE_2D, 4, GL_RGB, ceiling_width5, ceiling_height5, 0, GL_RGB, GL_FLOAT, ceiling_pixels5);
    glTexImage2D(GL_TEXTURE_2D, 5, GL_RGB, ceiling_width6, ceiling_height6, 0, GL_RGB, GL_FLOAT, ceiling_pixels6);
    glTexImage2D(GL_TEXTURE_2D, 6, GL_RGB, ceiling_width7, ceiling_height7, 0, GL_RGB, GL_FLOAT, ceiling_pixels7);
    glTexImage2D(GL_TEXTURE_2D, 7, GL_RGB, ceiling_width8, ceiling_height8, 0, GL_RGB, GL_FLOAT, ceiling_pixels8);
    glTexImage2D(GL_TEXTURE_2D, 8, GL_RGB, ceiling_width9, ceiling_height9, 0, GL_RGB, GL_FLOAT, ceiling_pixels9);
    
    //wall
    
    
    glGenTextures(1, &wall_texture);
    glBindTexture(GL_TEXTURE_2D, wall_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wall_width1, wall_height1, 0, GL_RGB, GL_FLOAT, wall_pixels1);
    glTexImage2D(GL_TEXTURE_2D, 1, GL_RGB, wall_width2, wall_height2, 0, GL_RGB, GL_FLOAT, wall_pixels2);
    glTexImage2D(GL_TEXTURE_2D, 2, GL_RGB, wall_width3, wall_height3, 0, GL_RGB, GL_FLOAT, wall_pixels3);
    glTexImage2D(GL_TEXTURE_2D, 3, GL_RGB, wall_width4, wall_height4, 0, GL_RGB, GL_FLOAT, wall_pixels4);
    glTexImage2D(GL_TEXTURE_2D, 4, GL_RGB, wall_width5, wall_height5, 0, GL_RGB, GL_FLOAT, wall_pixels5);
    glTexImage2D(GL_TEXTURE_2D, 5, GL_RGB, wall_width6, wall_height6, 0, GL_RGB, GL_FLOAT, wall_pixels6);
    glTexImage2D(GL_TEXTURE_2D, 6, GL_RGB, wall_width7, wall_height7, 0, GL_RGB, GL_FLOAT, wall_pixels7);
    glTexImage2D(GL_TEXTURE_2D, 7, GL_RGB, wall_width8, wall_height8, 0, GL_RGB, GL_FLOAT, wall_pixels8);
    glTexImage2D(GL_TEXTURE_2D, 8, GL_RGB, wall_width9, wall_height9, 0, GL_RGB, GL_FLOAT, wall_pixels9);
    
}

void draw_window::draw_floor(){
    
  
    if (program->three_d_choice->value()==3){
        
        double start_x = 10.0*( static_cast<int>((minx_overall-50)/10) );
        double start_y = 10.0*( static_cast<int>((miny_overall-50)/10) );
        double end_x = 10.0*( static_cast<int>((maxx_overall+50)/10) );
        double end_y = 10.0*( static_cast<int>((maxy_overall+50)/10) );
        
        
        if((end_x-start_x)<(end_y-start_y)){ //make square
            
        }
        else{
            
            double end_y_temp=end_y + 10 * (static_cast<int>((0.5*((end_x-start_x)-(end_y-start_y)))/10));
            double start_y_temp=start_y - 10 * (static_cast<int>((0.5*((end_x-start_x)-(end_y-start_y)))/10));
            end_y=end_y_temp;
            start_y=start_y_temp;
            
        }
            
        
        for (unsigned int i=0;i<current->floor_index.size();i++){
            
            if ((all_flrs)||(flr_choice==static_cast<int>(i))){

                double z=current->floor_index[i];
                
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D,floor_texture);
                
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
                
                glBegin(GL_TRIANGLES);
                glTexCoord2f(0,0);

                glVertex3d(start_x,start_y,z);
                glTexCoord2f(0,1000);

                glVertex3d(start_x,end_y,z);
                glTexCoord2f(1000,1000);

                glVertex3d(end_x,end_y,z);
                
                glTexCoord2f(0,0);

                glVertex3d(start_x,start_y,z);
                glTexCoord2f(1000,1000);

                glVertex3d(end_x,end_y,z);
                glTexCoord2f(1000,0);

                glVertex3d(end_x,start_y,z);
                
                
                glEnd();
                glDisable(GL_TEXTURE_2D);
            }
        }
    }
}

void draw_window::draw_ceiling(){
    
   
    if (program->three_d_choice->value()==3){
        


        
        double start_x = 10.0*(static_cast<int>((minx_overall-50)/10));
        double start_y = 10.0*(static_cast<int>((miny_overall-50)/10));
        double end_x = 10.0*(static_cast<int>((maxx_overall+50)/10));
        double end_y = 10.0*(static_cast<int>((maxy_overall+50)/10));
       
        if((end_x-start_x)<(end_y-start_y)){ //make square
                    
        }
        else{
            
            double end_y_temp=end_y+10*(static_cast<int>((0.5*((end_x-start_x)-(end_y-start_y)))/10));
            double start_y_temp=start_y-10*(static_cast<int>((0.5*((end_x-start_x)-(end_y-start_y)))/10));
            end_y=end_y_temp;
            start_y=start_y_temp;
            
        }
        
        
        for (unsigned int i=0;i<current->floor_index.size();i++){


            if ((all_flrs)||(flr_choice==static_cast<int>(i))){
                
                double z=current->floor_index[i]+2.95;
                
                
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D,ceiling_texture);
                glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
               
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
                glColor3ub(255,255,255);
                
                
                glBegin(GL_TRIANGLES);
                glTexCoord2f(0,0);
               
                glVertex3d(start_x,start_y,z);
                glTexCoord2f(0,1000);
               
                glVertex3d(start_x,end_y,z);
                glTexCoord2f(1000,1000);
               
                glVertex3d(end_x,end_y,z);
                
                glTexCoord2f(0,0);
               
                glVertex3d(start_x,start_y,z);
                glTexCoord2f(1000,1000);
               
                glVertex3d(end_x,end_y,z);
                glTexCoord2f(1000,0);
               
                glVertex3d(end_x,start_y,z);
                
                
                glEnd();
                         
                glDisable(GL_TEXTURE_2D);
            }
        }
    }
}

void draw_window::draw_walls(const std::vector<wall> &walls_sitting, const std::vector<wall> &walls_standing){
    

    if (!program->three_d_choice->value()){

        glLineWidth(1.51);
        glColor3ub(255,100,100);

        int transparent_old = 0;

        double dz = 0.5;

        int current_selected = 0;
        int highlighted = 0;

        if (program->show_walls_sitting->value()){
            glBegin(GL_LINES);

            for (unsigned int i=0;i<walls_sitting.size();i++){
                
                if ((walls_sitting[i].transparent==1)&&(!transparent_old)){
                    glColor3ub(100,255,100);
                    transparent_old = 1;
                }
                else if ((walls_sitting[i].transparent==0)&&(transparent_old)){
                    glColor3ub(255,100,100);
                    transparent_old = 0;
                }
                
                if ((all_flrs)||(walls_sitting[i].start.floor==flr_choice)){
                    
                    if ((selector==10)&&(static_cast<int>(i)==selected)){ //highlighting
                        
                        glColor3ub(255,255,0);

                        dz=0.6;
                        highlighted = 1;
                        
                    }

                    if ((selector==10)&&(group_selection_list.size()>0)){

                        if (static_cast<int>(i) == group_selection_list[current_selected]){
                        
                            glColor3ub(255,255,0);
                            dz=0.6;
                            highlighted =1;

                            if (current_selected < static_cast<int>(group_selection_list.size())-1){
                                current_selected++;
                            }
                        
                        }
                    }

                    //glBegin(GL_LINES);
                    glVertex3d(walls_sitting[i].start.x,walls_sitting[i].start.y,walls_sitting[i].start.z+dz);
                    glVertex3d(walls_sitting[i].end.x,walls_sitting[i].end.y,walls_sitting[i].end.z+dz);
                    //glEnd();

                    if (highlighted){//((selector==10)&&(static_cast<int>(i)==selected)){ //highlighting
                        glColor3ub(255,100,100);
                        transparent_old =0;                        
                        dz=0.5;
                        highlighted = 0;
                    }
                    
                }
            }
            
            glEnd();
        }

        glColor3ub(255,100,100);
        transparent_old = 0;
        highlighted = 0;

        if (program->show_walls_standing->value()){
            glBegin(GL_LINES);///////

            for (unsigned int i=0;i<walls_standing.size();i++){
                
                if ((walls_standing[i].transparent==1)&&(!transparent_old)){
                    glColor3ub(100,255,100);
                    transparent_old = 1;
                }
                else if ((walls_standing[i].transparent==0)&&(transparent_old)){
                    glColor3ub(255,100,100);
                    transparent_old = 0;
                }
                                
                if ((all_flrs)||(walls_standing[i].start.floor==flr_choice)){
                    
                    if ((selector==11)&&(static_cast<int>(i)==selected)){ //highlighting
                        glColor3ub(255,255,0);
                        dz=0.6;
                        highlighted = 1;
                        
                    }

                    if ((selector==11)&&(group_selection_list.size()>0)){

                        if (static_cast<int>(i) == group_selection_list[current_selected]){
                        
                            glColor3ub(255,255,0);
                            dz=0.6;
                            highlighted = 1;

                            if (current_selected < static_cast<int>(group_selection_list.size())-1){
                                current_selected++;
                            }
                        
                        }
                    }
                    //glBegin(GL_LINES);
                    glVertex3d(walls_standing[i].start.x,walls_standing[i].start.y,walls_standing[i].start.z+dz);
                    glVertex3d(walls_standing[i].end.x,walls_standing[i].end.y,walls_standing[i].end.z+dz);
                    //glEnd();

                    if (highlighted){//((selector==11)&&(static_cast<int>(i)==selected)){ //highlighting
                        glColor3ub(255,100,100);
                        transparent_old =0;
                        dz=0.5;        
                        highlighted=0;                
                    }
                }
            }

            glEnd();
        }

        if (set_first){ //draggable wall
       
            if ((all_flrs)||(temp_wall.start.floor==flr_choice)){
                
                double xx= x_mouse_coord;
                double yy= y_mouse_coord;
                
                if (snapon){  //snapon overwrite
                    xx=snapx;
                    yy=snapy;
                }                

                glColor3ub(20,235,40);
                
                
                glBegin(GL_LINES);
                glVertex3d(temp_wall.start.x,temp_wall.start.y,temp_wall.start.z+0.5);
                glVertex3d(xx,yy,temp_wall.start.z+0.5);
                glEnd();
            }
            
        }

    }
    else if ((program->show_walls_sitting->value())||(program->show_walls_standing->value())){
        
        GLfloat light_ambient[] = { 0.12,0.12,0.12, 1.0 };
        GLfloat light_diffuse[] = {1.0,1.0,1.0,1.0};
        GLfloat light_specular[] = {1.0,1.0,1.0,1.0};
        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

        glColor3ub(100,100,100);
        glColor3ub(255,255,255);        
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);

        glBindTexture(GL_TEXTURE_2D,wall_texture);
        
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glEnable(GL_TEXTURE_2D);
        

        if (root->transparent_walls){
            glEnable( GL_BLEND );
            
            if (root->bow){
                glBlendFunc(GL_ZERO,GL_SRC_COLOR);
                glColor3f((200.0/256.0),(200.0/256.0),(200.0/256.0));
    
            }
            else{
                glBlendFunc(GL_ONE,GL_ONE);
                glColor3f((10.0/256.0),(10.0/256.0),(10.0/256.0));
            }
         
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            
            glDepthMask(GL_FALSE);
            
            
            glDisable(GL_TEXTURE_2D);
        }

        glBegin(GL_TRIANGLES);

        for (int wo = 0;wo<2;wo++){

            const std::vector<wall> *w_ptr;
            double height = 0.0;
            Fl_Check_Button *show_option;
            
            if (wo==0){
                w_ptr = &walls_standing;  
                height =3.0;
                show_option = program->show_walls_standing;
            } 
            else{
                w_ptr = &walls_sitting;  
                height =1.0;
                show_option = program->show_walls_sitting;
            }

            const std::vector<wall> &w = *w_ptr;

            if ((show_option->value())){                
                
                for (size_t i=0; i<w.size();++i){
                    if ((all_flrs)||(w[i].start.floor==flr_choice)){
                        if ((w[i].transparent==1)&&(!root->transparent_walls)){

                            glEnd();
                            glEnable( GL_BLEND );
                            
                            if (root->bow){
                                glBlendFunc(GL_ZERO,GL_SRC_COLOR);
                                glColor3f((200.0/256.0),(200.0/256.0),(200.0/256.0));
                    
                            }
                            else{
                                glBlendFunc(GL_ONE,GL_ONE);
                                glColor3f((10.0/256.0),(10.0/256.0),(10.0/256.0));
                            }
                         
                            glDisable(GL_LIGHTING);
                            glDisable(GL_LIGHT0);
                            glDisable(GL_LIGHT1);
                            
                            glDepthMask(GL_FALSE);
                            
                            
                            glDisable(GL_TEXTURE_2D);
                            glBegin(GL_TRIANGLES);
                            
                        }

                        for (int tri =0;tri < 2;tri++){
                            
                            double ax=w[i].end.x-w[i].start.x;
                            double ay=w[i].end.y-w[i].start.y;
                            double bz=height+0.1;

                            double cx=ay*bz;
                            double cy=-(ax*bz);
                            double cz = 0.0;
                            
                            double norm=sqrt((cx*cx)+(cy*cy));

                            cx/=norm;
                            cy/=norm;                           
                            
                            double length = std::max(fabs(ax),fabs(ay));
                            length=length/0.05;

                            if (tri==0){
                                                                
                                glNormal3f(-cx,-cy,-cz);

                                //glBegin(GL_TRIANGLES);

                                glTexCoord2f(0,0);
                                glVertex3d(w[i].start.x,w[i].start.y,w[i].start.z-0.1);
                                glTexCoord2f(length,0);
                                glVertex3d(w[i].end.x,w[i].end.y,w[i].end.z-0.1);
                                glTexCoord2f(0,60);
                                glVertex3d(w[i].start.x,w[i].start.y,w[i].start.z+height);

                                //glEnd();
                            }
                            else{

                                glNormal3f(-cx,-cy,-cz);

                                //glBegin(GL_TRIANGLES);

                                glTexCoord2f(0,60);
                                glVertex3d(w[i].start.x,w[i].start.y,w[i].start.z+height);
                                glTexCoord2f(length,0);
                                glVertex3d(w[i].end.x,w[i].end.y,w[i].end.z-0.1);
                                glTexCoord2f(length,60);
                                glVertex3d(w[i].end.x,w[i].end.y,w[i].end.z+height);

                                //glEnd();  
                            }

                        }

                        if ((w[i].transparent==1)&&(!root->transparent_walls)){
                            glEnd();
                            glDisable(GL_BLEND);
                        
                            glDepthMask(GL_TRUE);
                            glEnable(GL_LIGHTING);
                            glEnable(GL_LIGHT0);
                            glEnable(GL_LIGHT1);
                            glEnable(GL_TEXTURE_2D);
                        
                            glBegin(GL_TRIANGLES);

                            glColor3ub(255,255,255);
                        }
                    }
                }
            }
                 
        }

        glEnd();
        
        glDisable(GL_BLEND);

        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_TEXTURE_2D);

        if (root->transparent_walls){
            glDepthMask(GL_TRUE);
        }

    }
   
}