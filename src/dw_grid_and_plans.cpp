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

void draw_window::draw_background(double back_z){
    
    //START background grid: relative to orthographic coords so always there
    if (!program->three_d_choice->value()){ //only for 2D drawing
        glLineWidth(1.0);
        glBegin(GL_LINES);
        
        if (root->bow){
            glColor3ub(180,180,180);
        }
        else{
            //glColor3ub(76,76,76);
            glColor3ub(115,115,115);
        }


        double dx = 10.0;

        if (maxx-minx>600){
            int exponent = log10((maxx-minx)/50.0);
            dx = pow(10,exponent+1);
        }

        
        for (int i=0; i < 4 + static_cast<int>(maxx/dx) - static_cast<int>(minx/dx);i++){
            int ii= -1 + i + static_cast<int>(minx/dx);
            glVertex3d(ii*dx,miny-10,back_z);
            glVertex3d(ii*dx,maxy+10,back_z);
        }
        for (int j=0;j<4+static_cast<int>(maxy/dx) - static_cast<int>(miny/dx);j++){
            int jj= -1 + j + static_cast<int>(miny/dx);
            glVertex3d(minx-10,(jj-1)*dx,back_z);
            glVertex3d(maxx+10,(jj-1)*dx,back_z);
            
        }

        double dx_s = dx/5.0;
        
        
        
        if (root->bow){
            glColor3ub(215,215,215);
        }
        else{
           // glColor3ub(41,41,41);
            glColor3ub(80,80,80);
        }
        
        for (int i=0;i<4+static_cast<int>(maxx/dx_s) - static_cast<int>(minx/dx_s);i++){
            int ii= -1 + i + static_cast<int>(minx/dx_s);
            glVertex3d(ii*dx_s,miny-10,back_z);
            glVertex3d(ii*dx_s,maxy+10,back_z);
        }
        for (int j=0;j<4+static_cast<int>(maxy/dx_s) - static_cast<int>(miny/dx_s);j++){
            int jj=-1+j+static_cast<int>(miny/dx_s);
            glVertex3d(minx-10,(jj-1)*dx_s,back_z);
            glVertex3d(maxx+10,(jj-1)*dx_s,back_z);
            
        }
           
        glEnd();
    }
    else{
                
        //BEGIN RELATIVE BACKGROUND GRID
        glLineWidth(1.0);
        glBegin(GL_LINES);
        
        
        if (root->bow){
            glColor3ub(180,180,180);
        }
        else{
            //glColor3ub(76,76,76);
            glColor3ub(115,115,115);
        }

        double dx = 10.0;

        if (maxx-minx>600){
            int exponent = log10((maxx-minx)/50.0);
            dx = pow(10,exponent+1);
        }


        double midx = 0.5*(maxx_overall+minx_overall);
        double midy = 0.5*(maxy_overall+miny_overall);
        double size = std::max(fabs(maxx_overall-minx_overall),fabs(maxy_overall-miny_overall));
        size += std::max(dx,std::max(50.0,size*0.25));
        int n = std::round(size/dx);
        size = n*dx;
        double start_x = midx - 0.5*size;
        double end_x = midx + 0.5*size;
        double start_y = midy - 0.5*size;
        double end_y = midy + 0.5*size;

       
        for (int i = 0 ; i<=n ; i++){
            double xx = start_x + i*dx;
            glVertex3d(xx,start_y,minz-z_gap);
            glVertex3d(xx,end_y,minz-z_gap);
        }

        for (int i = 0 ; i<=n ; i++){   
            double yy = start_y + i*dx;
            glVertex3d(start_x,yy,minz-z_gap);
            glVertex3d(end_x,yy,minz-z_gap);
            
        }                       
  
        if (root->bow){
            glColor3ub(215,215,215);
        }
        else{
           // glColor3ub(41,41,41);
            glColor3ub(80,80,80);
        }

        double dx_s = dx/5.0;


        n*=5;

        for (int i = 0 ; i<=n ; i++){
            double xx = start_x + i*dx_s;
            glVertex3d(xx,start_y,minz-z_gap);
            glVertex3d(xx,end_y,minz-z_gap);
        }

        for (int i = 0 ; i<=n ; i++){
            double yy = start_y + i*dx_s;
            glVertex3d(start_x,yy,minz-z_gap);
            glVertex3d(end_x,yy,minz-z_gap);
            
        }
               
        glEnd();
        //END RELATIVE BACKGROUND GRID
    }
    
    //END background grid
    
}


