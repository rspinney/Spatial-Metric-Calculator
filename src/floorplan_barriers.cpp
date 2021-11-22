/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/base.hpp"

void base::addarc(const arc data,std::vector<wall>* walls,int floor, int transparent,const container* root,double off_x,double off_y,matrix_2d mat)
{
    
    const double &shift_x = root->current->floor_plans[floor].shift_x;
    const double &shift_y = root->current->floor_plans[floor].shift_y;

    const double &cx=data.x;
    const double &cy=data.y;
    const double &r=data.r;    
    
    const double &ang1=data.ang1;
    const double &ang2=data.ang2;

    int num_segments=20+abs(static_cast<int>(((10.0*M_PI*r))));
    
    
    double xo= r*cos(ang1); 
    double yo= r*sin(ang1);

    auto xy = mat.mult_vec({xo + cx +off_x,yo + cy + off_y});
    xo = xy[0];
    yo = xy[1];
    
    for (int i=0;i<num_segments;i++){
        
        double x= r*cos(ang1+i*(ang2-ang1)/(static_cast<double>(num_segments)-1.0)); 
        double y= r*sin(ang1+i*(ang2-ang1)/(static_cast<double>(num_segments)-1.0));

        auto xy = mat.mult_vec({x+cx+off_x,y+cy+off_y});
        double xn = xy[0];
        double yn = xy[1];


        wall temp;
        
        temp.start.x=xo+shift_x;
        temp.end.x=xn+shift_x;
        temp.start.y=yo+shift_y;
        temp.end.y=yn+shift_y;
        temp.start.z=root->current->floor_index[floor];
        temp.end.z=root->current->floor_index[floor];
        temp.start.floor=floor;
        temp.end.floor=floor;
        temp.transparent=transparent;
        
        walls->push_back(temp);

        xo=xn;yo=yn;

    }
    
}



void base::addcircle(const circle data,std::vector<wall>* walls,int floor, int transparent,const container* root,double off_x,double off_y,matrix_2d mat)
{
    const double &shift_x = root->current->floor_plans[floor].shift_x;
    const double &shift_y = root->current->floor_plans[floor].shift_y;

    double cx=data.x;
    double cy=data.y;
    double r=data.r;
    
    int num_segments=20+abs(static_cast<int>(((10.0*M_PI*r))));
    float theta = 2 * 3.1415926 / static_cast<float>(num_segments);
    float tangetial_factor = tanf(theta);//calculate the tangential factor
    
    float radial_factor = cosf(theta);//calculate the radial factor
    
    float x = r ;//we start at angle = 0
    
    float y = 0;
    

    float tx = -y;
    float ty = x;
    
    //add the tangential vector
    
    x += tx * tangetial_factor;
    y += ty * tangetial_factor;
    
    //correct using the radial factor
    
    x *= radial_factor;
    y *= radial_factor;

    auto xy = mat.mult_vec({x+cx+off_x,y+cy+off_y});
    double xo = xy[0];
    double yo = xy[1];

    for(int ii = 0; ii < num_segments; ii++)
    {
               
        //calculate the tangential vector
        //remember, the radial vector is (x, y)
        //to get the tangential vector we flip those coordinates and negate one of them
        
        float tx = -y;
        float ty = x;
        
        //add the tangential vector
        
        x += tx * tangetial_factor;
        y += ty * tangetial_factor;
        
        //correct using the radial factor
        
        x *= radial_factor;
        y *= radial_factor;


        auto xy = mat.mult_vec({x+cx+off_x,y+cy+off_y});
        double xn = xy[0];
        double yn = xy[1];

        wall temp;
        
        temp.start.x=xo+shift_x;
        temp.end.x=xn+shift_x;
        temp.start.y=yo+shift_y;
        temp.end.y=yn+shift_y;
        temp.start.z=root->current->floor_index[floor];
        temp.end.z=root->current->floor_index[floor];
        temp.start.floor=floor;
        temp.end.floor=floor;
        temp.transparent=transparent;
        
        walls->push_back(temp);
    
        xo = xn;
        yo = yn;
    }
}

