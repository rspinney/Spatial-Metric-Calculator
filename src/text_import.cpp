/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/base.hpp"

std::istream& base::tool_getline(std::istream& is, std::string& t)
{
    t.clear();
    
    //consume string until a new line (of either windows/mac/linux type) or a terminating character },],) is found
    
    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();
    
    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
            case '}':
                return is;
            case ']':
                return is;
            case ')':
                return is;
            case '\n':
                return is;
            case '\r':
                if(sb->sgetc() == '\n')
                    sb->sbumpc();
                return is;
            case EOF:
                // Also handle the case when the last line has no line ending
                if(t.empty())
                    is.setstate(std::ios::eofbit);
                return is;
            default:
                t += (char)c;
        }
    }
}


int base::readin_walls(std::vector<wall> &standing,std::vector<wall>& sitting,int wall_type, int wall_floor,std::string str_desks){
    
    
    std::stringstream output;
    
    std::ifstream in_data;
    std::string str;
    
    
    output.str("");output<<"OPENING: "<<str_desks<<std::endl;
    push_time();
    push_output(output.str());
    
    
    try {
        in_data.open(str_desks.c_str());
    }
    catch (const std::ifstream::failure &e) {
        //std::cout << "Error opening "<<str_desks<<std::endl;
        output.str("");output<<"error opening "<<str_desks<<std::endl;
        push_time();
        push_output(output.str());
        
        return 1;
    }
    
    
    // determine format
    try{
        tool_getline(in_data, str);
    }
    catch(...){
        
        output.str("");output<<"error opening "<<str_desks<<std::endl;
        push_time();
        push_output(output.str());
        
        return 1;
    }
    
    
    
    for (unsigned int i=0;i<str.size();i++){
        if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
            str[i]=' ';
        }
        
    }
    
    
    
    int format =-1;
    double x1,x2,x3,x4;
    std::string test_string;
    
    std::istringstream iss(str);
    if (!(iss >> x1 >> x2 >>x3>> x4)){
        
        //std::cout << "Error opening "<<str_desks<<std::endl;
        output.str("");output<<"error opening "<<str_desks<<std::endl;
        push_time();
        push_output(output.str());
    }
    else{
        format=0;
    }
        
        
        in_data.close();
        
        if (format!=0){
            output.str("");output<<"error opening "<<str_desks<<std::endl;
            push_time();
            push_output(output.str());
            
            return 1;
        }
        
        //end determine format
        
        
        std::vector<wall> temp_wall;
        
        
        try {
            in_data.open(str_desks.c_str());
        }
        catch (const std::ifstream::failure &e) {
            output.str("");output<<"error opening "<<str_desks<<std::endl;
            push_time();
            push_output(output.str());
            
            return 1;
        }
        
        int c=1;
        int counter=0;
        while (c)
        {
            
            try{
                tool_getline(in_data, str);
            }
            catch(...){
                
                c=0;
                break;
            }
            
            for (unsigned int i=0;i<str.size();i++){
                if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
                    str[i]=' ';
                }
                
            }
            
            std::istringstream iss(str);
            std::string label;
            label.assign("");
            double x1,y1,x2,y2;
            
            int error=0;
            
            
            switch (format){
                case 0:
                    if (!(iss >> x1 >> y1>>x2>>y2)) { error = 1;}
                    break;
                    
                default:
                    error=1;
            }
                      
            
            if (error){
                break;
            }
            
            
            wall temp;

            double z = root.current->floor_index[wall_floor];
            temp.start.x=x1;
            temp.start.y=y1;
            temp.start.z=z;
            temp.end.x=x2;
            temp.end.y=y2;
            temp.end.z=z;
            
 
            
            if ((wall_type==1)||(wall_type==3)){
                temp.transparent=1;
            }
            else{
                temp.transparent=0;
            }
            
            temp.start.floor=wall_floor;
            temp.end.floor=wall_floor;
            

            walls_mutex.lock();

            if (wall_type<2){
                standing.push_back(temp);
            }
            else{
                sitting.push_back(temp);
            }

            walls_mutex.unlock();
            counter++;
            
            
            
            
        }
        
        in_data.close();
    
        
        output.str("");output<<"Read in "<<counter<<" walls."<<std::endl;
        push_time();
        push_output(output.str());
        
        return 0;
        
    }
    
    

