/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef LOCATIONS_H
#define LOCATIONS_H

#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <math.h>
#include <vector>
#include <string>

#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#include "matrix.hpp"

#undef min
#undef max

#ifndef M_PI
    #define M_PI 3.141592653589793238
#endif

class base;

struct triangle{
    friend class boost::serialization::access;
    double x1,x2,x3,y1,y2,y3,z1,z2,z3; //three points
    
    
    template<class Archive>
    void serialize(Archive & ar, const uint32_t /*version*/)
    {
        ar & x1 & x2 & x3 & y1 & y2 & y3 & z1 & z2 & z3;
    }
    triangle(){x1=0;x2=0;x3=0;y1=0;y2=0;y3=0;z1=0;z2=0;z3=0;};
};

BOOST_CLASS_TRACKING(triangle, boost::serialization::track_never)


struct vec{
    friend class boost::serialization::access;
    double x;
    double y;
    double z;
    int32_t floor;
    
    template<class Archive>
    void serialize(Archive & ar, const uint32_t /*version*/)
    {// note, version is always the latest when saving
        ar & x; ar & y; ar & z; ar & floor;
    }
    
    vec(){x=0;y=0;z=0;floor=0;};
    
};

BOOST_CLASS_TRACKING(vec, boost::serialization::track_never)

struct vertex{
    friend class boost::serialization::access;
    int32_t start;
    int32_t end;
    
    
    bool operator<( const vertex & n ) const {
        
        if ((this->start<n.start)||(this->start>n.start)){
            return this->start < n.start;   // for example
        }
        else{
            return this->end < n.end;
        }
    }
    
    template<class Archive>
    void serialize(Archive & ar, const uint32_t /*version*/)
    {// note, version is always the latest when saving
        ar &  start;ar &  end;
        
    }
   
    vertex(){
        start=0;
        end=0;
    };
};

BOOST_CLASS_TRACKING(vertex, boost::serialization::track_never)

//temporary region test
// struct region{
//     std::vector<vec> polygon;
//     int layer;
//     int block;
// };
//end temporary region test

struct location{
    friend class boost::serialization::access;
    vec loc;
    std::string label;
    int32_t point;
    int32_t segment;
    std::vector<int32_t> vis_sitting;
    std::vector<int32_t> vis_standing;
    std::vector<int32_t> vis_standing_trans;
    std::vector<int32_t> vis_sitting_trans;
    
    std::vector<std::vector<int32_t> > quick_vis;
    
    
    int is_desk;
    
    
    template<class Archive>
    void serialize(Archive & ar, const uint32_t){
        
        ar &  loc;
        ar &  label;
        ar &  point;
        ar &  segment;
        ar &  vis_sitting;
        ar &  vis_standing;
        ar &  quick_vis;
    }
    

    location(){is_desk=0;point=0;segment=0;label="NULL";};
    
};

BOOST_CLASS_TRACKING(location, boost::serialization::track_never)

struct wall{
    friend class boost::serialization::access;
    vec start;
    vec end;
    int32_t transparent;   
    
    template<class Archive>
    void serialize(Archive & ar, const uint32_t /*version*/)
    {
        // note, version is always the latest when saving
        ar &  start;
        ar &  end;
        ar & transparent;
          
    }
    
    wall(){
        transparent=0;
    };
};

BOOST_CLASS_TRACKING(wall, boost::serialization::track_never)

enum point_type{NONE, FIRST, LAST,BOTH };

struct floor_line{

    vec start;
    vec end;
    int32_t layer;
    int color;
    int block_color;
    int block;
    int is_spline;
    point_type point;

    floor_line():color(0),block_color(0),is_spline(0),point(BOTH){};
};


struct line_in{
    friend class boost::serialization::access;
    vec start;
    vec end;
    int32_t layer;
    int color;


