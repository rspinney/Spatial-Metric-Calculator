/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H

#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <vector>
#include <atomic>
#include <thread>
#include <algorithm>

#include "preprocessor.hpp"

#undef min
#undef max

//vector of atomic types (one for each thread) to keep track of progress
//or cancel status - includes get/set/find_min etc. functions

template <typename T>
class thread_vec{


	T reset_val;
	T default_val;

	//need to wrap atomic types before using std containers to avoid invoking
	//copy constuctors and copy assignment operators (at least for MSVC)
	//note: any use of atomwrapper constructors/copy/asisngment is NOT threadsafe
	//anything other than getting or setting values is NOT threadsafe
	
	struct atomwrapper
	{
	  std::atomic<T> a;

	  atomwrapper()
	    :a()
	  {}

	  atomwrapper(const std::atomic<T> &a_)
	    :a(a_.load())
	  {}

	  atomwrapper(const atomwrapper &other)
	    :a(other.a.load())
	  {}

	  atomwrapper &operator=(const atomwrapper &other)
	  {
	    a.store(other.a.load());
	    return *this;
	  }
	};

protected:

	std::vector< atomwrapper > data;
	int registered;
	
public:

	int size() const{
		return static_cast<int>(data.size());
	}


	void reset(){
		registered = size();
		for (auto &x:data){
			x.a = reset_val;
		}
	}

	void reset(int n){ //check there are enough elements
		
		registered = std::min(size(),std::max(1,n));
		for (auto &x:data){
			x.a = reset_val;
		}
	}

	T get() const{
		return get(0);
	}

	T get(int i) const{
		if ((i>-1)&&(i<size())){
			return data[i].a;
		}
		else{
			return default_val;
		}
	}
	thread_vec(T reset_in, T default_in): reset_val(reset_in),default_val(default_in),data(std::max(static_cast<unsigned int>(MIN_THREAD),std::thread::hardware_concurrency())){

		reset();
	};
};


class kill_switch : public thread_vec<int> {
public:
	void kill(){
		for (auto &x:data){
			x.a = 1;
		}
	}

	kill_switch():thread_vec(0,1){};

};


class progress : public thread_vec<double>  {
public:
	double get_min() const{
		double min_v = 200.0;
		for (int i=0;i<registered;i++){
			if (data[i].a<min_v){
				min_v = data[i].a;
			}
		}
		return std::min(min_v,100.0);
	}

	void set(int i,double in){
		if ((i>-1)&&(i<static_cast<int>(data.size()))){
			data[i].a = in;
		}
	}

	progress():thread_vec(0.0,0.0){};
};

#endif //THREAD_UTILS_H

