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

void draw_window::translate_frustum(){

    if ((program->three_d_choice->value()==2)){ //EYE SPACE TRANSFORM
        glTranslatef(-0.5*(minx+maxx),-0.5*(miny+maxy),-z_dist-minz);  //in conjuction with frustum
        
    }
    else if(program->three_d_choice->value()==3){
        
            glTranslatef(-xfp,-yfp,-zfp);
                        
            xfr=xfp;
            yfr=yfp;
            zfr=zfp;
            
            axx=ang_xfp;
            azz=ang_zfp;
            
    }
    //END FRUSTRUM PLACEMENT
}

void draw_window::set_rotation(double zval){
    
    
    if (program->three_d_choice->value()==3){
     
        glTranslatef(xfr,yfr,zfr);
        glRotatef(axx,1,0,0);
        glRotatef(azz,0,0,1);
        glTranslatef(-xfr,-yfr,-zfr);
                
    }
    
    else if (program->three_d_choice->value()){ //rotate
        
        glTranslatef(current->mid_x,current->mid_y,zval);
        glRotatef(ang_x,1,0,0);
        glRotatef(ang_z,0,0,1);
        glTranslatef(-current->mid_x,-current->mid_y,-zval);
        
    }
}

void draw_window::setzoom(){
    
    
    double ppsx,ppsy;
    
    if (!first_draw){ //first draw will set up initial values
        
        if (zoom){ //mouse has been scrolled
            
            
            if (zoom_mag>100)zoom_mag=20;  //limit zooming from the mouse
            if (zoom_mag<-100)zoom_mag=-20;
            
         
            double xmin,xmax,ymin,ymax;
            
            double dx=0,dy=0;
            
           
            xmin=minx-0.5*0.05*(static_cast<double>(zoom_mag))*(maxx-minx); //place holder versions of ortho coordinates
            xmax=maxx+0.5*0.05*(static_cast<double>(zoom_mag))*(maxx-minx); // has to take into account changes relative to window size/shape too
            ymin=miny-0.5*0.05*(static_cast<double>(zoom_mag))*(maxy-miny);
            ymax=maxy+0.5*0.05*(static_cast<double>(zoom_mag))*(maxy-miny);
            
           
            if(program->three_d_choice->value()<2){ //dont try to correct if using perspective
                dx=frac_x*((maxx-xmax)-(minx-xmin))-(xmin-minx); //correction to keep point in same place as mouse (ish)
                dy=frac_y*((maxy-ymax)-(miny-ymin))-(ymin-miny);
            }
            
            
            xmin+=dx;  //apply correction to placeholder
            xmax+=dx;
            ymin+=dy;
            ymax+=dy;
            
            if (new_zoom){
                               
                xmin=0.5*(maxx_overall+minx_overall) - (1.0/new_zoom)*(0.5*(maxx_overall+minx_overall)-minx_overall);
                xmax=0.5*(maxx_overall+minx_overall) + (1.0/new_zoom)*(maxx_overall-0.5*(maxx_overall+minx_overall));
                new_zoom=0;
            
            }
            
            if (((xmax-xmin)>std::max(5e-3,0.001*(maxx-minx)))&&((xmax-xmin)<10*(maxx_overall-minx_overall))){
                
                minx=xmin; //set actual values to the placeholders
                maxx=xmax;
                miny=ymin;
                maxy=ymax;

            }
            
            if(program->three_d_choice->value()<2){
                
                if((xmax-xmin)>10*(maxx_overall-minx_overall)){
                                    
                    double xmid = current->mid_x;
                    double dx=0.5*((maxx-xmid)-(xmid-minx));
                    minx-=0.5*dx;
                    maxx-=0.5*dx;
                    
                    double ymid = current->mid_y;
                    double dy=0.5*((maxy-ymid)-(ymid-miny));
                    miny-=0.5*dy;
                    maxy-=0.5*dy;
                    
                }
            }
            
            zoom=0;drag=0; //finished zooming
        }
        else if (drag){
            
            minx=minx_down-(xdragdown-xdown)*(maxx_down-minx_down)/(this->w());
            maxx=maxx_down-(xdragdown-xdown)*(maxx_down-minx_down)/(this->w());
            miny=miny_down+(ydragdown-ydown)*(maxy_down-miny_down)/(this->h());
            maxy=maxy_down+(ydragdown-ydown)*(maxy_down-miny_down)/(this->h());

            drag=0;zoom=0; //finished dragging
        }
        
        
        
    }
    else{ //first draw
        
        
        maxx = 103;
        minx = -103;
        maxx_overall = 103;
        minx_overall = -103;
        
        maxy = 103;
        miny = -103;
        maxy_overall = 103;
        miny_overall = -103;
        
        
        minz = 0;
        
        double maxx_t=-9e99,minx_t=9e99,maxy_t=-9e99,miny_t=9e99;
        double minz_t=9e99,maxz_t=-9e99; //find extremes of current->points
        
        for (unsigned int i=0;i<current->points.size();i++){

            
            if (current->points[i].x<minx_t){
                minx_t=current->points[i].x;
            }
            if (current->points[i].y<miny_t){
                miny_t=current->points[i].y;
            }
            if (current->points[i].x>maxx_t){
                maxx_t=current->points[i].x;
            }
            if (current->points[i].y>maxy_t){
                maxy_t=current->points[i].y;
            }
            if (current->points[i].z<minz_t){
                minz_t=current->points[i].z;
            }
            if (current->points[i].z>maxz_t){
                maxz_t=current->points[i].z;
            }
        }
        
       

        for (unsigned int j=0;j<current->floor_plans.size();j++){
            
            plan floor_plan;
            floor_plan=current->floor_plans[j];
            std::vector<int> layers = floor_plan.layers;

            if (floor_plan.fast_plan){
                 for (unsigned int ii=0;ii<floor_plan.line_only_version.size();ii++){

                    if (floor_plan.layers[ii]){
                        
                        
                        for (const auto &line:floor_plan.line_only_version[ii]){

                            if (line.block>-1){
                                if (!floor_plan.blocks_active[line.block]){
                                    continue;
                                }
                            }

                            if (line.start.x<minx_t) minx_t=line.start.x;
                            if (line.end.x<minx_t) minx_t=line.end.x;
                            if (line.start.x>maxx_t) maxx_t=line.start.x;
                            if (line.end.x>maxx_t) maxx_t=line.end.x;

                            if (line.start.y<miny_t) miny_t=line.start.y;
                            if (line.end.y<miny_t) miny_t=line.end.y;
                            if (line.start.y>maxy_t) maxy_t=line.start.y;
                            if (line.end.y>maxy_t) maxy_t=line.end.y;

                        }

                    }
                }
            }
            else{
                for (unsigned int i=0;i<floor_plan.lines.size();i++){
                
                    if (layers[floor_plan.lines[i].layer]){
                        
                        if (floor_plan.lines[i].start.x+floor_plan.shift_x<minx_t){
                            minx_t=floor_plan.lines[i].start.x+floor_plan.shift_x;
                        }
                        if (floor_plan.lines[i].start.x+floor_plan.shift_x>maxx_t){
                            maxx_t=floor_plan.lines[i].start.x+floor_plan.shift_x;
                        }
                        if (floor_plan.lines[i].start.y+floor_plan.shift_y<miny_t){
                            miny_t=floor_plan.lines[i].start.y+floor_plan.shift_y;
                        }
                        if (floor_plan.lines[i].start.y+floor_plan.shift_y>maxy_t){
                            maxy_t=floor_plan.lines[i].start.y+floor_plan.shift_y;
                        }
                        if (floor_plan.lines[i].start.z<minz_t){
                            minz_t=floor_plan.lines[i].start.z;
                        }
                        if (floor_plan.lines[i].start.z>maxz_t){
                            maxz_t=floor_plan.lines[i].start.z;
                        }
                        
                        if (floor_plan.lines[i].end.x+floor_plan.shift_x<minx_t){
                            minx_t=floor_plan.lines[i].start.x+floor_plan.shift_x;
                        }
                        if (floor_plan.lines[i].end.x+floor_plan.shift_x>maxx_t){
                            maxx_t=floor_plan.lines[i].start.x+floor_plan.shift_x;
                        }
                        if (floor_plan.lines[i].end.y+floor_plan.shift_y<miny_t){
                            miny_t=floor_plan.lines[i].start.y+floor_plan.shift_y;
                        }
                        if (floor_plan.lines[i].end.y+floor_plan.shift_y>maxy_t){
                            maxy_t=floor_plan.lines[i].start.y+floor_plan.shift_y;
                        }
                        if (floor_plan.lines[i].end.z<minz_t){
                            minz_t=floor_plan.lines[i].start.z;
                        }
                        if (floor_plan.lines[i].end.z>maxz_t){
                            maxz_t=floor_plan.lines[i].start.z;
                        }
                        
                    }
                    
                }
            }
            
            
        }

        
        /* //removing this as project wide contraints take priority
        
        if (!program->floor_choice->value()){ // if all floors

            <---above code went here.
        }
        else{//specific flor

            int j = program->floor_choice->value()-1;
                
            plan floor_plan;
            floor_plan=current->floor_plans[j];
            std::vector<int> layers = floor_plan.layers;
            

            if (floor_plan.fast_plan){
                 for (unsigned int ii=0;ii<floor_plan.line_only_version.size();ii++){

                    if (floor_plan.layers[ii]){
                        
                        
                        for (const auto &line:floor_plan.line_only_version[ii]){

                            if (line.start.x<minx_t) minx_t=line.start.x;
                            if (line.end.x<minx_t) minx_t=line.end.x;
                            if (line.start.x>maxx_t) maxx_t=line.start.x;
                            if (line.end.x>maxx_t) maxx_t=line.end.x;

                            if (line.start.y<miny_t) miny_t=line.start.y;
                            if (line.end.y<miny_t) miny_t=line.end.y;
                            if (line.start.y>maxy_t) maxy_t=line.start.y;
                            if (line.end.y>maxy_t) maxy_t=line.end.y;

                        }

                    }
                }
            }
            else{
                for (unsigned int i=0;i<floor_plan.lines.size();i++){
                    
                    if (layers[floor_plan.lines[i].layer]){
                        
                        if (floor_plan.lines[i].start.x+floor_plan.shift_x<minx_t){
                            minx_t=floor_plan.lines[i].start.x+floor_plan.shift_x;
                        }
                        if (floor_plan.lines[i].start.x+floor_plan.shift_x>maxx_t){
                            maxx_t=floor_plan.lines[i].start.x+floor_plan.shift_x;
                        }
                        if (floor_plan.lines[i].start.y+floor_plan.shift_y<miny_t){
                            miny_t=floor_plan.lines[i].start.y+floor_plan.shift_y;
                        }
                        if (floor_plan.lines[i].start.y+floor_plan.shift_y>maxy_t){
                            maxy_t=floor_plan.lines[i].start.y+floor_plan.shift_y;
                        }
                        if (floor_plan.lines[i].start.z<minz_t){
                            minz_t=floor_plan.lines[i].start.z;
                        }
                        if (floor_plan.lines[i].start.z>maxz_t){
                            maxz_t=floor_plan.lines[i].start.z;
                        }
                        
                        if (floor_plan.lines[i].end.x+floor_plan.shift_x<minx_t){
                            minx_t=floor_plan.lines[i].start.x+floor_plan.shift_x;
                        }
                        if (floor_plan.lines[i].end.x+floor_plan.shift_x>maxx_t){
                            maxx_t=floor_plan.lines[i].start.x+floor_plan.shift_x;
                        }
                        if (floor_plan.lines[i].end.y+floor_plan.shift_y<miny_t){
                            miny_t=floor_plan.lines[i].start.y+floor_plan.shift_y;
                        }
                        if (floor_plan.lines[i].end.y+floor_plan.shift_y>maxy_t){
                            maxy_t=floor_plan.lines[i].start.y+floor_plan.shift_y;
                        }
                        if (floor_plan.lines[i].end.z<minz_t){
                            minz_t=floor_plan.lines[i].start.z;
                        }
                        if (floor_plan.lines[i].end.z>maxz_t){
                            maxz_t=floor_plan.lines[i].start.z;
                        }
                        
                    }
                }
            }

        }
        */
        
        
        minx_t-=5; //pad the current->points
        miny_t-=5;
        maxx_t+=5;
        maxy_t+=5;
        minz_t-=5;
        maxz_t+=5;
        
        
        
        
        ppsx=(maxx_t-minx_t)/w(); //find metres per pixel
        ppsy=(maxy_t-miny_t)/h();
        
        
        double dif_x=0,dif_y=0; //do the maths...
        if (ppsx>ppsy){
            dif_y=0.5*(ppsx*h()-(maxy_t-miny_t));
        }
        if (ppsy>ppsx){
            dif_x=0.5*(ppsy*w()-(maxx_t-minx_t));
            
        }
        
        
        minx_t=minx_t-dif_x; //apply corrections
        maxx_t=maxx_t+dif_x;
        miny_t=miny_t-dif_y;
        maxy_t=maxy_t+dif_y;

        z_dist=1.5*(maxx_overall-minx_overall);
        
        
        if (minx_t<9e98){ //so we default if we havent read anything
            
            minx_overall=minx_t; //remember the limits of zoom one level
            maxx_overall=maxx_t;
            miny_overall=miny_t;
            maxy_overall=maxy_t;
            
            minx=minx_t;
            maxx=maxx_t;
            miny=miny_t;
            maxy=maxy_t;
            
            current->mid_x=0.5*(minx_overall+maxx_overall);
            current->mid_y=0.5*(miny_overall+maxy_overall);
            current->mid_z=0.5*(minz_t+maxz_t);
                       
        }
        
        z_dist=1.5*(maxx_overall-minx_overall);
        
        w_initial=w();
        h_initial=h();
        
        
        ang_x=0;
        ang_y=0;
        ang_z=0;
        zoom_level=1.0; //zoom level defined to be 1.
        
        
    }
    
    //back to main loop: deal with any resizing by user manipulation of window
    
    
    ppsx=(maxx-minx)/w();
    ppsy=(maxy-miny)/h();
    
    
    double dif_x=0,dif_y=0;
    
    if (ppsy>ppsx){
        dif_y=0.5*(ppsx*h()-(maxy-miny)); //this time only ever change y: otherwise keeps getting smaller with each resize!
    }
    if (ppsy<ppsx){
        dif_y=0.5*(ppsx*h()-(maxy-miny));
    }
    
    
    
    minx=minx-dif_x;
    maxx=maxx+dif_x;
    miny=miny-dif_y;
    maxy=maxy+dif_y;
    
    
}

