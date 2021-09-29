/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/dxf_parser.hpp"

#undef min
#undef max

/* reference used to parse DXFs can be found here:

    https://images.autodesk.com/adsk/files/autocad_2012_pdf_dxf-reference_enu.pdf

   Slowly making way through important objects. More may be added in future releases */

int dxf_parser::read_in(std::string in){

    reading_object = -1;
    
    std::ifstream dxf_stream;
    std::string str_code,str_value;
       
    try {
        dxf_stream.open(in.c_str());
    }
    catch (const std::ifstream::failure &e) {
        return 0;
    }
    
    current_layer = "0";
    current_color = 256;
    current_paper_space = 0;
    ang_convert = 1.0;

    int line = 0;

    error = 0;
    
    while (1){

        if (error){
            return 0;
        }

        try{
            getline(dxf_stream, str_code);
            getline(dxf_stream, str_value);
        }
        catch(...){
            return 0;
        }

        if (dxf_stream.eof()){
            break;
        }

        str_code = trim(str_code); 
        str_value = trim(str_value);

        int code = to_int(str_code);

        if (code==0){ //start an object or set a value

            if (line>0){
                if (reading_object==1){
                    push_line(line_read); //store object
                    line_read = line_in(); //reset temp object
                }
                else if (reading_object==2){
                    push_polyline(polyline_read); //store object 
                    polyline_read = polyline(); //reset temp object
                }
                else if (reading_object==4){
                    push_arc(arc_read); //store object 
                    arc_read = arc(); //reset temp object
                }
                else if (reading_object==5){
                    push_circle(circle_read); //store object 
                    circle_read = circle(); //reset temp object
                }
                else if (reading_object==6){
                    push_ellipse(ellipse_read); //store object 
                    ellipse_read = ellipse_in(); //reset temp object
                }
                else if (reading_object==7){
                    push_spline(spline_read); //store object 
                    spline_read = spline(); //reset temp object
                }
                else if (reading_object==10){
                    start_block(block_read);
                    block_read = block_in();
                }
                else if (reading_object==11){
                    end_block();
                }
                else if (reading_object==20){
                    push_insert(insert_read);
                    insert_read = insert();
                }
                else if (reading_object==30){
                    push_layer(layer_read);
                    layer_read = layer_in();
                }
            }
            current_layer = "0"; //default layer
            current_color = 256; //if not specified, color is "BYLAYER"
            current_paper_space = 0; //assume everything is model space unless otherwise specified

            reading_object=-1;//stop reading whatever we were reading
            if (!str_value.compare("LINE")){
                reading_object = 1;
            }
            else if ((!str_value.compare("POLYLINE"))||(!str_value.compare("LWPOLYLINE"))){
                reading_object = 2;
                poly_x.clear();
                poly_y.clear();
                poly_z.clear();
                poly_bulge.clear();
            }
            else if (!str_value.compare("ARC")){
                reading_object = 4;
            }
            else if (!str_value.compare("CIRCLE")){
                reading_object = 5;
            }
            else if (!str_value.compare("ELLIPSE")){
                reading_object = 6;
            }
            else if (!str_value.compare("SPLINE")){
                reading_object = 7;
                spline_x.clear();
                spline_y.clear();
                spline_z.clear();
                spline_knots.clear();
            }
            else if(!str_value.compare("BLOCK")){
                reading_object = 10;
            }
            else if(!str_value.compare("ENDBLK")){
                reading_object = 11;
            }
            else if(!str_value.compare("INSERT")){
                reading_object = 20;
            }
            else if(!str_value.compare("LAYER")){
                reading_object = 30;
            }
            // else if(!str_value.compare("HATCH")){ 
            //     reading_object = 50; //TO DO
            // }
        }
        else if (code==9){//setting  a value
            reading_object=-1; //stop reading whatever we were reading
            if(!str_value.compare("$INSUNITS")){ //units 
                reading_object = 40;
            }
            else if((!str_value.compare("$AUNITS"))||(!str_value.compare("$DIMAUNIT"))){ //angular units (not ellipses?)
                reading_object = 41;
            }    
        }
        else if (code==62){ //reading current color
            current_color = to_int(str_value);
        }
        else if (code==8){ //reading current layer
            current_layer = str_value;
        }
        else if (code==67){ //reading current paper_space
            current_paper_space = to_int(str_value);
        }
        else{ //capturing object properties
            if (reading_object==1){ //line
                switch (code){
                    case 10: line_read.start.x = to_double(str_value);break;
                    case 20: line_read.start.y = to_double(str_value);break;
                    case 30: line_read.start.z = to_double(str_value);break;
                    case 11: line_read.end.x = to_double(str_value);break;
                    case 21: line_read.end.y = to_double(str_value);break;
                    case 31: line_read.end.z = to_double(str_value);break;
                }
            }
            if (reading_object==2){ //polyline
                switch (code){
                    case 71: polyline_read.m = to_int(str_value);break;
                    case 72: polyline_read.n = to_int(str_value);break;
                    case 70: 
                        polyline_read.closed = to_int(str_value);
                        if (polyline_read.closed>1){//advanced options, but not closed.
                            polyline_read.closed = 0;
                        }
                        break;
                    case 10: 
                        poly_x.push_back(to_double(str_value)); 
                        poly_y.push_back(0.0);
                        poly_z.push_back(0.0);
                        poly_bulge.push_back(0.0);
                        polyline_read.number = poly_x.size(); 
                        break;
                    case 20: poly_y.back()=to_double(str_value); break;
                    case 30: poly_z.back()=to_double(str_value); break;
                    case 42: poly_bulge.back()=to_double(str_value); break;
                }
            }
            else if (reading_object==4){ //arc
                switch (code){
                    case 10: arc_read.x = to_double(str_value);break;
                    case 20: arc_read.y = to_double(str_value);break;
                    case 30: arc_read.z = to_double(str_value);break;
                    case 40: arc_read.r = to_double(str_value);break;
                    case 50: arc_read.ang1 = to_double(str_value);break;
                    case 51: arc_read.ang2 = to_double(str_value);break;
                }
            }
            else if (reading_object==5){ //circle
                switch (code){
                    case 10: circle_read.x = to_double(str_value);break;
                    case 20: circle_read.y = to_double(str_value);break;
                    case 30: circle_read.z = to_double(str_value);break;
                    case 40: circle_read.r = to_double(str_value);break;                    
                }
            }
            else if (reading_object==6){ //ellipse
                switch (code){
                    case 10: ellipse_read.x = to_double(str_value);break;
                    case 20: ellipse_read.y = to_double(str_value);break;
                    case 30: ellipse_read.z = to_double(str_value);break;
                    case 11: ellipse_read.major_x = to_double(str_value);break;
                    case 21: ellipse_read.major_y = to_double(str_value);break;
                    case 31: ellipse_read.major_z = to_double(str_value);break;
                    case 40: ellipse_read.radius_ratio = to_double(str_value);break; 
                    case 41: ellipse_read.angle1 = to_double(str_value);break; 
                    case 42: ellipse_read.angle2 = to_double(str_value);break;                    
                }
            }
            else if (reading_object==7){ //spline
                switch (code){
                    case 10: 
                        spline_x.push_back(to_double(str_value)); 
                        spline_y.push_back(0.0);
                        spline_z.push_back(0.0);
                        break;
                    case 20: spline_y.back() = to_double(str_value);break;
                    case 30: spline_z.back() = to_double(str_value);break;
                    case 40: spline_knots.push_back(to_double(str_value)); break;
                }
            }
            else if (reading_object==10){  //start block
                switch (code){
                    case 2: block_read.name = str_value;break;
                    case 10: block_read.x = to_double(str_value);break;
                    case 20: block_read.y = to_double(str_value);break;
                    case 30: block_read.z = to_double(str_value);break;
                }
            }
            else if (reading_object==11){ //end block
                //end block - do nothing
            }
            else if (reading_object==20){ //insert (i.e. a block)
                switch (code){
                    case 2: insert_read.name = str_value;break;
                    case 10: insert_read.insert_x = to_double(str_value);break;
                    case 20: insert_read.insert_y = to_double(str_value);break;
                    case 30: insert_read.insert_z = to_double(str_value);break;
                    case 41: insert_read.scale_x = to_double(str_value);break;
                    case 42: insert_read.scale_y = to_double(str_value);break;
                    case 43: insert_read.scale_z = to_double(str_value);break;
                    case 50: insert_read.rotation = to_double(str_value);break;
                    case 70: insert_read.cols = to_int(str_value);break;
                    case 71: insert_read.rows = to_int(str_value);break;
                    case 44: insert_read.space_dx = to_double(str_value);break;
                    case 45: insert_read.space_dy = to_double(str_value);break;
                }
            }
            else if (reading_object==30){ //reading layer
                if (code==2){
                    layer_read.name = str_value;
                }
            }
            else if (reading_object==40){ //reading units
                if (code==70){
                    units = to_int(str_value);
                    //std::cout<<"actually setting units"<<std::endl;
                }
            }
            else if (reading_object==41){ //reading angular units
                if (code==70){
                    int v = to_int(str_value);
                    if (v==0){ //degrees
                        ang_convert = 1.0;
                    }
                    else if (v==1){// degrees, mins, secs ---> TO DO: this will result in a lexical cast exception, how to manage?
                        ang_convert = 1.0;//last resort
                    }
                    else if (v==2){// gradians
                        ang_convert = 10.0/9.0;
                    }
                    else if (v==3){//radians
                        ang_convert = 180.0/M_PI; //convert TO degrees (for now)
                    }
                    else if (v==4){ //surveyors units
                        ang_convert =1.0; // last resort.
                    }
                    
                }
            }
            
        }

        line+=2; 
    }

    // temporary region testing
    //process_regions();

    return 1;
}