void base::open_walls(int type){

    if (root.calculating){
        push_time();
        push_output("Busy! Wait for current process to finish.\n");
        return;
    }
    
    wall_type = type;
    wall_floor =  floor_choice->value()-1;
    
    if (wall_floor<0){
        push_time();
        push_output("Select a floor!\n");
        return;
    }

    Fl_Native_File_Chooser fnfc;
    fnfc.title("Open file");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    if ( fnfc.show() ) return;
    desks_file=std::string(fnfc.filename());
    std::stringstream s;
    root.calculating=LOADING_DESKS;
    calc_message_on();

    test_thread = std::thread(&base::read_walls_pass_static,this);
    test_thread.detach();
    
}

void base::open_walls_standing_static(Fl_Widget* /*widget*/, void* data){
    base* program = static_cast<base*> (data);
    program->open_walls_standing();
}

void base::open_walls_standing_transparent_static(Fl_Widget* /*widget*/, void* data){
    base* program = static_cast<base*> (data);
    program->open_walls_standing_transparent();
}

void base::open_walls_sitting_static(Fl_Widget* /*widget*/, void* data){
    base* program = static_cast<base*> (data);
    program->open_walls_sitting();
}

void base::open_walls_sitting_transparent_static(Fl_Widget* /*widget*/, void* data){
    base* program = static_cast<base*> (data);
    program->open_walls_sitting_transparent();
}

void base::open_walls_standing(){
    
    int proceed = choice("Proceeding will add opaque standing height barriers on the current floor. Continue?","Read in","Cancel",0,"Add barriers?");
    
    if (proceed){
        return;
    }
    open_walls(0);
    
}

void base::open_walls_standing_transparent(){
    
    int proceed = choice("Proceeding will add transparent standing height barriers on the current floor. Continue?","Read in","Cancel",0,"Add barriers?");
    
    if (proceed){
        return;
    }
    open_walls(1);
    
}

void base::open_walls_sitting(){
    
    int proceed = choice("Proceeding will add opaque sitting height barriers on the current floor. Continue?","Read in","Cancel",0,"Add barriers?");
    
    if (proceed){
        return;
    }
    open_walls(2);
    
}

void base::open_walls_sitting_transparent(){
    
    int proceed = choice("Proceeding will add transparent sitting height barriers on the current floor. Continue?","Read in","Cancel",0,"Add barriers?");
    
    if (proceed){
        return;
    }
    open_walls(3);
    
}


void* base::read_points_pass_static(void* data){

    base* program = static_cast<base*> (data);
    program->read_points_pass();

    return NULL;
}


void* base::read_points_pass(){

    container* root = &(this->root);//shadowing
    
    std::vector<vec>* points = &(root->current->points);
    
    //clear everything here

    Fl::lock();
    floor_choice->value(0);
    floor_choice_cb(0);
    if (three_d_choice->value()==3){
        three_d_choice->value(0);
        three_d_cb();
    }
    
    root->current->clear();
    draw->redraw();
    Fl::unlock();
    
    readin_points(*points,points_file);
    
    Fl::lock();
    set_floor_choice();
    show_map->value(1);
    Fl::unlock();
    find_floor_extremes();
    draw->first_draw=1;
    
    prepare_undo();

    Fl::awake(show_warning_map_cb,this);
    
    Fl::awake();
    root->calculating=FREE;
    calc_message_off();
    Fl::awake(&redraw_from_thread,this);

    return NULL;
}

void* base::read_matrix_pass_static(void* data){

    base* program = static_cast<base*> (data);
    program->read_matrix_pass();

    return NULL;
}


