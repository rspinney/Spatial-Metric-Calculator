/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef BUILDING_H
#define BUILDING_H

#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <math.h>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <vector>
#include <deque>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <ctime>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/access.hpp>

#include "locations.hpp"
#include "text.hpp"
#include "graph.hpp"
#include "errors.hpp"

#undef min
#undef max

class base;

struct metric_description{
    

    friend class boost::serialization::access;

    int32_t destination_crit,route,metric,visible,floor,stat,destination,range_option,basis,route_break,self_count,iterations,inverse,error;
    double eig_tolerance;
    double metric_range,metric_range2;
    double metric_turn;
    ERROR_CODE errors;

    template<class Archive>
    void save(Archive & ar, const unsigned int ) const
    {
        ar & destination_crit;
        ar & route;
        ar & metric;
        ar & visible & floor & stat & destination & range_option & basis & route_break & self_count & iterations & inverse & error;
        ar & eig_tolerance;
        ar & metric_range;
        ar & metric_turn;
        ar & metric_range2;
        ar & errors;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        if (version<1){
            ar & destination_crit;
            ar & route;
            ar & metric;
            ar & visible & floor & stat & destination & range_option & basis & route_break & self_count & iterations & inverse & error;
            ar & eig_tolerance;
            ar & metric_range;
            ar & metric_turn;
            metric_range2 = 0.0;
            errors = ERROR_CODE();

        }
        else if (version==1){
            ar & destination_crit;
            ar & route;
            ar & metric;
            ar & visible & floor & stat & destination & range_option & basis & route_break & self_count & iterations & inverse & error;
            ar & eig_tolerance;
            ar & metric_range;
            ar & metric_turn;
            ar & metric_range2;
            errors = ERROR_CODE();

        }
        else{
            ar & destination_crit;
            ar & route;
            ar & metric;
            ar & visible & floor & stat & destination & range_option & basis & route_break & self_count & iterations & inverse & error;
            ar & eig_tolerance;
            ar & metric_range;
            ar & metric_turn;
            ar & metric_range2;
            ar & errors;

        }

    }

    BOOST_SERIALIZATION_SPLIT_MEMBER()

    metric_description(){
        
        destination_crit=0;
        route=0;
        metric=0;
        visible=0;
        floor=0;
        stat=0;
        destination=0;
        range_option=0;
        basis=0;
        route_break=0;
        self_count=0;
        iterations=0;
        inverse=0;
        error=0;
        eig_tolerance=0;
        metric_range=0;
        metric_turn=0;
        metric_range2=0;
        errors = ERROR_CODE();
        
    }
    
};
BOOST_CLASS_TRACKING(metric_description, boost::serialization::track_never)
BOOST_CLASS_VERSION(metric_description, 2)

struct building_base{

public:

    int use_segments, use_turns;

    double turn_definition;
    
    std::vector<plan> floor_plans;
    plan floor_plan;
    std::vector<vec> points;
    std::vector<std::vector<int32_t> > matrix; //remove
    
    std::vector<vertex> adj_list;
    
    std::vector<std::vector<double> > metrics;
    std::vector<metric_description> metric_des;
    
    std::vector<double> floor_index;
    
    std::vector<std::vector<location> > locations;
    std::vector<wall> walls_sitting,walls_standing;
    
    std::vector<std::vector<vec> > floorarea_polygons; //SERIALIZE???
    std::vector<std::vector<triangle> > floorarea_triangles; //SERIALIZE????
    std::vector<int> floorarea_type; //?!?!?! 2021
    std::vector<double> floor_areas; //SERIALIZE????

    int32_t segs_valid, turns_valid;  //new
    int32_t vis_valid,map_valid,lines_valid;

    

};

struct building: public building_base{
    
    friend class boost::serialization::access;

    plan holding_plan;

    std::deque<building_base> memento;

     base* program;

    int current_frame;
        
    map_structures map;

    double mid_x,mid_y,mid_z,minimum_z,maximum_z,minimum_x,minimum_y,maximum_x,maximum_y;
    
    std::vector<std::vector<int32_t> > route;
    int32_t dual_route;

    std::vector<double> custom_metric;
    std::vector<double> plot_metric,log_plot_metric;
    double max_plot_metric,min_plot_metric;
    metric_description plot_metric_des;
    