void dxf_parser::push_line(const line_in& temp_in) {
   
    line_in temp = temp_in;
        
    int color = std::abs(current_color);

    for (unsigned int i=0;i<layers.size();i++){
        if (current_layer.compare(layers[i])==0){   
            temp.layer=i;
            layers_used[i]=1;
            if (color == 256){ //color defined by layer
                color = layer_colors[i];
            }
            
        }
    }
    
    temp.color = color;
    
    if (!reading_block){
   
        lines.push_back(temp);

    }
    else{             
        
        int block = block_definitions.size(); 
        
        if (color==0){ // color defined by block
            // color = blockcolor;
            // temp.color = color;
        }

        if (block){

            block_definitions[block-1].lines.push_back(temp);            
            
        }
            
    }
    polyline_in=0;

}


void dxf_parser::start_block(const block_in& bl){
    
   blockcolor = std::abs(current_color);

    reading_block=1;
    block_definition temp;
    temp.name=bl.name;
    temp.start_x=bl.x;
    temp.start_y=bl.y;
    temp.start_z=bl.z;
    block_definitions.push_back(temp);
    blocks_used.push_back(0);
    
}

void dxf_parser::end_block(){

    reading_block=0;
}

void dxf_parser::push_insert(const insert & insert_in){

    if (current_paper_space)return;

    block_entry temp;
    
    temp.color = std::abs(current_color);

    for (unsigned int i=0;i<layers.size();i++){
        if (current_layer.compare(layers[i])==0){

            temp.layer=i;
            layers_used[i]=1;
            if (temp.color==256){
                temp.color = layer_colors[i];
            }
        }
    }

    for (unsigned int i=0;i<block_definitions.size();i++){
        if (insert_in.name.compare(block_definitions[i].name)==0){
            blocks_used[i]=1;
        }
    }


    temp.name=insert_in.name;
    temp.scale_x=insert_in.scale_x;
    temp.scale_y=insert_in.scale_y;
    temp.scale_z=insert_in.scale_z;
    temp.insert_x=insert_in.insert_x;
    temp.insert_y=insert_in.insert_y;
    temp.insert_z=insert_in.insert_z;
    temp.rotation=insert_in.rotation * ang_convert; //convert to degrees from file units


    matrix_2d mat;
    rotate_matrix rot(temp.rotation*M_PI/180.0);
    translate_matrix t1(temp.insert_x,temp.insert_y);
    translate_matrix t2(-temp.insert_x,-temp.insert_y);
    scale_matrix s(temp.scale_x,temp.scale_y);

    mat = mat.mult(t2);
    mat = mat.mult(s);
    mat = mat.mult(rot);
    mat = mat.mult(t1);

    temp.matrix = mat;

    int rows = insert_in.rows;
    int cols = insert_in.cols;
    double dx = insert_in.space_dx;
    double dy = insert_in.space_dy;

    
    if (!reading_block){
     
        for (int i=0;i<rows;i++){
            for (int j=0;j<cols;j++){

                block_entry bl =temp;
                bl.insert_x += j*dx;
                bl.insert_y += i*dy;

                blocks.push_back(std::move(bl));
            }
        }
        
    }
    else{
        
        int block = block_definitions.size();

        for (int i=0;i<rows;i++){
            for (int j=0;j<cols;j++){

                block_entry bl =temp;
                bl.insert_x += j*dx;
                bl.insert_y += i*dy;

                block_definitions[block-1].nested_blocks.push_back(std::move(bl)); //if reading block push inserted block onto nested blocks of the block definition
            }
        }
        
    }

}