void* base::read_matrix_pass(){


    container* root = &(this->root);//shadowing
    
    std::vector<vec>* points = &(root->current->points);
    std::vector<vertex>* adj_list = &(root->current->adj_list);

    adj_list->clear();
    std::vector<vertex>().swap(*adj_list);

    readin_matrix(*points,*adj_list,matrix_file);

    prepare_undo();
    
    Fl::lock();
    draw->first_draw=1;
    
    Fl::unlock();
    Fl::awake();
    root->calculating=FREE;
    calc_message_off();
    Fl::awake(&redraw_from_thread,this);

    return NULL;
}


void base::open_adjlist_cb_static(Fl_Widget* /*widget*/, void* data){
    
    base* program = static_cast<base*> (data);

    program->open_adjlist_cb();
}

void base::open_adjlist_cb() {
    
    container* root = &(this->root); //shadowing
    
    if (root->calculating){
        push_time();
        push_output("Busy! Wait for current process to finish.\n");
        calc_cb();
        return;
    }

    int i=choice("Read in adjacency list? Doing so will clear all current edges.","Yes","Cancel",0,"Read adjacency list?");
    
    if (i!=1){        

        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
       
        matrix_file=std::string(fnfc.filename());
        std::stringstream s;
        root->calculating=LOADING_ADJ;
        calc_message_on();

        test_thread = std::thread(&base::read_adj_list_pass_static,this);
        test_thread.detach();

    }
    
    
    
}

void* base::read_adj_list_pass_static(void* data){

    base* program = static_cast<base*> (data);
    program->read_adj_list_pass();

    return NULL;
}


void* base::read_adj_list_pass(){

    container* root = &(this->root);//shadowing

    std::vector<vec>* points = &(root->current->points);
    std::vector<vertex>* adj_list = &(root->current->adj_list);
    
    adj_list->clear();
    std::vector<vertex>().swap(*adj_list);
    readin_adj_list(*points,*adj_list,matrix_file);
    
    prepare_undo();

    Fl::lock();
    draw->first_draw=1;
    Fl::unlock();
    Fl::awake(&redraw_from_thread,this);
    root->calculating=FREE;
    calc_message_off();
    
    return NULL;
}


void base::open_points_cb_static(Fl_Widget* /*widget*/, void* data){
    
    base* program = static_cast<base*> (data);

    program->open_points_cb();
}

void base::open_points_cb() {
    
    container* root = &(this->root); //shadowing
    
    if (root->calculating){
        push_time();
        push_output("Busy! Wait for current process to finish.\n");
        calc_cb();
        return;
    }
    
    int i=choice("Read in vertices? Doing so will reset project and clear all structures.\nThis is intended to start a project as new imported floor and vertex references cannot be ensured across all existing constructs","Read in","Cancel",0,"Read in vertices?");
    if (i!=1){
    
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
        points_file=std::string(fnfc.filename());
        std::stringstream s;
        
        root->calculating=LOADING_POINTS;
        calc_message_on();

        test_thread = std::thread(&base::read_points_pass_static,this);
        test_thread.detach();
    }
    
}

void base::open_matrix_cb_static(Fl_Widget* /*widget*/, void* data){
    
    base* program = static_cast<base*> (data);
    program->open_matrix_cb();
}

void base::open_matrix_cb() {
    
    container* root = &(this->root); //shadowing
    
    Fl_Native_File_Chooser fnfc;
    fnfc.title("Open file");
    fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
    if ( fnfc.show() ) return;
    
    matrix_file=std::string(fnfc.filename());
    root->calculating=LOADING_MATRIX;
    calc_message_on();

    test_thread = std::thread(&base::read_matrix_pass_static,this);
    test_thread.detach();    
    
}



