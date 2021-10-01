/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef GRAPH_H
#define GRAPH_H

#define EPSILON 1e-6

#include <math.h>
#include <cstdio>
#include <vector>
#include <unordered_map>
#include <map>
#include <mutex> 
#include <stdexcept>
#include <atomic>
#include <stdint.h>

#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#include "thread_utils.hpp"
#include "enums.hpp"
#include "errors.hpp"
#include "locations.hpp"

#undef min
#undef max

class base;
class container;

bool equal(double a, double b);

struct graph{
    
    friend class boost::serialization::access;
    
    std::vector<std::vector<int32_t> > neighbours;
    std::vector<std::unordered_map<int32_t,double> > weights;
    
    void clear(){
      
        neighbours.clear();
        weights.clear();
        
        std::vector<std::vector<int32_t> >().swap(neighbours);
        std::vector<std::unordered_map<int32_t,double> >().swap(weights);    
    }
    
    // template<class Archive>
    // void serialize(Archive & ar, const uint32_t ){
    //     ar & neighbours & weights;
    // }

    graph(){};
};

//BOOST_CLASS_TRACKING(graph, boost::serialization::track_never)

struct route_weight_number{
    double weight;
    int number;
    route_weight_number(){
        weight=0;
        number=0;
    }
};

struct graph_routes{
    
    int exist;
    
    friend class boost::serialization::access;
    std::vector<double> weight;
    std::vector<std::vector <int32_t> > path;
    std::vector<int32_t> mult_path;
    std::vector<int32_t> end_node;
    int32_t origin;
    int32_t node_origin;
    int32_t dual;
    
    // template<class Archive>
    // void serialize(Archive & ar, const uint32_t /*version*/)
    // {// note, version is always the latest when saving
    //     ar &  weight;ar &  path;ar &  mult_path;ar &  end_node; ar &  origin; ar & node_origin; ar & dual;
        
    // }
   
    graph_routes(){
        origin=0;node_origin=0;dual=0;exist=0;
    };
};

//BOOST_CLASS_TRACKING(graph_routes, boost::serialization::track_never)

struct return_iterate{
    std::vector<std::vector<int> > paths;
    int count;
    ERROR_CODE error;
};


struct map_structures{
  
    friend class boost::serialization::access;
  

    base* program;

    std::vector<graph_routes> angular_array,turns_array,distance_array,segments_array; //graphs based on points in segemnt map
    std::vector<graph_routes> seg_angular_array,seg_turns_array,seg_distance_array,seg_segments_array;  //graphs based on segments within segment map
    std::vector<graph_routes> seg_distance_3d_array;
    std::vector<vertex> dual_points, links;
    int32_t num_segments,num_points,num_links;
    
    std::vector<std::vector<int32_t> > points2dual_1;
    
    std::map<vertex,int32_t> points2dual;
    std::map<vertex,int32_t> points_2_segs;
    
    std::vector<graph_routes> crow_array,crow_3d_array,distance_3d_array;
    
    std::vector<graph_routes> seg_crow_array,seg_crow_3d_array;
    
    std::vector<int32_t> line_segments;
    
    graph G,G_dist,G_dist_3D,S,S_dist,S_dist_3D,G_dual_ang,G_dual_turn,S_dual_ang,S_dual_turn;
    
    
    std::vector<std::vector<route_weight_number> > choice_cache_G_dist;
    std::vector<std::vector<route_weight_number> > choice_cache_G_dist_3D;
    std::vector<std::vector<route_weight_number> > choice_cache_G;
    std::vector<std::vector<route_weight_number> > choice_cache_G_ang;
    std::vector<std::vector<route_weight_number> > choice_cache_G_turn;
    std::vector<std::vector<route_weight_number> > choice_cache_S_dist;
    std::vector<std::vector<route_weight_number> > choice_cache_S_dist_3D;
    std::vector<std::vector<route_weight_number> > choice_cache_S;
    std::vector<std::vector<route_weight_number> > choice_cache_S_ang;
    std::vector<std::vector<route_weight_number> > choice_cache_S_turn;
    
    
    