void dxf_parser::push_polyline(const polyline& p_line) {
   
   if (current_paper_space)return;

    polylinecolor = std::abs(current_color);

    polyline_in=1;
    polyline_num=p_line.number;
    polyline_count=0;
    first_polyline=0;
    poly_flag = p_line.closed;
    is_arc=false;
    is_arc_next=false;

    for (int i=0;i<polyline_num;i++){
        poly_point temp;
        temp.x = poly_x[i];
        temp.y = poly_y[i];
        temp.z = poly_z[i];
        temp.bulge = poly_bulge[i];
        push_poly_point(temp);
    }

}

arc dxf_parser::bulge_to_arc(const line_in line, double bulge){

    arc to_return;

    double theta = 4.0 * atan(bulge);
    double a = theta / 2.0;
    double dist = sqrt((line.start.x-line.end.x)*(line.start.x-line.end.x)+(line.start.y-line.end.y)*(line.start.y-line.end.y));

    double r = dist / (2.0 * sin(a));

    to_return.r = fabs(r);

    double phi = atan2(line.end.y-line.start.y,line.end.x-line.start.x);

    to_return.x = line.start.x + r * cos (phi + (M_PI-theta)/2.0 );
    to_return.y = line.start.y + r * sin (phi + (M_PI-theta)/2.0 );

    to_return.x = line.start.x + r * cos (phi + (M_PI)/2.0 - a);
    to_return.y = line.start.y + r * sin (phi + (M_PI)/2.0 - a);
    to_return.z = line.start.z;

    if (bulge>0){
        
        to_return.ang2 = atan2(line.end.y-to_return.y,line.end.x-to_return.x);
        to_return.ang1 = atan2(line.start.y-to_return.y,line.start.x-to_return.x);    
    }
    else{
        
        to_return.ang1 = atan2(line.end.y-to_return.y,line.end.x-to_return.x);
        to_return.ang2 = atan2(line.start.y-to_return.y,line.start.x-to_return.x);  
    }

    while (to_return.ang1>to_return.ang2){ //angle 2 has to be bigger
        to_return.ang1-=2.0*M_PI;
    }

    return to_return;
}