void draw_window::projection(const double& back_z_ext,const double& fore_z_ext){

    setzoom(); // figure out what projection to setup

    //START PROJECTION SETUP
    
    double xplane=(maxx-minx),yplane=(maxy-miny);
    
    if ((program->three_d_choice->value()==2)){
        glFrustum(-0.001*(0.5*xplane),0.001*(0.5*xplane),-0.001*(0.5*yplane),0.001*(0.5*yplane),0.001*(maxx_overall-minx_overall),std::max(5000.0,50*(maxx_overall-minx_overall)));
    }
    else if (program->three_d_choice->value()==3){
        glFrustum(-0.00001*(0.5*xplane),0.00001*(0.5*xplane),-0.00001*(0.5*yplane),0.00001*(0.5*yplane),0.00001*(maxx_overall-minx_overall),4000000);
    }
    else{
        glOrtho(minx,maxx,miny,maxy,back_z_ext,fore_z_ext); //set the orthographic current->matrix (back_z creates room for rotating legend
    }
    //END PROJECTION SET UP
}

void draw_window::lighting(){
    
    GLfloat mat_ambient[]    = { 1.0, 1.0, 1.0, 1.0 };  // RGBA
    GLfloat mat_diffuse[]    = { 1.0, 1.0, 1.0, 1.0 };  // RGBA
    GLfloat mat_specular[]   = { 1.0, 1.0, 1.0, 1.0 };  // RGBA


    GLfloat light_position[4];
    light_position[0]=minx_overall-65.0;
    light_position[1]=maxy_overall +10.0;
    light_position[2]=100.0;
    light_position[3]=1.0;


    glShadeModel(GL_SMOOTH);
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular);
    glMaterialf(GL_FRONT_AND_BACK,  GL_SHININESS, 20.0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    
    GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
    GLfloat light_diffuse[] = { 0.5, 0.4, 0.4, 1.0 };
    GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };

    GLfloat light_position1[4];
    light_position1[0]=maxx_overall+65.0;
    light_position1[1]=miny_overall -10.0;
    light_position1[2]=100.0;
    light_position1[3]=1.0;
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);

    
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    
    //end lighting
}

void draw_window::reset_perspective(const double&back_z_ext,const double&fore_z_ext){
    if (program->three_d_choice->value()==2){ //reset perspective for anotations
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(minx,maxx,miny,maxy,back_z_ext,fore_z_ext); 
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
    }
    else if (program->three_d_choice->value()==3){ //reset perspective for anotations
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(minx,maxx,miny,maxy,back_z_ext,fore_z_ext); 
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
    }
}
