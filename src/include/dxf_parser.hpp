/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef DXF_PARSER_H
#define DXF_PARSER_H

#include "config.h"
#if defined(WIN32)
#include "stdafx.h"
#endif

#include <stdexcept>
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <algorithm>

#include <boost/lexical_cast.hpp>

#include "locations.hpp"

#undef min
#undef max

/* reference used to parse DXFs can be found here:

    https://images.autodesk.com/adsk/files/autocad_2012_pdf_dxf-reference_enu.pdf

   Slowly making way through important objects. More may be added in future releases */

class dxf_parser{
    
    double to_double(const std::string& str){
    
        try{
            return boost::lexical_cast<double>(str);
        }
        catch(...){
            error = 1;
            return 0.0;
        }
    }

    int to_int(const std::string& str){
        
        try{
            return boost::lexical_cast<int>(str);
        }
        catch(...){
            error = 1;
            return 0;
        }
    }

    template <typename T> int sgn(T val) {
        return (T(0) < val) - (val < T(0));
    }

    int polyline_in,first_polyline,polyline_num,polyline_count,poly_flag;
    int reading_block;
    int reading_spline;

    
    std::istream& getline(std::istream& is, std::string& t)
    {
        t.clear();
        
        std::istream::sentry se(is, true);
        std::streambuf* sb = is.rdbuf();
        
        for(;;) {
            int c = sb->sbumpc(); //consume and advance
            switch (c) {
                case '\n':
                    return is;
                case '\r':
                    if(sb->sgetc() == '\n') //examine next object without advance
                        sb->sbumpc(); //consume and advance in the case of windows line end \r\n
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

    std::string& ltrim(std::string &s)
    {
        auto it = std::find_if(s.begin(), s.end(),
                        [](char c) {
                            return !std::isspace<char>(c, std::locale::classic());
                        });
        s.erase(s.begin(), it);
        return s;
    }
     
    std::string& rtrim(std::string &s)
    {
        auto it = std::find_if(s.rbegin(), s.rend(),
                        [](char c) {
                            return !std::isspace<char>(c, std::locale::classic());
                        });
        s.erase(it.base(), s.end());
        return s;
    }
     
    std::string& trim(std::string &s) {
        return ltrim(rtrim(s));
    }

    struct block_in{
        std::string name;
        double x;
        double y;
        double z;
        block_in():name(""),x(0),y(0),z(0){}
    };

    struct insert{
        std::string name;
        double scale_x;
        double scale_y;
        double scale_z;
        double insert_x;
        double insert_y;
        double insert_z;
        double rotation;
        int cols;
        int rows;
        double space_dx;
        double space_dy;
        insert():name(""),scale_x(1),scale_y(1),scale_z(1),insert_x(0),insert_y(0),insert_z(0),rotation(0),cols(1),rows(1),space_dx(0),space_dy(0){}
    };

    struct polyline{
        int number,m,n,closed;
        polyline():number(0),m(0),n(0),closed(0){}
    };

    struct ellipse_in{
        double x,y,z;
        double major_x,major_y,major_z;
        double radius_ratio;
        double angle1,angle2;
        ellipse_in():x(0),y(0),z(0),major_x(0),major_y(0),major_z(0),radius_ratio(1.0),angle1(0.0),angle2(2*M_PI){}
    };

    struct layer_in{
        std::string name;
        layer_in():name(""){}
    };

    struct poly_point{
        double x,y,z,bulge;
        poly_point():x(0),y(0),z(0),bulge(0){}
    };

    struct control_point{
        double x,y,z;
        control_point():x(0),y(0),z(0){}
    };

    struct knot{
        double val;
        knot():val(0){}
    };

    layer_in layer_read;
    polyline polyline_read;
    insert insert_read;
    block_in block_read;
    line_in line_read;
    arc arc_read;
    circle circle_read;
    ellipse_in ellipse_read;
    spline spline_read;
    int reading_object;

    std::vector<double> poly_x;
    std::vector<double> poly_y;
    std::vector<double> poly_z;
    std::vector<double> poly_bulge;

    std::vector<double> spline_x;
    std::vector<double> spline_y;
    std::vector<double> spline_z;
    std::vector<double> spline_knots;

    int current_color;
    std::string current_layer;
    int current_paper_space;
    double ang_convert;

    int error;

    // temporary region testing

    // int intersect_test(double x1,double x2,double y1,double y2, double x3,double x4,double y3,double y4){//doesnt't need to be so fast
    
    //     int intersect=0;
        
    //     double x=((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));
    //     double y=((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/((x1-x2)*(y3-y4)-(y1-y2)*(x3-x4));
        
    //     if ((y>=std::min(y1,y2))&&(y<=std::max(y1,y2))&&(x>=std::min(x1,x2))&&(x<=std::max(x1,x2))&&(y>=std::min(y3,y4))&&(y<=std::max(y3,y4))&&(x>=std::min(x3,x4))&&(x<=std::max(x3,x4))){
    //         intersect=1; 
    //     }    
        
    //     return intersect;
        
    // }

    // void process_regions(){

    //     int count =regions.size();

    //     for (int ii = regions.size()-1;ii>=0;ii--){
    //         if (regions[ii].polygon.size()>2){
    //             int intersect=0;
    //             for (unsigned int i=1;i<regions[ii].polygon.size();i++){

    //                 for (unsigned int j=1;j<i;j++){
    //                     intersect += intersect_test(regions[ii].polygon[i-1].x,regions[ii].polygon[i].x,regions[ii].polygon[i-1].y,regions[ii].polygon[i].y,
    //                                                 regions[ii].polygon[j-1].x,regions[ii].polygon[j].x,regions[ii].polygon[j-1].y,regions[ii].polygon[j].y);
                        
    //                     if (intersect){
    //                         break;
    //                     }
    //                 }

    //                 if (intersect){
    //                     break;
    //                 }
    //             }

    //             for (unsigned int j=1;j<regions[ii].polygon.size();j++){
    //                 intersect += intersect_test(regions[ii].polygon.front().x,regions[ii].polygon.back().x,regions[ii].polygon.front().y,regions[ii].polygon.back().y,
    //                                             regions[ii].polygon[j-1].x,regions[ii].polygon[j].x,regions[ii].polygon[j-1].y,regions[ii].polygon[j].y);
                    
    //                 if (intersect){
    //                     break;
    //                 }
    //             }
    //             if (intersect){
    //                 regions[ii] = regions.back();
    //                 regions.pop_back();
    //             }
    //         }
    //         else{
    //             regions[ii] = regions.back();
    //             regions.pop_back();
    //         }
    //     }

    //     std::cout<<" read in "<<regions.size()<<" regions from "<<count<<std::endl;
    // }
    // end temporary region testing

public:
    
    int read_in(std::string);

    int units;

    // //temporary region test
    // std::vector<vec> current_polygon;
    // std::vector<region> regions;
    // //end temporary region test

    std::vector<line_in> lines;
    std::vector<arc> arcs;
    std::vector<circle> circles;
    std::vector<ellipse> ellipses;
    vec last_vertex,first_vertex;
    std::vector<block_definition> block_definitions;
    std::vector<block_entry> blocks;
    std::vector<std::string> layers;
    std::vector<spline> splines;
    std::vector<int> layers_used;
    std::vector<int> blocks_used;

    std::vector<int> layer_colors;
    std::vector<int> layer_widths;
    //std::vector<int> block_colors;
    int blockcolor;
    int polylinecolor;
    int blockwidth;

    bool is_arc,is_arc_next;
    double bulge,bulge_next;
    dxf_parser();
    
    arc bulge_to_arc(line_in,double);
   

    void push_line(const line_in&);
    void push_polyline(const polyline&);
    void push_poly_point(const poly_point&);
    void push_arc(const arc&);
    void push_circle(const circle&);
    void push_ellipse(const ellipse_in&);
    void start_block(const block_in&);
    void end_block();
    void push_insert(const insert&);
    void push_layer(const layer_in&);
    void push_spline(const spline&);
    void push_control_point(const control_point&);
    void push_knot(const knot&);
    void end_spline();

    
};


#endif /*DXF_PARSER_MODEL_H*/