void dxf_parser::push_arc(const arc & arc_in){
    
    if (current_paper_space)return;

    int color = std::abs(current_color);

    arc temp;
    
    for (unsigned int i=0;i<layers.size();i++){
        if (current_layer.compare(layers[i])==0){            
            temp.layer=i;
            layers_used[i]=1;
            if (color == 256){ //color defined by layer
                color = layer_colors[i];
            }
        }
    }
    
    temp.color= color;
    temp.x=arc_in.x;
    temp.y=arc_in.y;
    temp.z=arc_in.z;
    temp.r=arc_in.r;
    if (temp.r<0){
        temp.r *= -1.0;
    }
    temp.ang1=(M_PI/180.0)*ang_convert*arc_in.ang1; 
    temp.ang2=(M_PI/180.0)*ang_convert*arc_in.ang2;
    
    while (temp.ang1>2*M_PI){ //angle 2 has to be bigger
        temp.ang1-=2*M_PI;
    }

    while (temp.ang2>2*M_PI){ //angle 2 has to be bigger
        temp.ang2-=2*M_PI;
    }

    while (temp.ang1<0){ //angle 2 has to be bigger
        temp.ang1+=2*M_PI;
    }

    while (temp.ang2<0){ //angle 2 has to be bigger
        temp.ang2+=2*M_PI;
    }

    while (temp.ang1>temp.ang2){ //angle 2 has to be bigger
        temp.ang1-=2*M_PI;
    }

    
    if (!reading_block){
        
        arcs.push_back(temp);
    }
    else{
        
        if (color==0){ // color defined by block
            //color = blockcolor;
            //temp.color = color;
        }
        
        int block = block_definitions.size();
        
        if (block){
            
            block_definitions[block-1].arcs.push_back(std::move(temp));
            
        }
        
    }
    polyline_in=0;
    
}

