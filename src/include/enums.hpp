/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#ifndef ENUMS_H
#define ENUMS_H

#include "config.h"

#if defined(WIN32)
    #include "stdafx.h"
#endif



enum THREAD_STATE{
    
    FREE = 0,
    SAVING = 1,
    LOADING = 2,
    LOADING_POINTS = 3,
    LOADING_MATRIX = 4,
    LOADING_DESKS = 5,
    LOADING_DXF = 6,
    MAP_CHECK = 7,
    CALC_ROUTE = 8,
    CALC_METRIC = 9,
    CALC_STRUCTURES = 10,
    CALC_VIS = 11,
    CALC_LINES = 12,
    INITIALISING = 13,
    LOADING_ADJ = 14,
    CHECKING_MAP = 15,
    PREP_UNDO = 16,
    FLOORPLAN_UTILITY = 17,
    FLOORPLAN_CONVERT = 18
};


#endif /*ENUMS_H*/
