cmake_minimum_required(VERSION 3.10)

set(EXEC_NAME Spatial-Metric-Calculator)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# set the project name
project(${EXEC_NAME}  VERSION 1.4 LANGUAGES CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#compiler flags
if (MSVC)
	SET(CMAKE_CXX_FLAGS_RELEASE "/O2")
	SET(CMAKE_C_FLAGS_RELEASE "/O2")
elseif(APPLE)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
	set(CMAKE_CXX_FLAGS_DEBUG "-g3 -ggdb")
	set(CMAKE_CXX_FLAGS_RELEASE "-flto=full -O3 -Wno-deprecated -Wno-missing-field-initializers")
	SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
else()
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
	set(CMAKE_CXX_FLAGS_DEBUG "-g3 -ggdb")
	set(CMAKE_CXX_FLAGS_RELEASE "-flto -O3 -fexpensive-optimizations -Wno-deprecated -Wno-missing-field-initializers")
	SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")	
endif()
    
if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE icon.icns)
    set(smc_ICON ./resources/icon.icns)
    set_source_files_properties(${smc_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()
 
## executable + sources    
# sources
set(SRCS ./src/base.cpp ./src/edit_funcs.cpp ./src/fltk_extend.cpp ./src/locations.cpp ./src/metric_interface.cpp  ./src/text_import.cpp ./src/data.cpp  ./src/edit_window_funcs.cpp ./src/graph.cpp ./src/location_visibility.cpp ./src/metrics.cpp ./src/r_text.cpp ./src/ui.cpp ./src/dw_base.cpp ./src/dw_annotations.cpp ./src/dw_areas.cpp ./src/dw_graph_loc_metrics.cpp ./src/dw_grid_and_plans.cpp ./src/dw_helper_and_misc.cpp ./src/dw_input.cpp ./src/dw_shapes.cpp ./src/dw_snapon_select.cpp ./src/dw_view_transform_lighting.cpp ./src/dw_walls_and_textures.cpp ./src/floorplan_barriers.cpp ./src/main.cpp ./src/minheap.cpp ./src/undo_blocked_input.cpp ./src/dxf_parser.cpp  ./src/floors.cpp ./src/input_output.cpp ./src/mesh.cpp ./src/plot.cpp ./src/tabs_fltk_1-4-0.cpp ./src/undo_redo.cpp)

# add the executable
add_executable(${EXEC_NAME} WIN32 MACOSX_BUNDLE ${SRCS})

target_include_directories(${EXEC_NAME}  PRIVATE ./src/include)

#windows options
if (WIN32)
    target_sources(${EXEC_NAME} PRIVATE ./resources/smc.rc ./src/stdafx.cpp ./src/include/stdafx.h)
elseif (APPLE)
    target_sources(${EXEC_NAME} PRIVATE ${smc_ICON})
    set_target_properties(${EXEC_NAME} PROPERTIES MACOSX_BUNDLE_COPYRIGHT "Copyright (C), 2014-2021, Richard Spinney." MAXOSX_BUNDLE_VERSION "1.4.0" MACOSX_BUNDLE_LONG_VERSION_STRING "1.4.0" MACOSX_BUNDLE_SHORT_VERSION_STRING "1.4")
endif()

## Linking options   
#Mac options
if (APPLE)
  target_link_libraries(${EXEC_NAME}  "-framework cocoa")
endif (APPLE)

if (MSVC)
	include(CheckIPOSupported)
	check_ipo_supported(RESULT supported OUTPUT error)
	if( supported )
	    message(STATUS "IPO / LTO enabled")
	    set_property(TARGET ${EXEC_NAME}  PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
	else()
	    message(STATUS "IPO / LTO not supported: <${error}>")
	endif()
elseif (APPLE)
	set_target_properties(${EXEC_NAME} PROPERTIES LINK_FLAGS_RELEASE -flto=full)
else()
	set_target_properties(${EXEC_NAME} PROPERTIES LINK_FLAGS_RELEASE -flto)
endif()

# Linked dependencies
#openGL
set(OpenGL_GL_PREFERENCE LEGACY)
FIND_PACKAGE(OpenGL REQUIRED)

#fltk - must be compiled with cmake
find_package(FLTK REQUIRED NO_MODULE)
if (${FLTK_VERSION} VERSION_GREATER_EQUAL 1.3.7)
    find_library(FLTK_LIB_LOC1 fltk)
    find_library(FLTK_LIB_LOC2 fltk_images)
    find_library(FLTK_LIB_LOC3 fltk_gl)
    message(STATUS "Found FLTK version " ${FLTK_VERSION} ": " ${FLTK_LIB_LOC1} ", " ${FLTK_LIB_LOC2} ", " ${FLTK_LIB_LOC3} )
else()
    message(FATAL_ERROR "Found FLTK version " ${FLTK_VERSION} " but version 1.3.7 required.")
endif()
target_include_directories(${EXEC_NAME} PUBLIC ${FLTK_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME} fltk fltk_forms fltk_images fltk_gl)

#boost
set(BOOST_ALL_NO_LIB ON)
set(Boost_USE_STATIC_LIBS ON)
add_definitions(-D_Boost_USE_STATIC_LIBS=ON)
find_package(Boost 1.77 REQUIRED COMPONENTS date_time serialization)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${EXEC_NAME} Boost::serialization)

