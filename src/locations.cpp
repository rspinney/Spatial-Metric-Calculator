/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/locations.hpp"
#include "./include/preprocessor.hpp"

#undef min
#undef max

void plan::scale(double scale){
    
    for (unsigned int i=0;i<lines.size();i++){
        
        lines[i].start.x=scale*lines[i].start.x;// +shift_x;
        lines[i].end.x=scale*lines[i].end.x;//+shift_x;
        lines[i].start.y=scale*lines[i].start.y;//+shift_y;
        lines[i].end.y=scale*lines[i].end.y;//+shift_y;
        lines[i].start.z=scale*lines[i].start.z;//+shift_z;
        lines[i].end.z=scale*lines[i].end.z;//+shift_z;
    }
    
    
    for (unsigned int i=0;i<arcs.size();i++){
        
        arcs[i].x=scale*arcs[i].x;//+shift_x;
        arcs[i].y=scale*arcs[i].y;//+shift_y;
        arcs[i].z=scale*arcs[i].z;//+shift_z;
        arcs[i].r=scale*arcs[i].r;
        
    }
    
    for (unsigned int i=0;i<circles.size();i++){
        
        circles[i].x=scale*circles[i].x;//+shift_x;
        circles[i].y=scale*circles[i].y;//+shift_y;
        circles[i].z=scale*circles[i].z;//+shift_z;
        circles[i].r=scale*circles[i].r;
        
    }

    for (unsigned int i=0;i<ellipses.size();i++){
        
        ellipses[i].x*=scale;//+shift_x;
        ellipses[i].y*=scale;//+shift_y;
        ellipses[i].z*=scale;//+shift_z;
        ellipses[i].r1*=scale;
        ellipses[i].r2*=scale;
        
    }

    for (unsigned int i=0;i<splines.size();i++){

        splines[i].approx_size *= scale;

        for (unsigned int j=0; j<splines[i].x.size();j++){
            splines[i].x[j]*=scale;
            splines[i].y[j]*=scale;
            splines[i].z[j]*=scale;
        }
    }
    

    for (unsigned int i=0;i<blocks.size();i++){
        blocks[i].insert_x=scale*blocks[i].insert_x;
        blocks[i].insert_y=scale*blocks[i].insert_y;
        blocks[i].insert_z=scale*blocks[i].insert_z;

        matrix_2d mat;
        rotate_matrix rot(blocks[i].rotation*M_PI/180.0);
        translate_matrix t1(blocks[i].insert_x,blocks[i].insert_y);
        translate_matrix t2(-blocks[i].insert_x,-blocks[i].insert_y);
        scale_matrix s(blocks[i].scale_x,blocks[i].scale_y);

        mat = mat.mult(t2);
        mat = mat.mult(s);
        mat = mat.mult(rot);
        mat = mat.mult(t1);

        blocks[i].matrix = mat;

    }

    
    for (unsigned int i=0;i<block_definitions.size();i++){
        
        for (unsigned int j=0;j<block_definitions[i].lines.size();j++){
            
            block_definitions[i].lines[j].start.x=scale*block_definitions[i].lines[j].start.x;
            block_definitions[i].lines[j].end.x=scale*block_definitions[i].lines[j].end.x;
            block_definitions[i].lines[j].start.y=scale*block_definitions[i].lines[j].start.y;
            block_definitions[i].lines[j].end.y=scale*block_definitions[i].lines[j].end.y;
            block_definitions[i].lines[j].start.z=scale*block_definitions[i].lines[j].start.z;
            block_definitions[i].lines[j].end.z=scale*block_definitions[i].lines[j].end.z;
            
        }
        
        for (unsigned int j=0;j<block_definitions[i].arcs.size();j++){
            block_definitions[i].arcs[j].x=scale*block_definitions[i].arcs[j].x;
            block_definitions[i].arcs[j].y=scale*block_definitions[i].arcs[j].y;
            block_definitions[i].arcs[j].z=scale*block_definitions[i].arcs[j].z;
            block_definitions[i].arcs[j].r=scale*block_definitions[i].arcs[j].r;
            
        }
        
        for (unsigned int j=0;j<block_definitions[i].circles.size();j++){
            block_definitions[i].circles[j].x=scale*block_definitions[i].circles[j].x;
            block_definitions[i].circles[j].y=scale*block_definitions[i].circles[j].y;
            block_definitions[i].circles[j].z=scale*block_definitions[i].circles[j].z;
            block_definitions[i].circles[j].r=scale*block_definitions[i].circles[j].r;
            
        }

        for (unsigned int j=0;j<block_definitions[i].ellipses.size();j++){
            block_definitions[i].ellipses[j].x=scale*block_definitions[i].ellipses[j].x;
            block_definitions[i].ellipses[j].y=scale*block_definitions[i].ellipses[j].y;
            block_definitions[i].ellipses[j].z=scale*block_definitions[i].ellipses[j].z;
            block_definitions[i].ellipses[j].r1=scale*block_definitions[i].ellipses[j].r1;
            block_definitions[i].ellipses[j].r2=scale*block_definitions[i].ellipses[j].r2;
            
        }

        for (unsigned int j=0;j<block_definitions[i].splines.size();j++){

            block_definitions[i].splines[j].approx_size *= scale;

            for (unsigned int k=0; k<block_definitions[i].splines[j].x.size();k++){
                block_definitions[i].splines[j].x[k]*=scale;
                block_definitions[i].splines[j].y[k]*=scale;
                block_definitions[i].splines[j].z[k]*=scale;
            }
        }
        
        for (unsigned int j=0;j<block_definitions[i].nested_blocks.size();j++){
            
            block_definitions[i].nested_blocks[j].insert_x=scale*block_definitions[i].nested_blocks[j].insert_x;
            block_definitions[i].nested_blocks[j].insert_y=scale*block_definitions[i].nested_blocks[j].insert_y;
            block_definitions[i].nested_blocks[j].insert_z=scale*block_definitions[i].nested_blocks[j].insert_z;

            matrix_2d mat;
            rotate_matrix rot(block_definitions[i].nested_blocks[j].rotation*M_PI/180.0);
            translate_matrix t1(block_definitions[i].nested_blocks[j].insert_x,block_definitions[i].nested_blocks[j].insert_y);
            translate_matrix t2(-block_definitions[i].nested_blocks[j].insert_x,-block_definitions[i].nested_blocks[j].insert_y);
            scale_matrix s(block_definitions[i].nested_blocks[j].scale_x,block_definitions[i].nested_blocks[j].scale_y);

            mat = mat.mult(t2);
            mat = mat.mult(s);
            mat = mat.mult(rot);
            mat = mat.mult(t1);

            block_definitions[i].nested_blocks[j].matrix = mat;

        }
        
    }
    
    
    
}