void draw_window::draw_floorplan_gen(double depth){
    


    if (root->draw_floorplan){
        for (unsigned int i=0;i<current->floor_plans.size();i++){
            if ((all_flrs)||(static_cast<int>(i)==program->floor_choice->value()-1)){

                if (current->floor_plans[i].fast_plan){
                    draw_floorplan_floor_fast(depth,i);
                }
                else{
                    draw_floorplan_floor(depth,i);
                }

            }
        }

    }

}


void draw_window::draw_floorplan_floor_fast(double depth,int floor){

    int i = floor;

    glBegin(GL_LINES);

    glLineWidth(1.0);
    if (root->bow){
        glColor3ub(0,0,0);
    }
    else{
        glColor3ub(220,220,220);
    }

    
    old_plan_color = -1;//need to reset upon redrawing to instigate first color choice


    const plan &floor_plan = current->floor_plans[i];

    
    if (program->three_d_choice->value()){
        depth=-0.5*z_gap+0.01+current->floor_index[i];
    }


    for (unsigned int ii=0;ii<floor_plan.line_only_version.size();ii++){

        if (floor_plan.layers[ii]){
            
            
            for (const auto &line:floor_plan.line_only_version[ii]){

                if (!(program->three_d_choice->value())){
                    if ((((line.start.x>maxx)&&(line.end.x>maxx))  ||  ((line.start.x<minx)&&(line.end.x<minx))) ||  (((line.start.y>maxy)&&(line.end.y>maxy))  ||  ((line.start.y<miny)&&(line.end.y<miny)))){
                        continue;
                    }
                }

                if ((!use_splines)&&(line.is_spline)){
                    continue;
                }

                if (line.block>-1){
                    if (!floor_plan.blocks_active[line.block]){
                        continue;
                    }
                }


                if (plan_color){
                    set_color(line.color,line.block_color);
                }

                glVertex3d(line.start.x,line.start.y,depth);
                glVertex3d(line.end.x,  line.end.y,  depth);

            }

        }
    }

    glEnd();

}


void draw_window::draw_floorplan_floor(double depth, int floor){
    
                
    glLineWidth(1.0);
    if (root->bow){
        glColor3ub(0,0,0);
    }
    else{
        glColor3ub(200,200,200);
    }

    int i = floor;

    old_plan_color = -1;//need to reset upon redrawing to instigate first color choice


    const plan &floor_plan = current->floor_plans[i];
    
    const double &shift_x=floor_plan.shift_x;
    const double &shift_y=floor_plan.shift_y;
    
    if (program->three_d_choice->value()){
        depth=-z_gap+0.01+current->floor_index[i];
    }

    
    const std::vector<arc> &arcs=floor_plan.arcs;
    
    
    const std::vector<circle> &circles=floor_plan.circles;

    const std::vector<ellipse> &ellipses=floor_plan.ellipses;
    
    const std::vector<line_in> &lines=floor_plan.lines;
    
    const std::vector<int> &layers=floor_plan.layers;          
    
    const std::vector<block_entry> &blocks = floor_plan.blocks;
    

    const std::vector<spline> &splines=floor_plan.splines;

    glBegin(GL_LINES);

    for (const auto &x : ellipses){
        
        if (layers[x.layer]){
                                 
            DrawEllipse(x,depth,shift_x,shift_y,-1);
            
        }
        
    }
    

    for (const auto &x : arcs){
        
        if (layers[x.layer]){
            
            DrawArc(x,depth,shift_x,shift_y,-1);
        }
    }
    
    for (const auto &x : circles){
        
        if (layers[x.layer]){
                                                          
            DrawCircle(x,depth,shift_x,shift_y,-1);
            
        }
        
    }

    
    
    for (const auto &x : lines){
        
        if (layers[x.layer]){

            DrawLines(x,depth,shift_x,shift_y,-1);
            
        }
        
    }

    if (use_splines){
        for (const auto &x : splines){
            
            if (layers[x.layer]){
                                        
                DrawSpline(x,depth,shift_x,shift_y,-1);
                
            }
            
        }
    }
    
    glEnd();

    for (const auto &x : blocks){
        
        
        if (layers[x.layer]){ //<-- only layer check is at this leve - so all sub blocks behave according to their parent.

            double ix = x.insert_x+shift_x;
            double iy = x.insert_y+shift_y;

            glPushMatrix();

            glTranslated(ix,iy,0);
            
            glRotated(x.rotation,0,0,1);

            glScaled(x.scale_x,x.scale_y,1.0);
           
            glTranslated(-ix,-iy,0);

            draw_block_nested(&floor_plan,x,depth,0,ix,iy);
            
            glPopMatrix();
            
        }
        
    }           
 
}