void* base::read_locations_pass(int loc_index){
    
    container* root = &(this->root);//shadowing
    
    int i=readin_desks(root->current->points,root->current->locations[loc_index],desks_file);
    
    Fl::lock();
    show_locations[loc_index]->value(1);
    Fl::unlock();
    
    
    std::vector<location>& loc = root->current->locations[loc_index];
    
    locations_mutex.lock();

    for (unsigned int i=0;i<loc.size();i++){
        int floor = loc[i].loc.floor;
        loc[i].loc.z = root->current->floor_index[floor];
    }
    
    locations_mutex.unlock();

    prepare_undo();

    if (!i) Fl::awake(show_warning_vis_cb,this);
    
    root->calculating=FREE;
    Fl::awake(&redraw_from_thread,this);
    calc_message_off();
    return NULL;
    
}


void base::open_locations_cb_static(Fl_Widget* /*widget*/, void* data){
    packaged_base_ptr* pass = static_cast<packaged_base_ptr*>(data); 
    base* program = pass->ptr;
    int loc_index = pass->option;
    program->open_locations_cb(loc_index);
}


void base::open_locations_cb(int loc_index){

    container* root = &(this->root);//shadowing
    
    if (root->calculating){
        push_time();
        push_output("Busy! Wait for current process to finish.\n");
        calc_cb();
        return;
    }
    
    int i=1;

    switch (loc_index){
        case 0: i = choice("Read in origin location markers? Doing so will clear all current origin markers.","Read in","Cancel",0,"Read in locations?");break;
        case 1: i = choice("Read in destination 1 location markers? Doing so will clear all current destination 1 markers.","Read in","Cancel",0,"Read in locations?");break;
        case 2: i = choice("Read in destination 2 location markers? Doing so will clear all current destination 2 markers.","Read in","Cancel",0,"Read in locations?");break;
        case 3: i = choice("Read in destination 3 location markers? Doing so will clear all current destination 3 markers.","Read in","Cancel",0,"Read in locations?");break;
        case 4: i = choice("Read in destination 4 location markers? Doing so will clear all current destination 4 markers.","Read in","Cancel",0,"Read in locations?");break;
        case 5: i = choice("Read in destination 5 location markers? Doing so will clear all current destination 5 markers.","Read in","Cancel",0,"Read in locations?");break;
        case 6: i = choice("Read in destination 6 location markers? Doing so will clear all current destination 6 markers.","Read in","Cancel",0,"Read in locations?");break;
        case 7: i = choice("Read in destination 7 location markers? Doing so will clear all current destination 7 markers.","Read in","Cancel",0,"Read in locations?");break;
        case 8: i = choice("Read in destination 8 location markers? Doing so will clear all current destination 8 markers.","Read in","Cancel",0,"Read in locations?");break;
    }


    
    if (!i){
        
        Fl_Native_File_Chooser fnfc;
        fnfc.title("Open file");
        fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
        if ( fnfc.show() ) return;
        desks_file=std::string(fnfc.filename());
        std::stringstream s;
        root->calculating=LOADING_DESKS;
        calc_message_on();


        test_thread = std::thread(&base::read_locations_pass,this,loc_index);
        test_thread.detach();

    }



}