void dxf_parser::push_circle(const circle & circle_in){

    if (current_paper_space)return;

    int color = std::abs(current_color);

    circle temp;
    
    for (unsigned int i=0;i<layers.size();i++){
        if (current_layer.compare(layers[i])==0){            
            temp.layer=i;
            layers_used[i]=1;
            if (color == 256){ //color defined by layer
                color = layer_colors[i];
            }
        }
    }
    
    temp.color = color;    
    temp.x=circle_in.x;
    temp.y=circle_in.y;
    temp.z=circle_in.z;
    temp.r=circle_in.r;

    
    if (!reading_block){
        
        circles.push_back(std::move(temp));
    }
    else{
                
        if (color==0){ // color defined by block
            //color = blockcolor;
            //temp.color = color;
        }

        int block = block_definitions.size();
        
        if (block){
            
            block_definitions[block-1].circles.push_back(std::move(temp));
            
        }
        
    }
    polyline_in=0;
    
}

void dxf_parser::push_ellipse(const ellipse_in & ellipse_in){

    if (current_paper_space)return;

    int color = std::abs(current_color);

    ellipse temp;
    
    for (unsigned int i=0;i<layers.size();i++){
        if (current_layer.compare(layers[i])==0){            
            temp.layer=i;
            layers_used[i]=1;
            if (color == 256){ //color defined by layer
                color = layer_colors[i];
            }
        }
    }
    
    temp.color=color;
    temp.x=ellipse_in.x;
    temp.y=ellipse_in.y;
    temp.z=ellipse_in.z;

    temp.ang1 = ellipse_in.angle1; //ELLIPSE ANGLES ARE ALWAYS IN RADIANS INDEPENDENT OF FILE WIDE UNITS
    temp.ang2 = ellipse_in.angle2;

    temp.r1 = sqrt((ellipse_in.major_x)*(ellipse_in.major_x) + (ellipse_in.major_y)*(ellipse_in.major_y)+ (ellipse_in.major_z)*(ellipse_in.major_z));
    temp.r2 = temp.r1 * ellipse_in.radius_ratio;
    temp.phi = atan2(ellipse_in.major_y,ellipse_in.major_x);
    temp.cos_phi = cos(temp.phi);
    temp.sin_phi = sin(temp.phi);

    while (temp.ang1>2*M_PI){ //angle 2 has to be bigger
        temp.ang1-=2*M_PI;
    }

    while (temp.ang2>2*M_PI){ //angle 2 has to be bigger
        temp.ang2-=2*M_PI;
    }

    while (temp.ang1<0){ //angle 2 has to be bigger
        temp.ang1+=2*M_PI;
    }

    while (temp.ang2<0){ //angle 2 has to be bigger
        temp.ang2+=2*M_PI;
    }

    while (temp.ang1>temp.ang2){ //angle 2 has to be bigger
        temp.ang1-=2*M_PI;
    }

    if (!reading_block){
        
        ellipses.push_back(std::move(temp));
    }
    else{
        
        if (color==0){ // color defined by block
            //color = blockcolor;
            //temp.color = color;
        }

        int block = block_definitions.size();
        
        if (block){

            block_definitions[block-1].ellipses.push_back(std::move(temp));
            
        }
        
    }
    polyline_in=0;
    
}