void base::addspline(const spline data,std::vector<wall>* walls,int floor, int transparent,const container* root,double off_x,double off_y,matrix_2d mat)
{
    
    const double &shift_x = root->current->floor_plans[floor].shift_x;
    const double &shift_y = root->current->floor_plans[floor].shift_y;

    double xo = data.get_x(data.t_start);
    double yo = data.get_y(data.t_start);
    
    auto xyo = mat.mult_vec({xo+off_x,yo+off_y});
    

    matrix_2d eye;

    int n = std::max(static_cast<int>(2*data.knots.size()),abs(static_cast<int>(20*data.approx_size)));

    //int num_segments=20+abs(static_cast<int>(((10.0*M_PI*std::max(r1,r2)))));

    double dt = data.t_range/static_cast<double>(n);

    for (int i=0;i<n;i++){
        double t = data.t_start + (i+1)*dt;
        double xn = data.get_x(t);
        double yn = data.get_y(t);    


        auto xyn = mat.mult_vec({xn+off_x,yn+off_y});
        
        wall temp;
        
        temp.start.x=xyo[0]+shift_x;
        temp.end.x=xyn[0]+shift_x;
        temp.start.y=xyo[1]+shift_y;
        temp.end.y=xyn[1]+shift_y;
        temp.start.z=root->current->floor_index[floor];
        temp.end.z=root->current->floor_index[floor];
        temp.start.floor=floor;
        temp.end.floor=floor;
        temp.transparent=transparent;
        
        walls->push_back(temp);

        xyo=xyn;
        
    }   
   
}

void base::addellipse(const ellipse data,std::vector<wall>* walls,int floor, int transparent,const container* root,double off_x,double off_y,matrix_2d mat)
{
    
    const double &shift_x = root->current->floor_plans[floor].shift_x;
    const double &shift_y = root->current->floor_plans[floor].shift_y;

    const double &cx=data.x;
    const double &cy=data.y;
    const double &r1=data.r1;
    const double &r2=data.r2;

    const double &cos_phi = data.cos_phi;
    const double &sin_phi = data.sin_phi;
    const double &ang1 = data.ang1;
    const double &ang2 = data.ang2;

    int num_segments=20+abs(static_cast<int>(((10.0*M_PI*std::max(r1,r2)))));
    
    double d_ang = (ang2-ang1)/(static_cast<double>(num_segments)-1.0);

    double xx = cos(ang1); 
    double yy = sin(ang1);

    double xo= off_x + cx +(r1*xx*cos_phi-r2*yy*sin_phi);
    double yo= off_y + cy +(r1*xx*sin_phi+r2*yy*cos_phi);

    auto xy = mat.mult_vec({xo,yo});
    xo = xy[0];
    yo = xy[1];


    for (int i=1;i<num_segments;i++){
        

        double xx = cos(ang1+i*d_ang); 
        double yy = sin(ang1+i*d_ang);        

        double x= off_x + cx +(r1*xx*cos_phi-r2*yy*sin_phi);
        double y= off_y + cy +(r1*xx*sin_phi+r2*yy*cos_phi);

        auto xy = mat.mult_vec({x,y});
        x = xy[0];
        y = xy[1];

        wall temp;
        
        temp.start.x=xo+shift_x;
        temp.end.x=x+shift_x;
        temp.start.y=yo+shift_y;
        temp.end.y=y+shift_y;
        temp.start.z=root->current->floor_index[floor];
        temp.end.z=root->current->floor_index[floor];
        temp.start.floor=floor;
        temp.end.floor=floor;
        temp.transparent=transparent;
        
        walls->push_back(temp);

        xo=x;
        yo=y;
        
    }
    
}

void base::addline(const line_in data,std::vector<wall>* walls,int floor, int transparent,const container* root,double xs,double ys,matrix_2d mat)
{

    const double &shift_x = root->current->floor_plans[floor].shift_x;
    const double &shift_y = root->current->floor_plans[floor].shift_y;

    wall temp;

    double x1  = data.start.x;
    double y1  = data.start.y;

    double x2  = data.end.x;
    double y2  = data.end.y;

    auto xy = mat.mult_vec({x1+xs,y1+ys});
    temp.start.x = xy[0]+shift_x;
    temp.start.y = xy[1]+shift_y;

    xy = mat.mult_vec({x2+xs,y2+ys});
    temp.end.x = xy[0]+shift_x;
    temp.end.y = xy[1]+shift_y;

    temp.start.z=root->current->floor_index[floor];
    temp.end.z=root->current->floor_index[floor];

    temp.start.floor=floor;
    temp.end.floor=floor;
    temp.transparent=transparent;

    walls->push_back(temp);
        
}

