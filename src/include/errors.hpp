/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef ERRORS_H
#define ERRORS_H

#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif

#include <string>
#include <vector>

#include <boost/serialization/utility.hpp>
#include <boost/serialization/access.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/version.hpp>

class ERROR_CODE{
	friend class boost::serialization::access;

public:

    int MAX_PATHS_REACHED; // WARNING
    int NO_DESTINATIONS; //none even available before selection criteria - ERROR
    int VIS_DEST_MISMATCH; // ERROR
    int NO_VALID_DESTINATIONS; // none available after selction crtiera - WARNING
    int INPUT_ERROR; // ERROR - BUG
    int INFINITE_VALUE; // WARNING - infinite distance
    int MAX_PATHS_REACHED_NONE_FOUND; // ERROR - malformed graph/measure
    int LOOPS_DETECTED; // WARNNING 
    int NO_GRAPH;
    int INFINITE_PATH;

	ERROR_CODE():MAX_PATHS_REACHED(0),
				 NO_DESTINATIONS(0),
				 VIS_DEST_MISMATCH(0),
				 NO_VALID_DESTINATIONS(0),
				 INPUT_ERROR(0),
				 INFINITE_VALUE(0),
				 MAX_PATHS_REACHED_NONE_FOUND(0),
				 LOOPS_DETECTED(0),
				 NO_GRAPH(0),
				 INFINITE_PATH(0){}

	void clear(){
		MAX_PATHS_REACHED=0;
		NO_DESTINATIONS=0;
		VIS_DEST_MISMATCH=0;
		NO_VALID_DESTINATIONS=0;
		INPUT_ERROR=0;
		INFINITE_VALUE=0;
		MAX_PATHS_REACHED_NONE_FOUND=0;
		LOOPS_DETECTED=0;
		NO_GRAPH=0;
		INFINITE_PATH=0;
	}

	void collect(ERROR_CODE &in){
		if (in.MAX_PATHS_REACHED) this->MAX_PATHS_REACHED=1;
		if (in.NO_DESTINATIONS) this->NO_DESTINATIONS=1;
		if (in.VIS_DEST_MISMATCH) this->VIS_DEST_MISMATCH=1;
		if (in.NO_VALID_DESTINATIONS) this->NO_VALID_DESTINATIONS=1;
		if (in.INPUT_ERROR) this->INPUT_ERROR=1;
		if (in.INFINITE_VALUE) this->INFINITE_VALUE=1;
		if (in.MAX_PATHS_REACHED_NONE_FOUND) this->MAX_PATHS_REACHED_NONE_FOUND=1;
		if (in.LOOPS_DETECTED) this->LOOPS_DETECTED=1;
		if (in.NO_GRAPH) this->NO_GRAPH=1;
		if (in.INFINITE_PATH) this->INFINITE_PATH=1;
	}

	int is_error(){
		int ret = 0;
		if (MAX_PATHS_REACHED) ret=1;
		if (NO_DESTINATIONS) ret=1;
		if (VIS_DEST_MISMATCH) ret=1;
		if (NO_VALID_DESTINATIONS) ret=1;
		if (INPUT_ERROR) ret=1;
		if (INFINITE_VALUE) ret=1;
		if (MAX_PATHS_REACHED_NONE_FOUND) ret=1;
		if (LOOPS_DETECTED) ret=1;
		if (NO_GRAPH) ret=1;
		if (INFINITE_PATH) ret=1;
		return ret;
	}	

	std::vector<std::string> get_error_messages(){
		std::vector<std::string> ret;

		if (MAX_PATHS_REACHED){
			ret.push_back("WARNING: Not all paths found for at least one basis element. The max. number of routes may be too small.\n");
		}
		if (NO_DESTINATIONS){
			ret.push_back("ERROR: No destinations available for use in calculation.\n");
		}
		if (VIS_DEST_MISMATCH){
			ret.push_back("ERROR: Visibility information does not match destinations for at least one basis element.\n");
		}
		if (NO_VALID_DESTINATIONS){
			ret.push_back("WARNING: No qualifying destinations for use in metric for at least one basis element.\n");
		}
		if (INPUT_ERROR){
			ret.push_back("ERROR: Unrecognised input - report bug.\n");
		}	
		if (MAX_PATHS_REACHED_NONE_FOUND){
			ret.push_back("ERROR: Max. routes limit reached, but all contain loops.\n");
		}
		if (LOOPS_DETECTED){
			ret.push_back("WARNING: Loops detected in shortest paths.\n");
		}
		if (NO_GRAPH){
			ret.push_back("ERROR: No graph constructs.\n");
		}
		if (INFINITE_PATH){
			ret.push_back("WARNING: At least one shortest path between a basis element and a qualifying destination is infinite.\n");
		}
		if (INFINITE_VALUE){
			ret.push_back("WARNING: Infinite metric recorded for at least one basis element.\n");
		}

		return ret;
	}

	template<class Archive>
    void serialize(Archive & ar, const uint32_t /*version*/)
    {// note, version is always the latest when saving
        ar & MAX_PATHS_REACHED;
		ar & NO_DESTINATIONS;
		ar & VIS_DEST_MISMATCH;
		ar & NO_VALID_DESTINATIONS;
		ar & INPUT_ERROR;
		ar & INFINITE_VALUE;
		ar & MAX_PATHS_REACHED_NONE_FOUND;
		ar & LOOPS_DETECTED;
		ar & NO_GRAPH;
		ar & INFINITE_PATH;
    }
    
};
BOOST_CLASS_TRACKING(ERROR_CODE, boost::serialization::track_never)

#endif /*ERRORS_H*/