    template<class Archive>
    void save(Archive & ar, const unsigned int ) const
    {
        ar &  start &  end & layer & color;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        if (version<1){

            ar &  start &  end & layer;
            this->color = 250;

        }
        else{
            ar &  start &  end & layer & color;
        }

    }


    BOOST_SERIALIZATION_SPLIT_MEMBER()


    line_in(){
        layer=0;
        color=250;
    };
  
};

BOOST_CLASS_TRACKING(line_in, boost::serialization::track_never)
BOOST_CLASS_VERSION(line_in,1)


struct arc{
    friend class boost::serialization::access;
    double x,y,z,r,ang1,ang2;
    int32_t layer;
    int color;

    template<class Archive>
    void save(Archive & ar, const unsigned int ) const
    {
        ar &  x & y & z & r & ang1 & ang2 & layer & color;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        if (version<1){

            ar &  x & y & z & r & ang1 & ang2 & layer;
            ang1 *= M_PI/180.0;
            ang2 *= M_PI/180.0;
            while (ang1>2*M_PI){ //angle 2 has to be bigger
                ang1-=2*M_PI;
            }

            while (ang2>2*M_PI){ //angle 2 has to be bigger
                ang2-=2*M_PI;
            }

            while (ang1<0){ //angle 2 has to be bigger
                ang1+=2*M_PI;
            }

            while (ang2<0){ //angle 2 has to be bigger
                ang2+=2*M_PI;
            }

            while (ang1>ang2){ //angle 2 has to be bigger
                ang1-=2*M_PI;
            }
            this->color = 250;

        }
        else{
            ar &  x & y & z & r & ang1 & ang2 & layer & color;
        }

    }


    BOOST_SERIALIZATION_SPLIT_MEMBER()
   
    arc(){
        x=0;y=0;z=0;r=0;ang1=0;ang2=0;layer=0;color=250;
    };
};

BOOST_CLASS_TRACKING(arc, boost::serialization::track_never)
BOOST_CLASS_VERSION(arc,1)


struct circle{
    friend class boost::serialization::access;
    double x,y,z,r;
    int32_t layer;
    int color;

    template<class Archive>
    void save(Archive & ar, const unsigned int ) const
    {
        ar &  x & y & z & r & layer & color;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        if (version<1){

            ar &  x & y & z & r & layer;
            this->color = 250;

        }
        else{
            ar &  x & y & z & r & layer & color;
        }

    }


    BOOST_SERIALIZATION_SPLIT_MEMBER()

    circle(){
        x=0;y=0;z=0;r=0;layer=0;color=250;
    };
   
};

BOOST_CLASS_TRACKING(circle, boost::serialization::track_never)
BOOST_CLASS_VERSION(circle,1)

struct ellipse{
    friend class boost::serialization::access;

    double x,y,z,r1,r2,ang1,ang2,phi,cos_phi,sin_phi;
    int32_t layer;
    int color;
    
    template<class Archive>
    void serialize(Archive & ar, const uint32_t /*version*/)
    {
        ar &  x & y & z & r1 & r2 & ang1 & ang2 & phi & cos_phi & sin_phi & layer & color;
        
    }
    
    ellipse(){
        x=0;y=0;z=0;r1=0;r2=0;ang1=0;ang2=0;phi=0;layer=0;cos_phi=1.0;sin_phi=0.0;color=250;
    };
   
};

BOOST_CLASS_TRACKING(ellipse, boost::serialization::track_never)


struct spline{

    friend class boost::serialization::access;

    int color;
    int layer;
    int degree;
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<double> knots;
    double approx_size;
    double t_start,t_end,t_range;
    spline(){
        color= 250;layer=0;approx_size = 0;degree=0;t_start=0;t_end=0;t_range=0;
    };

    template<class Archive>
    void serialize(Archive & ar, const uint32_t /*version*/)
    {
        ar &  color & layer & degree & x & y & z & knots & approx_size & t_start & t_end & t_range;
        
    }

