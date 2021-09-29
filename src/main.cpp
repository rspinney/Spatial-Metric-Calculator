/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

#include "./include/config.h"

#if defined(WIN32)
    #include "./include/stdafx.h"
#endif

#include "./include/base.hpp"


int main() { 

	//if in need of a debug console for std::cout on windows...

	/*
	AllocConsole();
	freopen("CONOUT$","w",stdout);
	*/

    base program;  
    program.build_and_run();  
    return 0;

}


