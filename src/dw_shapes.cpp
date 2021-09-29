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


void draw_window::drawpyramid(double x, double y, double z, double length){
    
    double height=sqrt(0.6666666666)*length;
    glBegin(GL_TRIANGLES);
    
    glVertex3d(x-0.5*length,y-0.3333*length,z-0.5*height);
    glVertex3d(x+0.5*length,y-0.3333*length,z-0.5*height);
    glVertex3d(x,y,z+0.5*height);
    
    glVertex3d(x-0.5*length,y-0.3333*length,z-0.5*height);
    glVertex3d(x,y+0.66666*length,z-0.5*height);
    glVertex3d(x,y,z+0.5*height);
    
    glVertex3d(x+0.5*length,y-0.3333*length,z-0.5*height);
    glVertex3d(x,y+0.666666*length,z-0.5*height);
    glVertex3d(x,y,z+0.5*height);
    
    glVertex3d(x-0.5*length,y-0.3333*length,z-0.5*height);
    glVertex3d(x+0.5*length,y-0.3333*length,z-0.5*height);
    glVertex3d(x,y+0.666666*length,z-0.5*height);
    glEnd();
}


void draw_window::drawcube_no_begin_end(double x, double y, double z, double length){
    glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    
    
    glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    
    
    glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);

}

void draw_window::drawcube(double x, double y, double z, double length){
    glBegin(GL_TRIANGLES);
    glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    
    
    glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    
    
    glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    glEnd();
}


void draw_window::drawcube(double x, double y, double z, double length, GLuint texture){
    
    glBindTexture(GL_TEXTURE_2D,texture);
    
    
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0,0);
    glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(1,1);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    
    glTexCoord2f(0,0);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(0,1);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glTexCoord2f(0,1);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,1);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    
    glTexCoord2f(0,0);
    glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,1);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    
    glTexCoord2f(0,0);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,1);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    
    
    glTexCoord2f(0,0);
    glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glTexCoord2f(1,1);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    
    
    glTexCoord2f(0,0);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(1,0);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glTexCoord2f(0,1);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glTexCoord2f(1,1);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}


void draw_window::drawcubeedge(double x,double y,double z,double length,double lwidth, int a){
    glLineWidth(lwidth);
    
    glBegin(GL_LINES);
    
    if (!a){
        glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
        glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    }
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    if (!a){
        glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
        glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    }
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    if (!a){
        glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
        glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    }
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    
    glEnd();
}

void draw_window::drawcubeedge_no_begin_end(double x,double y,double z,double length, int a){

    if (!a){
        glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
        glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    }
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    if (!a){
        glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
        glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    }
    glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);
    if (!a){
        glVertex3d(x-0.5*length,y-0.5*length,z-0.5*length);
        glVertex3d(x-0.5*length,y-0.5*length,z+0.5*length);
    }
    glVertex3d(x-0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x-0.5*length,y+0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y-0.5*length,z+0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z-0.5*length);
    glVertex3d(x+0.5*length,y+0.5*length,z+0.5*length);

}