//deprecated
void draw_window::draw_floorplan_fast(double depth){
    
    
    if (root->draw_floorplan){
        
        glBegin(GL_LINES);
     
        glLineWidth(1.0);
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(220,220,220);
        }
         
        
        for (unsigned int i=0;i<current->floor_plans.size();i++){
            

            if ((all_flrs)||(static_cast<int>(i)==program->floor_choice->value()-1)){
                
                old_plan_color = -1;//need to reset upon redrawing to instigate first color choice


                const plan &floor_plan = current->floor_plans[i];
            
                
                if (program->three_d_choice->value()){
                    depth=-z_gap+0.01+current->floor_index[i];
                }


                for (unsigned int ii=0;ii<floor_plan.layers.size();ii++){

                    if (floor_plan.layers[ii]){
                        
                        for (auto &x:floor_plan.line_only_version[ii]){

                            if ((!use_splines)&&(x.is_spline)){
                                continue;
                            }

                            if (x.block>-1){
                                if (!floor_plan.blocks_active[x.block]){
                                    continue;
                                }
                            }


                            if (plan_color){
                                set_color(x.color,x.block_color);
                            }

                            
                            glVertex3d(x.start.x,x.start.y,depth);
                            glVertex3d(x.end.x,x.end.y,depth);
                            
                        }

                    }
                }

            }
        }
        glEnd();
    }
}

//deprecated
void draw_window::draw_floorplan(double depth){
    
    
    if (root->draw_floorplan){
        
     
        glLineWidth(1.0);
        if (root->bow){
            glColor3ub(0,0,0);
        }
        else{
            glColor3ub(200,200,200);
        }
         
        
        for (unsigned int i=0;i<current->floor_plans.size();i++){

            if ((all_flrs)||(static_cast<int>(i)==program->floor_choice->value()-1)){
                
                old_plan_color = -1;//need to reset upon redrawing to instigate first color choice


                const plan &floor_plan = current->floor_plans[i];
                
                const double &shift_x=floor_plan.shift_x;
                const double &shift_y=floor_plan.shift_y;
                
                if (program->three_d_choice->value()){
                    depth=-z_gap+0.01+current->floor_index[i];
                }
 
                
                const std::vector<arc> &arcs=floor_plan.arcs;
                
                
                const std::vector<circle> &circles=floor_plan.circles;

                const std::vector<ellipse> &ellipses=floor_plan.ellipses;
                
                const std::vector<line_in> &lines=floor_plan.lines;
                
                const std::vector<int> &layers=floor_plan.layers;          
                
                const std::vector<block_entry> &blocks = floor_plan.blocks;
                

                const std::vector<spline> &splines=floor_plan.splines;

                glBegin(GL_LINES);

                for (const auto &x : ellipses){
                    
                    if (layers[x.layer]){
                                             
                        DrawEllipse(x,depth,shift_x,shift_y,-1);
                        
                    }
                    
                }
                
                for (const auto &x : arcs){
                    
                    if (layers[x.layer]){
                        
                        DrawArc(x,depth,shift_x,shift_y,-1);
                    }
                }
                
                for (const auto &x : circles){
                    
                    if (layers[x.layer]){
                                                                      
                        DrawCircle(x,depth,shift_x,shift_y,-1);
                        
                    }
                    
                }

                                
                for (const auto &x : lines){
                    
                    if (layers[x.layer]){

                        DrawLines(x,depth,shift_x,shift_y,-1);
                        
                    }
                    
                }

                if (use_splines){
                    for (const auto &x : splines){
                        
                        if (layers[x.layer]){
                                                    
                            DrawSpline(x,depth,shift_x,shift_y,-1);
                            
                        }
                        
                    }
                }

                glEnd();
                
                for (const auto &x : blocks){
                        
                    if (layers[x.layer]){

                        double ix = x.insert_x+shift_x;
                        double iy = x.insert_y+shift_y;

                        glPushMatrix();
    
                        glTranslated(ix,iy,0);
                        
                        glRotated(x.rotation,0,0,1);

                        glScaled(x.scale_x,x.scale_y,1.0);
                       
                        glTranslated(-ix,-iy,0);
                    
                        draw_block_nested(&floor_plan,x,depth,0,ix,iy);
                        
                        glPopMatrix();
                        
                    }
                    
                }
                                
            }
        }
    }
}