void base::add_block_nested(const plan* floor_plan, const block_entry &block,int count,const container* root,std::vector<wall>* walls,int transparent,int floor,const matrix_2d &mat,const double &tot_xs, const double &tot_ys){
     

    if (!floor_plan->blocks_active[block.list_no]){
        return;
    }
    
    if (block.list_no<0){ //no record of the block
        return;
    }
    
    const std::vector<block_entry> &blocks=floor_plan->block_definitions[block.list_no].nested_blocks; //use data structures in relevant block definition
    const std::vector<line_in> &lines=floor_plan->block_definitions[block.list_no].lines;
    const std::vector<arc> &arcs=floor_plan->block_definitions[block.list_no].arcs;
    const std::vector<circle> &circles=floor_plan->block_definitions[block.list_no].circles;
    const std::vector<ellipse> &ellipses=floor_plan->block_definitions[block.list_no].ellipses;
    const std::vector<spline> &splines=floor_plan->block_definitions[block.list_no].splines;
    const double xs = tot_xs-floor_plan->block_definitions[block.list_no].start_x;//insert_x +ii*rs-start_x;
    const double ys = tot_ys-floor_plan->block_definitions[block.list_no].start_y;//;insert_y + jj*cs-start_y;
     
    
    for (const arc &x :arcs){
        
        addarc(x,walls,floor, transparent,root,xs,ys,mat);
        
    }
    
    for (const circle &x : circles){
                   
        addcircle(x,walls,floor, transparent,root,xs,ys,mat);
                
    }

    for (const ellipse &x : ellipses){
            
        addellipse(x,walls,floor, transparent,root,xs,ys,mat);
        
    }
        
    for (const line_in &x : lines){
            
        addline(x,walls,floor,transparent,root,xs,ys,mat);    
        
    }
    

    if (splines_button->value()){
        for (const spline &x : splines){
                
            addspline(x,walls,floor, transparent,root,xs,ys,mat);
            
        }
    }

    
    for (const auto &x : blocks){
        
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

        add_block_nested(floor_plan,x,count+1,root,walls,transparent,floor,mat1,ix,iy);
        
    }
       
}


void base::lines_to_walls(int type){
    
    container* root = &(this->root);
    
    int transparent =0;
    
    bool st = false;
    
    int floor =  floor_choice->value()-1;

    std::vector<wall>* floor_walls = NULL;

    switch (type){
        case 0:
            floor_walls=&(root->current->walls_standing);
            st=true;
            break;
        case 1:
            floor_walls=&(root->current->walls_standing);
            transparent=1;
            st=true;
            break;
        case 2:
            floor_walls=&(root->current->walls_sitting);
            break;
        case 3:
            floor_walls=&(root->current->walls_sitting);
            transparent=1;
            break;
        default:
            return;
    }

    std::vector<wall> walls;

    floorplan_to_barriers(&walls,root,floor,transparent);

    walls_mutex.lock();

    for (int i=walls.size()-1;i>=0;i--){
        floor_walls->push_back(walls[i]);
        walls.pop_back();
    }

    walls_mutex.unlock();

    if (st){
        show_walls_standing->value(1);
    }
    else{
        show_walls_sitting->value(1);

    }

    prepare_undo();
    show_warning_visibility();
  
}


void base::floorplan_to_barriers(std::vector<wall> *walls,const container * root,int floor, int transparent){
    
    plan floor_plan;

    plans_mutex.lock();
    floor_plan=root->current->floor_plans[floor];
    plans_mutex.unlock();
    
    const std::vector<arc> &arcs=floor_plan.arcs;
    
    const std::vector<circle> &circles=floor_plan.circles;

    const std::vector<ellipse> &ellipses=floor_plan.ellipses;
    
    const std::vector<line_in> &lines=floor_plan.lines;

    const std::vector<spline> &splines=floor_plan.splines;
    
    const std::vector<block_entry> &blocks = floor_plan.blocks;

    const std::vector<int> &layers=floor_plan.layers;
    
    matrix_2d eye;

    for (const line_in &x : lines){
        
        if (layers[x.layer]){
                            
            addline(x,walls,floor,transparent,root,0,0,eye); 
                      
        }
        
    }
    
    for (const auto &x : arcs ){
        
        if (layers[x.layer]){

            addarc(x,walls,floor, transparent,root,0,0,eye);
        }
    }

    for (const auto &x : circles) {
        
        if (layers[x.layer]){

            addcircle(x,walls,floor, transparent,root,0,0,eye);
        }
    }
     

    for (const auto &x : ellipses ){
        
        if (layers[x.layer]){

            addellipse(x,walls,floor, transparent,root,0,0,eye);
        }
    }

    if (splines_button->value()){

        for (const auto &x : splines ){
            
            if (layers[x.layer]){
                
                addspline(x,walls,floor, transparent,root,0,0,eye);
            }
        }
    }
     

    for (const auto &x : blocks ){
                
                
        if (layers[x.layer]){
            
            
            double ix = x.insert_x;
            double iy = x.insert_y;

            add_block_nested(&floor_plan,x,0,root,walls,transparent,floor,x.matrix,ix,iy);
            
            
        }
        
    }
}

