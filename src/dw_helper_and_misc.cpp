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

void draw_window::capture_screen(){

    if (root->capture_screen){

        try{

            int windowWidth = this->w();
            int windowHeight = this->h();
            root->screen_w = windowWidth;
            root->screen_h = windowHeight;

            std::vector<GLfloat> image(windowWidth * windowHeight*3);
            glReadPixels(0, 0, windowWidth, windowHeight, GL_RGB, GL_FLOAT, image.data());

            root->screen_cap = image;
            Fl::lock();
            root->capture_screen = 0; //finished capturing the screen
            Fl::unlock();
        }
        catch (...){
            Fl::lock();
            root->capture_screen = -1; //something went wrong
            Fl::unlock();
            return;
        }
    }

} 


bool is_number(const std::string &str){
    return str.find_first_not_of("-.0123456789") == std::string::npos;
}


void draw_window::determine_floor(){
    flr_choice=program->floor_choice->value()-1;

    if (program->floor_choice->value()){
        all_flrs=0;
        minz=root->current->floor_index[flr_choice];
    }
    else{
        all_flrs=1;
        minz = root->current->minimum_z;
    }

}