void draw_window::draw_block_nested(const plan* const floor_plan, const block_entry &block,const double &depth,int count,double &insert_x_tot,double &insert_y_tot){
    

    if (block.list_no<0){ //no record of the block
        return;
    }
    
    if (!floor_plan->blocks_active[block.list_no]){
        return;
    }
    
    const std::vector<block_entry> &blocks=floor_plan->block_definitions[block.list_no].nested_blocks; //use data sytructures in relevant block definition
    const std::vector<line_in> &lines=floor_plan->block_definitions[block.list_no].lines;
    const std::vector<arc> &arcs=floor_plan->block_definitions[block.list_no].arcs;
    const std::vector<circle> &circles=floor_plan->block_definitions[block.list_no].circles;
    const std::vector<ellipse> &ellipses=floor_plan->block_definitions[block.list_no].ellipses;
    const std::vector<spline> &splines=floor_plan->block_definitions[block.list_no].splines;
    double xs = insert_x_tot-floor_plan->block_definitions[block.list_no].start_x;//insert_x +ii*rs-start_x;
    double ys = insert_y_tot-floor_plan->block_definitions[block.list_no].start_y;//;insert_y + jj*cs-start_y;

    
    glBegin(GL_LINES);
    
    for (const auto &x : ellipses){
        DrawEllipse(x,depth,xs,ys,block.color);
    }

    for (const auto &x : arcs){
        DrawArc(x,depth,xs,ys,block.color);
    }
    
    for (const auto &x : circles){
        DrawCircle(x,depth,xs,ys,block.color);
    }
    
    for (const auto &x : lines){
        DrawLines(x,depth,xs,ys,block.color);        
    }
    
    if (use_splines){
        for (const auto &x : splines){
            DrawSpline(x,depth,xs,ys,block.color);
        }
    }

    glEnd();

    for (const auto &x:blocks){   

        double ix = x.insert_x+insert_x_tot;
        double iy = x.insert_y+insert_y_tot;

        glPushMatrix();
    
        glTranslated(ix,iy,0);
        
        glRotated(x.rotation,0,0,1);

        glScaled(x.scale_x,x.scale_y,1.0);
       
        glTranslated(-ix,-iy,0);

        draw_block_nested(floor_plan,x,depth,count+1,ix,iy);

        glPopMatrix();

    }
       
    
}


void draw_window::DrawLines(const line_in &data,const double &depth,const double &off_x,const double &off_y,const int block_color){
    
    if (plan_color){
        set_color(data.color,block_color);
    }

    glVertex3d(off_x+data.start.x,off_y+data.start.y,depth);
    glVertex3d(off_x+data.end.x,off_y+data.end.y,depth);
    
}

void draw_window::DrawSpline(const spline &data,const double &depth,const double &off_x,const double &off_y,const int block_color){
    
    if (plan_color){
        set_color(data.color,block_color);
    }

    int n = std::max(static_cast<int>(data.knots.size()),abs(static_cast<int>(40*data.approx_size/(maxx-minx))));

    double dt = data.t_range/static_cast<double>(n);

    double xo = data.get_x(data.t_start);
    double yo = data.get_y(data.t_start);
    
    
    for (int i=0;i<n;i++){
        double t = data.t_start + (i+1)*dt;
        double xn = data.get_x(t);
        double yn = data.get_y(t);    
        
        glVertex3d(off_x+xo,off_y+yo,depth);
        glVertex3d(off_x+xn,off_y+yn,depth);
        
        xo=xn;
        yo=yn;
    }

    
}