int plan::scale(base*){
    
    double scale;
    
    // 0 = unitless
    // 1 = inches
    // 2 = feet
    // 3 = miles
    // 4 = mm
    // 5 = cm
    // 6 = m
    // 7 = km
    // 8 = microinches
    // 9 = Mils
    // 10 = yards
    // 11 = angstroms
    // 12 = nanonmeters
    // 13 = microns
    // 14 = decimeters
    // 15 = decameters
    // 16 = hectometers
    // 17 = gigameters
    // 18 = AU
    // 19 = light years
    // 20 = parsecs
    
    switch (unit_type){
            
        case 0:
            return 1;
            break;
        case 1:
            scale=0.0254; break;
        case 2:
            scale=0.3048;break;
        case 3:
            scale=1609.344;break;
        case 4:
            scale=0.001;break;
        case 5:
            scale=0.01;break;
        case 6:
            scale=1.0;break;
        case 7:
            scale=1000.0;break;
        case 8:
            scale=2.5400e-8;break;
        case 9:
            scale=2.5400e-5;break;
        case 10:
            scale=0.9144;break;
        case 11:
            scale=1.0e-10;break;
        case 12:
            scale=1.0e-9;break;
        case 13:
            scale=1.0e-6;break;
        case 14:
            scale=0.1;break;
        case 15:
            scale=10.0;break;
        case 16:
            scale=100.0;break;
        case 17:
            scale=1.0e9;break;
        case 18:
            scale=149597870700.0;break;
        case 19:
            scale=9.4605284e15;break;
        case 20:
            scale=3.08567758e16;break; //getting a bit silly now...
        default:
            return 1;
            break;
            
    }

    this->scale(scale);

    return 0;
}

