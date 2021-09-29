/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef PLOT_H
#define PLOT_H

#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include <FL/glut.H>

#include "container.hpp"

#undef min
#undef max

class base;

class plot_window : public Fl_Gl_Window {
    
    //MSVC compliance
    static const GLfloat cursor1[15*3*3];
    static const GLfloat cursor2[15*3*3];

    //methods
    void draw();
    int handle(int);
    
    //data
    double frac_x,frac_y;
    double minx,maxx,miny,maxy,minz,maxz;
    double minx_overall,maxx_overall,miny_overall,maxy_overall;
    int h_initial,w_initial;
    double zoom_level;
    int zoom;
    int drag;
    int zoom_mag;
    double xdown,ydown,xdragdown,ydragdown,xdragdown_old,ydragdown_old;
    double minx_down,maxx_down,miny_down,maxy_down;

    double midx,midy,midz;
    double ang,ang_old,rot[3],old_rot[3],cv[3],ov[3];
    double min_zoom,max_zoom;

    double xaxis_min,xaxis_max,yaxis_min,yaxis_max,zaxis_min,zaxis_max;
    
    int hist_select;
    
    int got_formula;
    double plot_m,plot_c,plot_r2,plot_r2a;
    
    void stat_functions();
    
    void setzoom();
    void draw_axes();
    void draw_grid();
    void draw_points();
    void drawcuboid(double,double,double,double,double,double);
    void drawcuboidedge(double,double,double,double,double,double,double);
    void drawcross(double, double, double, double,double,double);
    void draw_hist();
    void drawcursor(double);
    double spline_func(double,std::vector<double>&,std::vector<double>&);
    
    
    container* root;
    base* program;

public:
    
    int first_draw;
    double z_gap;
    int flr_choice;
    int all_flrs;
    int selector;
    int selected;
    double ang_x,ang_y,ang_z;
    int num_bins;
    
    std::vector<std::vector<double> >hist_x;
    std::vector<std::vector<double> > hist_n;
    std::vector<std::vector<double> > hist_nc;
    std::vector<std::vector<double> > bin_min,bin_max,bin_sum;
    std::vector<double> hist_gap,hist_min,hist_max;
    std::vector<int> sum_hist;
    std::vector<std::vector<double> > poly_hist_x;
    std::vector<std::vector<double> > poly_hist_y;
    
    double h_mean[2];
    double h_std1[2];
    double h_std2[2];
    
    void histogram();
    
    plot_window(int X, int Y, int W, int H, const char *L,container* root_,base* program_): Fl_Gl_Window(X, Y, W, H, L),root(root_),program(program_) {

        frac_x=0;frac_y=0;
        minx=0;maxx=0;miny=0;maxy=0;minz=0;maxz=0;
        minx_overall=0;maxx_overall=0;miny_overall=0;maxy_overall=0;
        h_initial=0;w_initial=0;
        zoom_level=0;
        zoom=0;
        drag=0;
        zoom_mag=0;
        xdown=0;ydown=0;xdragdown=0;ydragdown=0;xdragdown_old=0;ydragdown_old=0;
        minx_down=0;maxx_down=0;miny_down=0;maxy_down=0;

        midx=0;midy=0;midz=0;
        ang=0;ang_old=0;
        rot[0]=0;rot[1]=0;rot[2]=0;
        old_rot[0]=0;old_rot[1]=0;old_rot[2]=0;
        cv[0]=0;cv[1]=0;cv[2]=0;
        ov[0]=0;ov[1]=0;ov[2]=0;
        min_zoom=0;max_zoom=0;

        xaxis_min=0;xaxis_max=0;yaxis_min=0;yaxis_max=0;zaxis_min=0;zaxis_max=0;
        
        hist_select=0;
        
        got_formula=0;
        plot_m=0;plot_c=0;plot_r2=0;plot_r2a=0;

        first_draw=0;
        z_gap=0;
        flr_choice=0;
        all_flrs=0;
        selector=0;
        selected=0;
        ang_x=0;ang_y=0;ang_z=0;
        num_bins=0;


        h_mean[0]=0;h_mean[1]=0;
        h_std1[0]=0;h_std1[1]=0;
        h_std2[0]=0;h_std2[1]=0;

        first_draw=1;
        zoom=0;
        drag=0;
        zoom_mag=0;
        zoom_level=1.0;
        ang_x=0;ang_y=0;ang_z=0;
        
        
        min_zoom=0.2;
        max_zoom=60;
        num_bins=20;

        
        
        for (int i=0;i<4;i++){
            std::vector<double> temp;

            poly_hist_y.push_back(temp);
            poly_hist_x.push_back(temp);
            bin_min.push_back(temp);
            bin_max.push_back(temp);
            bin_sum.push_back(temp);
            hist_n.push_back(temp);
            hist_nc.push_back(temp);
            hist_x.push_back(temp);
            sum_hist.push_back(0);
            hist_gap.push_back(0);
            hist_min.push_back(0);
            hist_max.push_back(0);
        }
        
    
        hist_select=-1;
    }
    
   
};


#endif /*PLOT_H*/