void draw_window::DrawEllipse(const ellipse &data,double const &depth,const double &off_x,const double &off_y,const int block_color)
{
    
    if (plan_color){
        set_color(data.color,block_color);
    }

    const double &cx=data.x;
    const double &cy=data.y;
    const double &r1=data.r1;
    const double &r2=data.r2;
    const double &cos_phi = data.cos_phi;
    const double &sin_phi = data.sin_phi;
    const double &ang1 = data.ang1;
    const double &ang2 = data.ang2;
    
   
    int num_segments=20+abs(static_cast<int>(40*(((ang2-ang1)*r1)/(maxx-minx))));

    double d_ang = (ang2-ang1)/(static_cast<double>(num_segments)-1.0);

    for (int i=0;i<num_segments-1;i++){
        
        double xx = cos(ang1+i*d_ang); 
        double yy = sin(ang1+i*d_ang);

        double x= (r1*xx*cos_phi-r2*yy*sin_phi);
        double y= (r1*xx*sin_phi+r2*yy*cos_phi);
        
        glVertex3d( off_x +(cx+x),off_y+(cy+y),depth);

        xx = cos(ang1+(i+1)*d_ang); 
        yy = sin(ang1+(i+1)*d_ang);

        x= (r1*xx*cos_phi-r2*yy*sin_phi);
        y= (r1*xx*sin_phi+r2*yy*cos_phi);
        
        glVertex3d( off_x +(cx+x),off_y+(cy+y),depth);
        
    }

}


void draw_window::DrawCircle(const circle &data,const double &depth,const double &off_x,const double &off_y,const int block_color)
{
    if (plan_color){
        set_color(data.color,block_color);
    }
    
    const double &cx=data.x;
    const double &cy=data.y;
    const double &r=data.r;
    
    
    const int num_segments=20+abs(static_cast<int>(40*((2.0*M_PI*r)/(maxx-minx))));
	double theta = 2.0 * 3.1415926 / static_cast<double>(num_segments);
	const double tangetial_factor = tan(theta);//calculate the tangential factor
    
	const double radial_factor = cos(theta);//calculate the radial factor
	
    double xo = r ;//we start at angle = 0
    double yo = 0;
    double x,y;
	
    for(int ii = 1; ii < num_segments; ii++)
	{

        //calculate the tangential vector
        //remember, the radial vector is (x, y)
        //to get the tangential vector we flip those coordinates and negate one of them
        
        //add the tangential vector
        
        x = xo - yo * tangetial_factor;
        y = yo + xo * tangetial_factor;
        
        //correct using the radial factor
        
        x *= radial_factor;
        y *= radial_factor;

        glVertex3d(off_x + (xo + cx), off_y + (yo + cy),depth);//output vertex
        glVertex3d(off_x + (x + cx), off_y + (y + cy),depth);//output vertex

        xo=x;
        yo=y;


	}
    //complete the loop
    glVertex3d(off_x + (r + cx), off_y + (0 + cy),depth);//output vertex
    glVertex3d(off_x + (x + cx), off_y + (y + cy),depth);//output vertex
}


void draw_window::DrawArc(const arc &data,const double &depth,const double &off_x,const double &off_y,const int block_color)
{
 
    if (plan_color){
        set_color(data.color,block_color);
    }

    const double &cx=data.x;
    const double &cy=data.y;
    const double &r=data.r;
    
    const double &ang1=data.ang1;
    const double &ang2=data.ang2;
    int num_segments=20+abs(static_cast<int>(40*(((ang2-ang1)*r)/(maxx-minx))));

    double d_ang = (ang2-ang1)/(static_cast<double>(num_segments)-1.0);

    double xo = r*cos(ang1);
    double yo = r*sin(ang1);

    for (int i=1;i<num_segments;i++){
        
        double x= r*cos(ang1+i*d_ang);
        double y= r*sin(ang1+i*d_ang);
        glVertex3d(off_x + (xo + cx), off_y + (yo + cy),depth);
        glVertex3d(off_x + (x + cx), off_y + (y + cy),depth);
        xo=x;
        yo=y;

    }
    

}