void dxf_parser::push_layer (const layer_in & layer_data){

    if (layer_data.name.length()==0){
        return;
    }
    //current_layer = layer_data.name;

    if (std::abs(current_color)==256){
        current_color = 7;
    }
    if (std::abs(current_color)==0){
        current_color = 7;
    }

    if ((layers.size())&&(!layer_data.name.compare(layers[0]))){ //default layer that we put in for broken files
        //overwrite
        layers_used[0]=0;
        layer_colors[0]=0;
        layer_widths[0]=1;
    }
    else{
        layers.push_back(layer_data.name);
        layers_used.push_back(0);
        layer_colors.push_back(std::abs(current_color));
        layer_widths.push_back(1); //default
    }
    polyline_in=0;
}




void dxf_parser::push_poly_point(const poly_point& poly_in) {

    int color = std::abs(current_color);

    polyline_count++;


    if (polyline_in){
        
        if (first_polyline==0){
            
            last_vertex.x=poly_in.x;
            last_vertex.y=poly_in.y;
            last_vertex.z=poly_in.z; 
            first_vertex = last_vertex;
            first_polyline=1;

            bulge_next=poly_in.bulge;

            if (fabs(poly_in.bulge)>0){
                is_arc_next = true;
            }
            else{
                is_arc_next = false;
            }

            //temporary region test
            // vec temp; ////////////////////////////
            // temp.x = poly_in.x; ////////////////////////////
            // temp.y = poly_in.y; ////////////////////////////
            // temp.z = poly_in.z; ////////////////////////////
            // current_polygon.push_back(std::move(temp)); ////////////////////////////
            // //end temporary region test
        }
        else{
            line_in temp;
            temp.start.x=last_vertex.x;
            temp.end.x=poly_in.x;
            temp.start.y=last_vertex.y;
            temp.end.y=poly_in.y;
            temp.start.z=last_vertex.z;
            temp.end.z=poly_in.z;

            bulge_next=poly_in.bulge;

            arc bulge_arc;
            
            if (fabs(poly_in.bulge)>0){
                is_arc_next = true;
            } 
            else{
                is_arc_next=false;
            }

            if (is_arc){
                bulge_arc = bulge_to_arc(temp,bulge);  
            }
            
            for (unsigned int i=0;i<layers.size();i++){
                if (current_layer.compare(layers[i])==0){

                    temp.layer=i;
                    bulge_arc.layer=i;
                    layers_used[i]=1;
                    if (color == 256){ //color defined by layer
                        color = layer_colors[i];
                    }
                }
            }

            temp.color = color;
            bulge_arc.color = color;
            
            if (!reading_block){
                if(!is_arc){
                    lines.push_back(std::move(temp));
                }
                else{
                    arcs.push_back(std::move(bulge_arc));
                }
            }
            else{

                if (color==0){ // color defined by block
                    //color = blockcolor;
                    //temp.color = color;
                }

                int block = block_definitions.size();
                
                if (block){

                    if(!is_arc){
                        block_definitions[block-1].lines.push_back(std::move(temp));
                    }
                    else{
                        //block_definitions[block-1].lines.push_back(temp);
                        block_definitions[block-1].arcs.push_back(std::move(bulge_arc));
                    }

                    
                    
                }
                
            }
            
            last_vertex.x=poly_in.x;
            last_vertex.y=poly_in.y;
            last_vertex.z=poly_in.z;

            // //temporary region test
            // vec temp_vec; ////////////////////////////
            // temp_vec.x = poly_in.x; ////////////////////////////
            // temp_vec.y = poly_in.y; ////////////////////////////
            // temp_vec.z = poly_in.z; ////////////////////////////
            // current_polygon.push_back(std::move(temp_vec)); ////////////////////////////
            // //end temporary region test

            if ((polyline_count == polyline_num)&&(poly_flag)){

                //temporary region test
                // region temp_region; /////////////////////
                // temp_region.polygon = current_polygon; /////////////////////
                // temp_region.layer = 0; /////////////////////
                // temp_region.block = 0; /////////////////////
                // regions.push_back(temp_region); /////////////////////
                // //end temporary region test

                is_arc_next = false;
                is_arc = false;

                line_in temp;
                temp.end.x=first_vertex.x;
                temp.start.x=poly_in.x;
                temp.end.y=first_vertex.y;
                temp.start.y=poly_in.y;
                temp.end.z=first_vertex.z;
                temp.start.z=poly_in.z;
                
                arc bulge_arc;
                bool is_arc_n = false;
                if (fabs(poly_in.bulge)>0){
                    is_arc_n = true;
                    bulge_arc = bulge_to_arc(temp,poly_in.bulge);  
                } 

                for (unsigned int i=0;i<layers.size();i++){
                    if (current_layer.compare(layers[i])==0){

                        temp.layer=i;
                        bulge_arc.layer=i;
                        layers_used[i]=1;                        
                        if (color == 256){ //color defined by layer
                            color = layer_colors[i];
                        }

                        // //temporary region test
                        // regions.back().layer = i; //////////////////////////////
                        // //end temporary region test
                    }
                }
                
                temp.color = color;
                bulge_arc.color = color;

                if (!reading_block){
                    if(!is_arc_n){
                        lines.push_back(std::move(temp));
                    }
                    else{
                        arcs.push_back(std::move(bulge_arc));
                    }
                }
                else{

                    if (color==0){ // color defined by block
                        //color = blockcolor;
                        //temp.color = color;
                    }

                    int block = block_definitions.size();

                    // //temporary region test
                    //regions.back().layer = block - 1;  //////////////////////////////
                    // //end temporary region test
                
                    if (block){

                        if(!is_arc_n){
                            block_definitions[block-1].lines.push_back(std::move(temp));
                        }
                        else{
                            block_definitions[block-1].arcs.push_back(std::move(bulge_arc));
                        }
                        
                    }
                    
                }
                
            }

        }

        is_arc = is_arc_next;
        bulge = bulge_next;
        
    }    
    
}