int base::readin_desks(std::vector<vec> &points,std::vector<location>& desks,std::string str_desks){
    
    std::stringstream output;
    
    std::ifstream in_data;
    std::string str;
    
    
    if (!points.size()){        
        push_time();
        push_output("No spatial graph vertices! Cannot associate location markers without a spatial graph. Failed.\n");        
        return 1;
    }
    
    output.str("");output<<"OPENING: "<<str_desks<<std::endl;
    
    push_time();
    push_output(output.str());
        
    try {
        in_data.open(str_desks.c_str());
    }
    catch (const std::ifstream::failure &e) {
        //std::cout << "Error opening "<<str_desks<<std::endl;
        output.str("");output<<"error opening "<<str_desks<<std::endl;
        push_time();
        push_output(output.str());
        
        return 1;
    }
    
    
    // determine format
    try{
        tool_getline(in_data, str);
    }
    catch(...){
        
        //std::cout << "Error opening "<<str_desks<<std::endl;
        output.str("");output<<"error opening "<<str_desks<<std::endl;
        
        push_time();
        push_output(output.str());
        
        
        return 1;
    }
    
    
    for (unsigned int i=0;i<str.size();i++){
        if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
            str[i]=' ';
        }
        if (str[i]==' '){
            str.erase(str.begin()+i);
        }
        
    }

       
    
    int format =-1;
    double x1,x2,x3;
    std::string test_string;
    int test_point;
    int in_index;
    int in_floor;
    
    std::istringstream iss(str);
    if (!(iss >> in_index>> test_string  >>test_point>> x1 >> x2 >>x3>>in_floor)){
        if (!(iss >> test_string  >> x1 >> x2 >>x3>>test_point)){
            std::istringstream iss(str);
            if (!(iss >> test_string >> x1 >>x2 >>x3)){
                std::istringstream iss(str);
                if (!(iss >>  x1 >> x2 >> x3>>test_point)){
                    
                    std::istringstream iss(str);
                    if (!(iss >> x1 >> x2 >>x3)){
                        //std::cout << "Error opening "<<str_desks<<std::endl;
                        output.str("");output<<"error opening "<<str_desks<<std::endl;
                        push_time();
                        push_output(output.str());
                        
                        return 1;
                    }
                    else{
                        format=0;
                    }
                }
                else{
                    //std::cout<<test_point<<" "<<x1<<" "<<x2<<" "<<x3<<std::endl;
                    format =1;
                }
            }
            else{
                format =2;
            }
        }
        else{
            format=3;
        }
    }
    else{
        format=4;
        
    }

    
    
    in_data.close();
    
    if ((format!=0)&&(format!=1)&&(format!=2)&&(format!=3)&&(format!=4)){
        //std::cout << "Error opening "<<str_desks<<std::endl;
        output.str("");output<<"error opening "<<str_desks<<std::endl;
        
        push_time();
        push_output(output.str());
        
        
        return 1;
    }
    
    std::vector<location> temp_desks;
    
    try {
        in_data.open(str_desks.c_str());
    }
    catch (const std::ifstream::failure &e) {
        //std::cout << "Error opening "<<str_desks<<std::endl;
        output.str("");output<<"error opening "<<str_desks<<std::endl;
        
        push_time();
        push_output(output.str());
        
        
        return 1;
    }

    int c=1;
    while (c)
    {
        
        try{
            tool_getline(in_data, str);
        }
        catch(...){
            
            c=0;
            break;
        }
        
        for (unsigned int i=0;i<str.size();i++){
            if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
                str[i]=' ';
            }

            if (str[i]==' '){
                str.erase(str.begin()+i);
                //i=i-1;
            }
            
        }
        
        std::istringstream iss(str);
        std::string label;
        label.assign("");
        int point = -1;
        double x,y,z;
        
        int error=0;
        int in_index;
        int in_floor;
        
        
        switch (format){
            case 4:
                if (!(iss >> in_index>> label  >>point >> x >> y >>z>>in_floor)) { error = 1 ;}//wwwwsss
                break;
            case 3:
                if (!(iss >> label >> x >> y >> z >> point)) { error = 1;}
                break;
            case 2:
                if (!(iss  >> x >> y >> z >> point)) { error = 1;}
                break;
            case 1:
                if (!(iss >> label >> x >> y >> z)) { error = 1;}
                break;
            case 0:
                if (!(iss >> x >> y >> z)) { error = 1;}
                break;
            default:
                error=1;
        }
        

        if (error){break;}
        
    
        if ((point == -1)||(point<0)||(point>=static_cast<int>(points.size())) ){
            double min=9e99,xx,yy,zz;
            double temp;
            for (unsigned int i=0;i<points.size();i++){
                
                xx= points[i].x;
                yy= points[i].y;
                zz= points[i].z;
                
                temp = (xx-x)*(xx-x)+(yy-y)*(yy-y)+(zz-z)*(zz-z);
                
                if (temp<min){min=temp;point=i;}
                
            }
        }
        
        location temp;
    
        temp.point = point;
        temp.loc.x = x;
        temp.loc.y = y;
        temp.loc.z = z;
        temp.label = label;
        temp.is_desk = 1;
        
        if ((point<static_cast<int>(points.size()))&&(point>=0)){
            int floor = points[point].floor;
            temp.loc.floor=floor;            
            temp_desks.push_back(temp);
        }      
        
    }
    
    in_data.close();
    
    locations_mutex.lock();
    desks=temp_desks;
    locations_mutex.unlock();
    
    output.str("");output<<"Read in "<<desks.size()<<" location markers."<<std::endl;
    
    push_time();
    push_output(output.str());
    
    return 0;
    
}

