/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/minheap.hpp"


bool operator< (const weight_index& w1, const weight_index& w2)
{
    return w1.weight < w2.weight;
}

bool operator> (const weight_index& w1, const weight_index& w2)
{
    return w1.weight > w2.weight;
}

min_heap::min_heap(const std::vector<weight_index>& data_in) : data(data_in)
{
    
    index_back.reserve(data_in.size());
    
    for (unsigned int i=0;i<data_in.size();i++){
        index_back.push_back(i);
    }
    
    make_heap();
}

void min_heap::make_heap()
{
    int length = data.size();
    for(int i=length-1; i>=0; --i)
    {
        send_down(i);
    }
}

void min_heap::send_down(int index)
{
    int length = data.size();
    int left_child_index = 2*index + 1;
    int right_child_index = 2*index + 2;
    
    if(left_child_index >= length)
        return; //index is a leaf
    
    int min_index = index;
    
    
    if (data[index].weight > data[left_child_index].weight)
    {
        min_index = left_child_index;
    }
    
    if ((right_child_index < length) && (data[min_index].weight > data[right_child_index].weight))
    {
        min_index = right_child_index;
    }
    
    
    if(min_index != index)
    {
        //need to swap
        weight_index temp = data[index];
        data[index] = data[min_index];
        data[min_index] = temp;
        
        //update indexing
        index_back[data[min_index].index]=min_index;
        index_back[data[index].index]=index;
        
        send_down(min_index);
    }
}

void min_heap::send_up(int index)
{
    if(index == 0)
        return;
    
    int parent_index = (index-1)/2;
    
    if(data[parent_index].weight > data[index].weight)
    {
        weight_index temp = data[parent_index];
        data[parent_index] = data[index];
        data[index] = temp;
        
        index_back[data[parent_index].index]=parent_index;
        index_back[data[index].index]=index;
        
        send_up(parent_index);


    }
}

void min_heap::insert(weight_index new_val)
{
    
    int length = data.size();
    data.push_back(new_val);
    send_up(length);
}

int min_heap::get_min()
{
    return data[0].index;
}

void min_heap::remove_min()
{
    int length = data.size();
    
    if(length == 0)
    {
        return;
    }
    
    data[0] = data[length-1];
    data.pop_back();
    send_down(0);
}