    void process(){
        
        degree = knots.size() - x.size() - 1;

        double ti = knots.front();
        double tf = knots.back();

        double eps_t = 0.00001*(tf-ti);

        ti += eps_t;
        tf -= eps_t;

        t_start = ti;
        t_end = tf;
        t_range = tf-ti;

        int n = knots.size() * 5 * degree;

        double dt = t_range/static_cast<double>(n);

        double x0 = get_x(ti);
        double y0 = get_y(ti);

        double len = 0;

        for (int i=1; i<n+1; i++){
            double t =ti + i*dt;
            double x = get_x(t);
            double y = get_y(t);
            len += sqrt((x-x0)*(x-x0) + (y-y0)*(y-y0));
            
            x0=x;
            y0=y;
        }

        approx_size = len;

        //approx_size = std::max(maxx-minx,maxy-miny);

    }

    double spline_func(const std::vector<double> &CP,int i,int n,double t) const{

        if (n==0){
            if ((t>=knots[i]) && (t<knots[i+1])){
                return 1.0;
            }
            else{
                return 0.0;
            }
        }
        else{

            double A = 0;
            double B = 0;

            double C1 = 0;
            double del1 = (knots[i+n]-knots[i]);
            
            double C2 = 0;
            double del2 = (knots[i+n+1]-knots[i+1]);

            if (del1>0){
                C1 = ((t-knots[i])/del1);
                A = spline_func(CP,i,n-1,t);
            }
            
            if (del2>0){
                C2 = ((knots[i+n+1]-t)/del2);
                B = spline_func(CP,i+1,n-1,t);
            }

            return C1 * A + C2 * B;
           
        }
    }

    double get_x(double t) const{

        double sum = 0;

        for (unsigned int i=0;i<x.size();i++){
            sum += x[i] * spline_func(x,i,degree,t);
        }

        return sum;
    }

    double get_y(double t) const{

        double sum = 0;

        for (unsigned int i=0;i<y.size();i++){
            sum += y[i] * spline_func(y,i,degree,t);
        }

        return sum;
    }
};

BOOST_CLASS_TRACKING(spline, boost::serialization::track_never)

struct block_entry{
    
    friend class boost::serialization::access;
    std::string name;
    double scale_x;
    double scale_y;
    double scale_z;
    double insert_x;
    double insert_y;
    double insert_z;
    double rotation;
    int32_t list_no;
    int32_t layer;
    int color;

    matrix_2d matrix;


    template<class Archive>
    void save(Archive & ar, const unsigned int ) const
    {
        ar &  name & scale_x & scale_y & scale_z & insert_x & insert_y & insert_z & rotation & list_no & layer & matrix & color;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        if (version<1){
            ar &  name & scale_x & scale_y & scale_z & insert_x & insert_y & insert_z & rotation & list_no & layer;

            matrix_2d mat;
            rotate_matrix rot(this->rotation*M_PI/180.0);
            translate_matrix t1(this->insert_x,this->insert_y);
            translate_matrix t2(-this->insert_x,-this->insert_y);
            scale_matrix s(this->scale_x,this->scale_y);

            mat = mat.mult(t2);
            mat = mat.mult(s);
            mat = mat.mult(rot);
            mat = mat.mult(t1);

            this->matrix = mat;

            this->color = 250;

        }
        else{
            ar &  name & scale_x & scale_y & scale_z & insert_x & insert_y & insert_z & rotation & list_no & layer & matrix & color;
        }

    }


    BOOST_SERIALIZATION_SPLIT_MEMBER()

    block_entry(){

        scale_x=0;
        scale_y=0;
        scale_z=0;
        insert_x=0;
        insert_y=0;
        insert_z=0;
        rotation=0;
        list_no=0;
        layer=0;
        color=250;

    };
 
    
};

BOOST_CLASS_TRACKING(block_entry, boost::serialization::track_never)
BOOST_CLASS_VERSION(block_entry,1)