    std::string save_file_string;
    std::string dxf_file_string;

    
     template<class Archive>
     void save(Archive & ar, const unsigned int ) const
     {
     

         ar &  floor_plans;
         ar &  points;
         ar &  adj_list;
         
         ar &  custom_metric;
        
         ar & locations[0];
         ar & locations[1];
         ar & locations[2];
         ar & locations[3];
         ar & locations[4];
         ar & locations[5];
         ar & locations[6];
         ar & locations[7];
         ar & locations[8];

         ar & floor_areas;
         ar &  walls_sitting;
         ar &  walls_standing;
         
         
         
         ar & floorarea_polygons;
         ar & floorarea_triangles;
         ar & floorarea_type;
         
         ar & mid_x & mid_y & mid_z & minimum_x & minimum_y & minimum_z & maximum_x & maximum_y & maximum_z;
         ar & floor_index;
         
         ar & metrics;          //these two added in new version
         ar & metric_des;
    
         
     }

     template<class Archive>
     void load(Archive & ar, const unsigned int version)
     {
      
         if (version<1){

           ar &  floor_plans;
             ar &  points;
             ar &  adj_list;
             ar &  map;
             ar &  custom_metric;

             ar & locations[0];
             ar & locations[1];
             ar & locations[2];
             ar & locations[3];
             ar & locations[4];
             ar & locations[5];
             ar & locations[6];
             ar & locations[7];
             ar & locations[8];

             ar &  walls_sitting;
             ar &  walls_standing;
             ar & mid_x & mid_y & mid_z & minimum_x & minimum_y & minimum_z & maximum_x & maximum_y & maximum_z;
             ar & floor_index;
             ar & vis_valid & map_valid & lines_valid;
             ar & floorarea_polygons;
             ar & floorarea_triangles;
             ar & floor_areas;

             floorarea_type.clear();
             for (size_t i=0;i<floorarea_triangles.size();i++){
                floorarea_type.push_back(0);
             }
             
        }
        else if (version==1){
                ar &  floor_plans;
             ar &  points;
             ar &  adj_list;
             ar &  custom_metric;

             ar & locations[0];
             ar & locations[1];
             ar & locations[2];
             ar & locations[3];
             ar & locations[4];
             ar & locations[5];
             ar & locations[6];
             ar & locations[7];
             ar & locations[8];

             ar & floor_areas;
             ar &  walls_sitting;
             ar &  walls_standing;
             
             ar & floorarea_polygons;
             ar & floorarea_triangles;
             
             
             ar & mid_x & mid_y & mid_z & minimum_x & minimum_y & minimum_z & maximum_x & maximum_y & maximum_z;
             ar & floor_index;
             
             ar & metrics;          //these two added in new version
             ar & metric_des;

             floorarea_type.clear();
             for (size_t i=0;i<floorarea_triangles.size();i++){
                floorarea_type.push_back(0);
             }
        }
        else{
               ar &  floor_plans;
             ar &  points;
             ar &  adj_list;
             ar &  custom_metric;

             ar & locations[0];
             ar & locations[1];
             ar & locations[2];
             ar & locations[3];
             ar & locations[4];
             ar & locations[5];
             ar & locations[6];
             ar & locations[7];
             ar & locations[8];

             ar & floor_areas;
             ar &  walls_sitting;
             ar &  walls_standing;
             
             ar & floorarea_polygons;
             ar & floorarea_triangles;
             ar & floorarea_type;
             
             
             ar & mid_x & mid_y & mid_z & minimum_x & minimum_y & minimum_z & maximum_x & maximum_y & maximum_z;
             ar & floor_index;
             
             ar & metrics;          //these two added in new version
             ar & metric_des;

        }
     }
     BOOST_SERIALIZATION_SPLIT_MEMBER()
     
     
    
    void temp_reset(){

        vis_valid=0;
        map_valid=0;
        lines_valid=0;
        segs_valid=0;
        turns_valid=0;
        mid_x=0;
        mid_y=0;mid_z=0;minimum_z=0;maximum_z=0;minimum_x=0;minimum_y=0;maximum_x=0;maximum_y=0;
        dual_route=0;
    }

