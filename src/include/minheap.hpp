/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>


class weight_index{

public:
    double weight; 
    int index;
    
    friend bool operator< (const weight_index&, const weight_index&);
    friend bool operator> (const weight_index&, const weight_index&);
        
};


class min_heap
{
private:
   
    void make_heap();
    
public:
    
    std::vector<weight_index> data;
    std::vector<int> index_back;
    
  
    min_heap(const std::vector<weight_index>&);
    min_heap(){};
    
    void insert(weight_index);
    int get_min();
    void remove_min();
    
    void send_down(int index);
    void send_up(int index);
    
};


#endif /*MINHEAP_H*/

/*
MinHeap min_heap
BubbleUp send_up
BubbleDown send_down
Heapify make_hep
GetMin get_min
DeleteMin remove_min
Insert insert
_vector data

*/