struct block_definition{
    friend class boost::serialization::access;
    std::string name;
    std::vector<line_in> lines;
    std::vector<arc> arcs;
    std::vector<circle> circles;
    std::vector<ellipse> ellipses; 
    std::vector<spline> splines; 
    double start_x,start_y,start_z;
    std::vector<block_entry> nested_blocks;
    

    template<class Archive>
    void save(Archive & ar, const unsigned int ) const
    {
        ar &  name & lines & arcs & circles & start_x & start_y & start_z & nested_blocks & ellipses & splines;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        if (version<1){
            ar &  name & lines & arcs & circles & start_x & start_y & start_z & nested_blocks;
        }
        else{
            ar &  name & lines & arcs & circles & start_x & start_y & start_z & nested_blocks & ellipses & splines;
        }

    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    
    block_definition(){start_x=0;start_y=0;start_z=0;};
    
};

BOOST_CLASS_TRACKING(block_definition, boost::serialization::track_never)
BOOST_CLASS_VERSION(block_definition,1)


struct plan{


    friend class boost::serialization::access;

    std::vector<std::vector<floor_line> >  line_only_version;


    std::vector<line_in> lines_original;
    std::vector<line_in> lines;
    double shift_x,shift_y,shift_z;
    int32_t unit_type,user_unit_type;
    std::vector<int32_t> layers;
    std::vector<std::string> layers_name;
    std::vector<int32_t> layers_used;

    
    
    std::vector<arc> arcs_original;
    std::vector<arc> arcs;
    
    std::vector<circle> circles_original;
    std::vector<circle> circles;
    
    /////// ?!?!?!

    std::vector<ellipse> ellipses_original;
    std::vector<ellipse> ellipses;

    std::vector<spline> splines_original;
    std::vector<spline> splines;

    std::vector<std::string> block_names;
    std::vector<int32_t> blocks_used;
    std::vector<int32_t> blocks_active;


    /////////


    //nested
    
    std::vector<block_entry> blocks;
    std::vector<block_entry> blocks_original;
    std::vector<block_definition> block_definitions;
    std::vector<block_definition> block_definitions_original;
    
    //end nested
    
    
    // //temporary region test
    //std::vector<region> regions;
    // //end temporary region test

    int fast_plan;

    
    template<class Archive>
    void save(Archive & ar, const unsigned int ) const
    {
        ar & shift_x & shift_y & unit_type & lines & circles & arcs & blocks & block_definitions & layers & layers_name & layers_used & ellipses & splines & block_names & blocks_used & blocks_active;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        if (version<1){
            ar & shift_x & shift_y & unit_type & lines & circles & arcs & blocks & block_definitions & layers & layers_name & layers_used;


            blocks_active.clear();
            for (size_t i=0;i<block_definitions.size();++i){ //active blocks must exist even if not in the file
                blocks_active.push_back(1);
            }
        }
        else{
            ar & shift_x & shift_y & unit_type & lines & circles & arcs & blocks & block_definitions & layers & layers_name & layers_used & ellipses & splines & block_names & blocks_used & blocks_active;   
        }

    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    plan(){
    
        unit_type=0;
        user_unit_type=0;
        
        shift_x=0;
        shift_y=0;
        shift_z=0;
        fast_plan = 0;
    }


    
    int scale(base*);
    void scale(double);
    void convert_to_lines();
private:
    int line_only_version_size();
    int add_block_nested(const block_entry&,int,const matrix_2d&,const double &, const double &,int);
    int addarc(const arc,double ,double ,matrix_2d,int,int,int);
    int addcircle(const circle,double ,double ,matrix_2d,int,int,int);
    int addline(const line_in,double ,double ,matrix_2d,int,int,int);
    int addellipse(const ellipse,double ,double ,matrix_2d,int,int,int);
    int addspline(const spline,double ,double ,matrix_2d,int,int,int);
    
    
};

BOOST_CLASS_TRACKING(plan, boost::serialization::track_never)
BOOST_CLASS_VERSION(plan, 1)



#endif //LOCATIONS_H