int plan::line_only_version_size(){
    int count = 0;
    for (unsigned i =0; i< line_only_version.size();i++){
        count += line_only_version[i].size();
    }
    return count;
}

int plan::addline(const line_in data,double xs,double ys,matrix_2d mat,int block,int block_color,int block_layer)
{


    if (line_only_version_size()+1 > MAX_PLAN_LINES){
        return 1;
    }

    floor_line temp;

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

    temp.block = block;
    temp.color = data.color;
    temp.block_color = block_color;
    temp.layer = data.layer;
    if (block_layer>-1){
        temp.layer = block_layer;
    }   
    temp.is_spline=0;
    temp.point = BOTH;

    if ((temp.layer>=0)&& (temp.layer<static_cast<int>(line_only_version.size()))){
        line_only_version[temp.layer].push_back(temp);
    }
    else if (line_only_version.size()>0){
        line_only_version[0].push_back(temp);
    }

    return 0;
        
}

int plan::addellipse(const ellipse data,double off_x,double off_y,matrix_2d mat,int block,int block_color,int block_layer)
{
    
    const double &cx=data.x;
    const double &cy=data.y;
    const double &r1=data.r1;
    const double &r2=data.r2;

    const double &cos_phi = data.cos_phi;
    const double &sin_phi = data.sin_phi;
    const double &ang1 = data.ang1;
    const double &ang2 = data.ang2;
       

    int num_segments = LINE_MINIMUM + abs(static_cast<int>(((2*LINE_DENSITY*M_PI*std::max(r1,r2)))));

    if (line_only_version_size()+num_segments > MAX_PLAN_LINES){
        return 1;
    }
    
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

        floor_line temp;
        
        temp.start.x=xo+shift_x;
        temp.end.x=x+shift_x;
        temp.start.y=yo+shift_y;
        temp.end.y=y+shift_y;
        
        temp.block = block;
        temp.color = data.color;
        temp.block_color = block_color;
        temp.layer = data.layer;
        if (block_layer>-1){
            temp.layer = block_layer;
        }
        temp.is_spline=0;
        temp.point = NONE;

        if ((temp.layer>=0)&& (temp.layer<static_cast<int>(line_only_version.size()))){
            line_only_version[temp.layer].push_back(temp);
        }
        else if (line_only_version.size()>0){
            line_only_version[0].push_back(temp);
        }


        xo=x;
        yo=y;
        
    }

    return 0;
    
}


int plan::addspline(const spline data,double off_x,double off_y,matrix_2d mat,int block,int block_color,int block_layer)
{
    

    double xo = data.get_x(data.t_start);
    double yo = data.get_y(data.t_start);
    
    auto xyo = mat.mult_vec({xo+off_x,yo+off_y});
    

    matrix_2d eye;

    int n = std::max(static_cast<int>(2*data.knots.size()),abs(static_cast<int>(LINE_DENSITY*data.approx_size)));

    if (line_only_version_size()+n > MAX_PLAN_LINES){
        return 1;
    }

    double dt = data.t_range/static_cast<double>(n);

    for (int i=0;i<n;i++){
        double t = data.t_start + (i+1)*dt;
        double xn = data.get_x(t);
        double yn = data.get_y(t);    

        auto xyn = mat.mult_vec({xn+off_x,yn+off_y});
        
        floor_line temp;
        
        temp.start.x=xyo[0]+shift_x;
        temp.end.x=xyn[0]+shift_x;
        temp.start.y=xyo[1]+shift_y;
        temp.end.y=xyn[1]+shift_y;
    
        
        temp.block = block;
        temp.color = data.color;
        temp.block_color = block_color;
        temp.layer = data.layer;
        if (block_layer>-1){
            temp.layer = block_layer;
        }
        temp.is_spline=1;
        if (i==0){
            temp.point = FIRST;
        }
        else if (i==n-1){
            temp.point = LAST;
        }
        else{
            temp.point = NONE;
        }

        if ((temp.layer>=0)&& (temp.layer<static_cast<int>(line_only_version.size()))){
            line_only_version[temp.layer].push_back(temp);
        }
        else if (line_only_version.size()>0){
            line_only_version[0].push_back(temp);
        }

        xyo=xyn;
        
    }   

    return 0;
   
}