    building(){
        locations = std::vector<std::vector<location> >(9,std::vector<location>(0));
        max_plot_metric=0;
        min_plot_metric=0;
        current_frame = 0;
    }
    building(base* program_):map(map_structures(program_)){
        program = program_;


        current_frame = 0;
        locations = std::vector<std::vector<location> >(9,std::vector<location>(0));

        max_plot_metric=0;
        min_plot_metric=0;

        use_segments = 1;
        use_turns = 1;

        turn_definition = 45.0;

        vis_valid=0;
        map_valid=0;
        lines_valid=0;
        segs_valid=0;
        turns_valid=0;
        
        std::vector<vec> temp;
        plan temp1;
        
        floor_plans.push_back(temp1);
        floorarea_polygons.push_back(temp);
        
        floor_index.push_back(0);
        floor_areas.push_back(0);
        
        save_file_string = "projectname.proj";
        
        mid_x=0;
        mid_y=0;mid_z=0;minimum_z=0;maximum_z=0;minimum_x=0;minimum_y=0;maximum_x=0;maximum_y=0;
        dual_route=0;
     

        clear();   
        
    }

    void assign(const building_base &input){
        
        this->points=input.points;
        this->adj_list=input.adj_list;
        

        this->locations=input.locations;

        this->walls_sitting=input.walls_sitting;
        this->walls_standing=input.walls_standing;        
        this->floor_index=input.floor_index;
             
        this->floor_index = input.floor_index;
        this->floor_plans = input.floor_plans;
        this->turn_definition = input.turn_definition;
        this->floorarea_polygons = input.floorarea_polygons;
        this->floorarea_triangles = input.floorarea_triangles;
        this->floorarea_type = input.floorarea_type;
        this->floor_areas = input.floor_areas;
        this->segs_valid=input.segs_valid; 
        this->turns_valid=input.turns_valid; 


        this->metrics = input.metrics;
        this->metric_des = input.metric_des;
 

    }

    void write_to_memento(){//(building_base &output){


        building_base output;

        output.points=this->points;
        output.adj_list=this->adj_list;
        output.locations = this->locations;
        output.walls_sitting=this->walls_sitting;
        output.walls_standing=this->walls_standing;        
        output.floor_index=this->floor_index;
        output.floor_index = this->floor_index;
        output.floor_plans = this->floor_plans;
        output.turn_definition = this->turn_definition;
        output.floorarea_polygons = this->floorarea_polygons;
        output.floorarea_triangles = this->floorarea_triangles;
        output.floorarea_type = this->floorarea_type;
        output.floor_areas = this->floor_areas;
        output.segs_valid=this->segs_valid; 
        output.turns_valid=this->turns_valid; 

        output.metrics = this->metrics;
        output.metric_des = this->metric_des;
 

        memento.push_back(std::move(output));
    }

    void clear(){
        
        floor_plans.clear();
        std::vector<plan>().swap(floor_plans);
        points.clear();
        std::vector<vec>().swap(points);
        matrix.clear();
        std::vector<std::vector<int32_t> >().swap(matrix);
        adj_list.clear();
        std::vector<vertex>().swap(adj_list);
        map.clear();
        custom_metric.clear();
        std::vector<double>().swap(custom_metric);
        metrics.clear();
        std::vector<std::vector<double> >().swap(metrics);
        metric_des.clear();
        std::vector<metric_description>().swap(metric_des);
        
        floor_index.clear();
        
        for (unsigned int i=0;i<locations.size();i++){
            locations[i].clear();
            std::vector<location>().swap(locations[i]);            
        }
        
        walls_sitting.clear();
        walls_standing.clear();
        
        std::vector<wall>().swap(walls_sitting);
        std::vector<wall>().swap(walls_standing);

        
        floorarea_polygons.clear(); 
        std::vector<std::vector<vec> >().swap(floorarea_polygons);
        floorarea_triangles.clear();
        std::vector<std::vector<triangle> >().swap(floorarea_triangles);
        floor_areas.clear(); 
        std::vector<int>().swap(floorarea_type);
        floorarea_type.clear(); 
        
        route.clear();
        
        
        vis_valid=1;
        map_valid=1;
        lines_valid=1;
        
        std::vector<vec> temp;
        plan temp1;
        
        floor_plans.push_back(temp1);
        floorarea_polygons.push_back(temp);
        
        floor_index.push_back(0);
        floor_areas.push_back(0);
        
    }
    
    
};
BOOST_CLASS_TRACKING(building, boost::serialization::track_never)
BOOST_CLASS_VERSION(building, 2)


struct file_in_out{
    building content;
    std::string file;
};



#endif //BUILDING_H