    // template<class Archive>
    // void serialize(Archive & ar, const uint32_t )
    // {// note, version is always the latest when saving
        
    //  ar & dual_points;ar & links;
     
    //  ar & num_segments; ar & num_points; ar & num_links;
     
    //  ar & points2dual & points_2_segs;
     
    //  ar & G_dual_ang & G_dual_turn & S_dual_ang & S_dual_turn & S & S_dist & S_dist_3D & G & G_dist & G_dist_3D;
     
    //  ar & line_segments;
    // }
   
    map_structures(){program = NULL;}
    
    map_structures(base* program_){

        program = program_;

        num_segments=0;num_points=0;num_links=0;
    }

    void clear();

    static void* calculate_lines_static(void*);
    void* calculate_lines(void*);
    void set_cache_arrays(map_structures*,int,int);
    static void* calculate_distances_paths_static(void*);
    void* calculate_distances_paths(void*);
    void dijkstra_reference_list_heap(graph_routes*, const graph&,int,int, int,std::vector<double>&,std::vector<int>&);
    void dijkstra_reference_list_heap_raw(graph_routes*, const graph&,int,int ,int ,std::vector<double>&,std::vector<int>&);
    void populate_raw(std::vector<std::vector<route_weight_number> >*,const graph& ,int, int,const map_structures*, int,ERROR_CODE&,kill_switch&);
    void populate_raw_inner(int,int,std::vector<std::vector<route_weight_number> >*,const graph &,int,const map_structures*,int,int ,int &,kill_switch &,ERROR_CODE &,int);
    void populate_raw2(std::vector<std::vector<route_weight_number> >*,const graph& ,int, int,const map_structures*, int,ERROR_CODE&,kill_switch&);
    void iterate_path_reference(return_iterate*,const std::vector< std::vector<int> > &,const std::vector< int> &,const int &,const int &,const int &, int ,const int &,int ,int);

    //previously desk metrics:

    void power_iteration (std::vector<double>&,graph&,container*,double);
    std::vector<std::vector<route_weight_number> >* desk_choice_new(std::vector<std::vector<route_weight_number> >* choice,map_structures* map, int route_choice_metric, int desk_or_point, int iterations,container* root,ERROR_CODE& error,int do_cache);
    std::vector<std::vector<int> > calculate_route(container* root,const map_structures &map,int choice,int start, int end,int& dual_marker,int iterations, double& value);
    double desk_custom(const std::vector<vec>& points,
                       const std::vector<location>& desks, 
                       map_structures* map, 
                       int desk, 
                       int dest_choice_metric,
                       int route_choice_metric,
                       int metric, 
                       int same_floor, 
                       int visible,
                       int stat,
                       int range_option,
                       std::vector<location>* destination, 
                       ERROR_CODE &error,
                       int is_desk,
                       double range,
                       double range2,
                       int desk_or_point, 
                       int self_count_aware,
                       int dest_is_points,
                       int dest_is_lines,
                       int iterations, 
                       int,std::vector<std::vector<route_weight_number> >&,
                       int route_stat, 
                       int do_cache,
                       std::mutex &map_mutex,
                       kill_switch &killthread,
                       int id);

    double dist_2d(int& ,int& ,const std::vector<vec>& );
    double dist_3d(int& ,int& ,const std::vector<vec>& );
    double seg_dist_2d(int& ,int& ,const map_structures* ,const std::vector<vec>& );
    double seg_dist_3d(int& ,int& ,const map_structures*,const std::vector<vec>& );
    
};

//BOOST_CLASS_TRACKING(map_structures, boost::serialization::track_never)

//BOOST_CLASS_VERSION(map_structures, 1)


#endif /*GRAPH_H*/