void dxf_parser::push_spline(const spline &){
        
    if (current_paper_space)return;

    int color = std::abs(current_color);

    int layer = 0;

    for (unsigned int i=0;i<layers.size();i++){
        if (current_layer.compare(layers[i])==0){   
            layer=i;
            layers_used[i]=1;
            if (color == 256){ //color defined by layer
                color = layer_colors[i];
            }
        }
    }
    if (!reading_block){
        splines.emplace_back(spline());
        splines.back().color = color;
        splines.back().layer = layer;
    }
    else{
        block_definitions.back().splines.emplace_back(spline());
        block_definitions.back().splines.back().color = color;
        block_definitions.back().splines.back().layer = layer;
    }

    for (unsigned int i=0; i<spline_x.size();i++){
        control_point temp;
        temp.x = spline_x[i];
        temp.y = spline_y[i];
        temp.z = spline_z[i];
        push_control_point(temp);
    }

    for (unsigned int i=0; i<spline_knots.size();i++){
        knot temp;
        temp.val = spline_knots[i];
        push_knot(temp);
    }

    end_spline();
}

void dxf_parser::push_control_point(const control_point &cp){
    
    
    if (!reading_block){
        splines.back().x.push_back(cp.x);
        splines.back().y.push_back(cp.y);
        splines.back().z.push_back(cp.z);
    }
    else{
        block_definitions.back().splines.back().x.push_back(cp.x);
        block_definitions.back().splines.back().y.push_back(cp.y);
        block_definitions.back().splines.back().z.push_back(cp.z);
    }
}

void dxf_parser::push_knot(const knot &kt){
    
    if (!reading_block){
        splines.back().knots.push_back(kt.val);
    }
    else{
        block_definitions.back().splines.back().knots.push_back(kt.val);
    }
   
}

void dxf_parser::end_spline(){

    if (!reading_block){
        splines.back().process();
    }
    else{
        block_definitions.back().splines.back().process();
    }

}


dxf_parser::dxf_parser() {

    is_arc_next = false;
    is_arc = false;

    polyline_num=0;polyline_count=0;poly_flag=0;
    units=0;

    polyline_in=0;
    first_polyline=0;
    reading_block=0;
    layers.clear();
    layers_used.clear();
    layer_colors.clear();
    layer_widths.clear();

    layers_used.push_back(0);
    layers.push_back("0");
    layer_colors.push_back(0);
    layer_widths.push_back(1); 
    
    blocks_used.clear();
    block_definitions.clear();
    blockcolor = 250;
    reading_spline = 0;
    
}