int plan::addcircle(const circle data,double off_x,double off_y,matrix_2d mat,int block,int block_color,int block_layer)
{
    
    double cx=data.x;
    double cy=data.y;
    double r=data.r;

    
    int num_segments=LINE_MINIMUM+abs(static_cast<int>(((2.0*LINE_DENSITY*M_PI*r))));

    if (line_only_version_size()+num_segments > MAX_PLAN_LINES){
        return 1;
    }

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

        floor_line temp;
        
        temp.start.x=xo+shift_x;
        temp.end.x=xn+shift_x;
        temp.start.y=yo+shift_y;
        temp.end.y=yn+shift_y;
        
        temp.block = block;
        temp.color = data.color;
        temp.block_color = block_color;
        temp.layer = data.layer;
        if (block_layer>-1){
            temp.layer = block_layer;
        }
        temp.is_spline=0;
        temp.point = NONE;
        
        if ((temp.layer>=0)&& (temp.layer<static_cast<int>(line_only_version.size()))){
            line_only_version[temp.layer].push_back(temp);
        }
        else if (line_only_version.size()>0){
            line_only_version[0].push_back(temp);
        }



        xo = xn;
        yo = yn;
    }

    return 0;
}


int plan::addarc(const arc data,double off_x,double off_y,matrix_2d mat,int block,int block_color,int block_layer)
{
    

    const double &cx=data.x;
    const double &cy=data.y;
    const double &r=data.r;
    

    
    const double &ang1=data.ang1;
    const double &ang2=data.ang2;
    int num_segments = LINE_MINIMUM + abs(static_cast<int>(((2.0*LINE_DENSITY*fabs(ang2-ang1)*M_PI*r))));
    
    if (line_only_version_size()+num_segments > MAX_PLAN_LINES){
        return 1;
    }
    
    double xo= r*cos(ang1); //watch for specific fasb(r) vs not behaviour is x and y: ridiculous format
    double yo= r*sin(ang1);

    auto xy = mat.mult_vec({xo + cx +off_x,yo + cy + off_y});
    xo = xy[0];
    yo = xy[1];
    
    for (int i=0;i<num_segments;i++){
        
        
        
        double x= r*cos(ang1+i*(ang2-ang1)/(static_cast<double>(num_segments)-1.0)); //watch for specific fasb(r) vs not behaviour is x and y: ridiculous format
        double y= r*sin(ang1+i*(ang2-ang1)/(static_cast<double>(num_segments)-1.0));

        auto xy = mat.mult_vec({x+cx+off_x,y+cy+off_y});
        double xn = xy[0];
        double yn = xy[1];


        floor_line temp;
        
        temp.start.x=xo+shift_x;
        temp.end.x=xn+shift_x;
        temp.start.y=yo+shift_y;
        temp.end.y=yn+shift_y;
        
        temp.block = block;
        temp.color = data.color;
        temp.block_color = block_color;
        temp.layer = data.layer;
        if (block_layer>-1){
            temp.layer = block_layer;
        }
        temp.is_spline=0;


        if (i==0){
            temp.point = FIRST;
        }
        else if (i==num_segments-1){
            temp.point = LAST;
        }
        else{
            temp.point = NONE;
        }

        if ((temp.layer>=0)&& (temp.layer<static_cast<int>(line_only_version.size()))){
            line_only_version[temp.layer].push_back(temp);
        }
        else if (line_only_version.size()>0){
            line_only_version[0].push_back(temp);
        }

        xo=xn;yo=yn;

    }
    
    
    return  0;
    
    
}