int base::readin_points(std::vector<vec> &points_,std::string str_points){
    
    
    std::stringstream output;
    
    std::ifstream in_data;
    std::string str;
    
    output.str("");output<<"OPENING: "<<str_points<<std::endl;
    push_time();
    push_output(output.str());

    
    in_data.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    
    
    
    std::vector<vec> points;
    
    try {
        in_data.open(str_points.c_str());
       
    }
    catch (const std::ifstream::failure &e) {
        //std::cout << "Error opening "<<str_points<<std::endl;
        output.str("");output<<"error opening "<<str_points<<std::endl;
        push_time();
        push_output(output.str());
        
        return 1;
    }
    
    // determine format
    try{
        tool_getline(in_data, str);
    }
    catch(...){
        
        //std::cout << "Error opening "<<str_points<<std::endl;
        output.str("");output<<"error opening "<<str_points<<std::endl;
        push_time();
        push_output(output.str());
        
        return 1;
    }
    
    

    
    for (unsigned int i=0;i<str.size();i++){
        if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
            str[i]=' ';
        }

    }
    
   
    
    int format =-1;
    double x1,x2,x3,x4,x5;
    std::istringstream iss(str);
    if (!(iss >> x1 >> x2 >> x3 >> x4 >>x5)){
        std::istringstream iss(str);
        if (!(iss >> x1 >> x2 >> x3 >> x4)){
            std::istringstream iss(str);
            if (!(iss >> x1 >> x2 >> x3)){
                //std::cout << "Error opening "<<str_points<<std::endl;
                output.str("");output<<"error opening "<<str_points<<std::endl;
                push_time();
                push_output(output.str());
                
                return 1;
            }
            else{
                format =0;
            }
        }
        else{
            format =1;
        }
    }
    else{
        format=2;
    }
    
    
    in_data.close();
    
    if ((format!=0)&&(format!=1)&&(format!=2)){
        //std::cout << "Error opening "<<str_points<<std::endl;
        output.str("");output<<"error opening "<<str_points<<std::endl;
        push_time();
        push_output(output.str());
        
        return 1;
    }
    
    //end determine format
    
    
    try {
        in_data.open(str_points.c_str());
    }
    catch (const std::ifstream::failure &e) {
        //std::cout << "Error opening "<<str_points<<std::endl;
        output.str("");output<<"error opening "<<str_points<<std::endl;
        push_time();
        push_output(output.str());
        
        return 1;
    }
    
    std::string delim;
    delim = '\r';
    
    int c=1;
    while (c)
    {
        
        try{
            tool_getline(in_data, str);
        }
        catch(...){
            
            c=0;
            break;
        }
        
        for (unsigned int i=0;i<str.size();i++){
            if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
                str[i]=' ';
            }

        }
        
        std::istringstream iss(str);
        int point,floor;
        double x,y,z;
        
        int error=0;
        
        if (format==0){
            
            if (!(iss >> x >> y >> z)) { error=1; }
            
        }
        else if (format ==1){
            
            if (!(iss >> point>> x >> y >> z )) { error=1; }
            
        }
        else if( format ==2){
            
            if (!(iss>> point >> x >> y >> z >> floor)) { error=1; }
            
        }
        
        
        
        if (error){break;}
        
        vec temp;
        temp.x=x;
        temp.y=y;
        temp.z=z;
        if (format==2){
            temp.floor=floor;
        }
        points.push_back(temp);
        
    }
    
    in_data.close();
    
    output.str("");output<<"read in "<<points.size()<<" vertices."<<std::endl;
    push_time();
    push_output(output.str());
    
        
    
    if (format!=2){
        
        int floor=0;
        
        double maxz=-9e99;
        for (unsigned int i=0;i<points.size();i++){
            if (points[i].z>maxz) maxz=points[i].z;
        }
        
        double minz=9e99,old_minz=-9e99;
        
        
        do{
            minz=9e99;
            for (unsigned int i=0;i<points.size();i++){
                if ((points[i].z<minz)&&(points[i].z>old_minz)) minz=points[i].z;
            }
            
            for (unsigned int i=0;i<points.size();i++){
                if (points[i].z==minz) points[i].floor=floor;
            }
            old_minz=minz;
            floor++;
           // std::cout<<old_minz<<" "<<maxz<<" "<<floor<<std::endl;
        }while(old_minz<maxz);
        //end floor asignment
        
    }
    
    points_mutex.lock();

    points_=points;
    
    points_mutex.unlock();

    return 0;
    
    
}

int base::readin_adj_list(std::vector<vec> &points_,std::vector<vertex> &adj_list,std::string str_matrix){

    std::stringstream output;
    
    std::ifstream in_data;
    std::string str;
    
    output.str("");output<<"OPENING: "<<str_matrix<<std::endl;
    push_time();
    push_output(output.str());
    
    in_data.exceptions ( std::ifstream::failbit | std::ifstream::badbit );
    
    
    try {
        in_data.open (str_matrix.c_str());
        
    }
    catch (const std::ifstream::failure &e) {
        //std::cout << "Error opening "<<str_matrix<<std::endl;
        output.str("");output<<"error opening "<<str_matrix<<std::endl;
        push_time();
        push_output(output.str());
        
        return 1;
    }
    
    // determine format
    try{
        tool_getline(in_data, str);
    }
    catch(...){
        
        output.str("");output<<"error opening "<<str_matrix<<std::endl;
        push_time();
        push_output(output.str());
        
        return 1;
    }
    
    for (unsigned int i=0;i<str.size();i++){
        if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
            str[i]=' ';
        }

    }
    
   
    
    int format =-1;
    int x1,x2,x3;
    std::istringstream iss(str);
    
    if (!(iss >> x1 >> x2 >> x3)){
        std::istringstream iss(str);
        if (!(iss >> x1 >> x2)){
            //std::cout<<"Line looks like: "<<str<<std::endl;
            std::cout << "Error opening "<<str_matrix<<std::endl;
            output.str("");output<<"error opening "<<str_matrix<<std::endl;
            push_time();
            push_output(output.str());
            
            return 1;
        }
        else{
            format =0;
        }
    }
    else{
        format =1;
    }
    
    
    
    
    in_data.close();
    
    if ((format!=0)&&(format!=1)){
        //std::cout << "Error opening "<<str_matrix<<std::endl;
        output.str("");output<<"error opening "<<str_matrix<<std::endl;
        push_time();
        push_output(output.str());
        
        return 1;
    }
    
    //end determine format

    try {
        in_data.open(str_matrix.c_str());
    }
    catch (const std::ifstream::failure &e) {
        //std::cout << "Error opening "<<str_matrix<<std::endl;
        output.str("");output<<"error opening "<<str_matrix<<std::endl;
        push_time();
        push_output(output.str());
        
        return 1;
    }



    std::vector<vertex> to_add;
    
    int c=1;
    while (c)
    {
        
        try{
            tool_getline(in_data, str);
        }
        catch(...){
            
            c=0;
            break;
        }
        
        
        
        for (unsigned int i=0;i<str.size();i++){
            if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
                str[i]=' ';
            }
        }
        
        
        std::vector<int> temp;
        
        std::istringstream iss(str);
        int point1=-1,point2=-1,index=-1;
    

        int error=0;
        
        if (format==0){
            
            if (!(iss >> point1 >> point2 )) { error=1; }
            
        }
        else if (format ==1){
            
            if (!(iss >> index >> point1 >> point2 )) { error=1; }
            
        }

        if (error){break;}


        int include =1;
        
        if ((point1>-1)&&(point2>-1)){
        
            if ((point1>static_cast<int>(points_.size()-1))||(point1<0)){
                include=0;
            }
            if ((point2>static_cast<int>(points_.size()-1))||(point2<0)){
                include=0;
            }
            
            for (unsigned int i=0;i<adj_list.size();i++){
                if (((adj_list[i].start==point1)&&(adj_list[i].end==point2))||((adj_list[i].end==point1)&&(adj_list[i].start==point2))){
                    include=0;
                }
                
                
            }
            for (unsigned int i=0;i<to_add.size();i++){
                if (((to_add[i].start==point1)&&(to_add[i].end==point2))||((to_add[i].end==point1)&&(to_add[i].start==point2))){
                    include=0;
                }
            }
            if (include){
                vertex temp;
                temp.start=point1;
                temp.end=point2;
                to_add.push_back(temp);
            }
        }
        
    }
    
    in_data.close();
    
    
    for (unsigned int i=0;i<to_add.size();i++){
        adj_list.push_back(to_add[i]);
    }
    
    push_time();
    std::stringstream sss;
    sss<<"Added "<<to_add.size()<<" unique edges."<<std::endl;
    push_output(sss.str());
    
    return 0;
}

    
int base::readin_matrix(std::vector<vec> &points_,std::vector<vertex> &adj_list,std::string str_matrix){
    
    std::stringstream output;
    
    std::ifstream in_data;
    std::string str;
    
    output.str("");output<<"OPENING: "<<str_matrix<<std::endl;
    push_time();
    push_output(output.str());
    
    in_data.exceptions ( std::ifstream::failbit | std::ifstream::badbit );

    
    try {
        in_data.open (str_matrix.c_str());
        
    }
    catch (const std::ifstream::failure &e) {
        //std::cout << "Error opening "<<str_matrix<<std::endl;
        output.str("");output<<"error opening "<<str_matrix<<std::endl;
        push_time();
        push_output(output.str());
        
        return 1;
    }
    
   
    points_mutex.lock();
    adj_mutex.lock();
    
    int line =0;
    int c=1;
    while (c)
    {
        
        try{
            tool_getline(in_data, str);
        }
        catch(...){
            
            c=0;
            break;
        }
        
        for (unsigned int i=0;i<str.size();i++){
            if ((str[i]==',')||(str[i]==';')||(str[i]=='[')||(str[i]=='{')||(str[i]=='(')||(str[i]=='\t')){
                str[i]=' ';
            }
        }
        
        

        std::vector<int> temp;
        
        std::istringstream iss(str);
        int ii;
        
        int column=0;
        while (iss>>ii){
           
            if ((column>line)&&(column < static_cast<int>(points_.size()))&&(ii>0)){
                vertex temp;
                temp.start = line;
                temp.end = column;
                adj_list.push_back(temp);
            }
            column++;
        }
        
        
        line++;
        if (line>=static_cast<int>(points_.size())){
            break;
        }
    }


    adj_mutex.unlock();
    points_mutex.unlock();
    
    
    in_data.close();
    
    std::stringstream s;
    s<<"Added "<<adj_list.size()<<" edges"<<std::endl;
    push_time();
    push_output(s.str().c_str());
    
    return 0;
    
}