int plan::add_block_nested(const block_entry &block,int count,const matrix_2d &mat,const double &tot_xs, const double &tot_ys,int block_layer){
       
    
    if (block.list_no<0){ //no record of the block
        return 0;
    }

    
    const std::vector<block_entry> &blocks=block_definitions[block.list_no].nested_blocks; //use data sytructures in relevant block definition
    const std::vector<line_in> &lines=block_definitions[block.list_no].lines;
    const std::vector<arc> &arcs=block_definitions[block.list_no].arcs;
    const std::vector<circle> &circles=block_definitions[block.list_no].circles;
    const std::vector<ellipse> &ellipses=block_definitions[block.list_no].ellipses;
    const std::vector<spline> &splines=block_definitions[block.list_no].splines;
    const double xs = tot_xs-block_definitions[block.list_no].start_x;//insert_x +ii*rs-start_x;
    const double ys = tot_ys-block_definitions[block.list_no].start_y;//;insert_y + jj*cs-start_y;
     
    

    
    for (const arc &x :arcs){
        
        int i = addarc(x,xs,ys,mat,block.list_no,block.color,block_layer);
        if (i){
            fast_plan = 0;
            line_only_version.clear();
            return 1;
        }
        
    }
    
    for (const circle &x : circles){
                   
        int i = addcircle(x,xs,ys,mat,block.list_no,block.color,block_layer);
        if (i){
            fast_plan = 0;
            line_only_version.clear();
            return 1;
        }
                
    }

    for (const ellipse &x : ellipses){
            
        int i = addellipse(x,xs,ys,mat,block.list_no,block.color,block_layer);
        if (i){
            fast_plan = 0;
            line_only_version.clear();
            return 1;
        }
        
    }
        
    for (const line_in &x : lines){
            
        int i = addline(x,xs,ys,mat,block.list_no,block.color,block_layer);
        if (i){
            fast_plan = 0;
            line_only_version.clear();
            return 1;
        }
            
    }
    

    for (const spline &x : splines){
            
        int i = addspline(x,xs,ys,mat,block.list_no,block.color,block_layer);
        if (i){
            fast_plan = 0;
            line_only_version.clear();
            return 1;
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

        int i = add_block_nested(x,count+1,mat1,ix,iy,block_layer); //carry layer of parent block down the tree
        if (i){
            fast_plan = 0;
            line_only_version.clear();
            return 1;
        }
        
    }
    
    return 0;
}

void plan::convert_to_lines(){
    //return value is "fast_plan" data member
    // i.e. 0:  failure to convert to fast plan form, 1: success.

    line_only_version.clear();


    line_only_version = std::vector<std::vector<floor_line> > (layers.size(),std::vector<floor_line>(0));
    
    matrix_2d eye;

    
    for (const line_in &x : lines){
                            
        int i = addline(x,0,0,eye,-1,-1,-1); 
        if (i){
            fast_plan = 0;
            line_only_version.clear();
            return;
        }
                      
    }
    
    for (const auto &x : arcs ){
        
        int i = addarc(x,0,0,eye,-1,-1,-1);
        if (i){
            fast_plan = 0;
            line_only_version.clear();
            return;
        }
        
    }

    for (const auto &x : circles) {
        
        
        int i = addcircle(x,0,0,eye,-1,-1,-1);
        if (i){
            fast_plan = 0;
            line_only_version.clear();
            return;
        }
        
    }
     

    for (const auto &x : ellipses ){

        int i = addellipse(x,0,0,eye,-1,-1,-1);
        if (i){
            fast_plan = 0;
            line_only_version.clear();
            return;
        }
        
    }


    for (const auto &x : splines ){    
                
        int i = addspline(x,0,0,eye,-1,-1,-1);
        if (i){
            fast_plan = 0;
            line_only_version.clear();
            return;
        }
        
    }
    

    for (const auto &x : blocks ){
                               
        double ix = x.insert_x;
        double iy = x.insert_y;

        int i = add_block_nested(x,0,x.matrix,ix,iy,x.layer);
        if (i){
            fast_plan = 0;
            line_only_version.clear();
            return;
        }
            
    }

    fast_plan = 1;
    return;


}