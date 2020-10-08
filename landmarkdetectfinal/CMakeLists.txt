cmake_minimum_required( VERSION 2.8 )

project( opencvprj )

set( CMAKE_CXX_FLAGS "-std=c++11" )

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

#add_executable( main main.cpp detection.cpp estimate.cpp)
add_library( detect_final SHARED final.cpp detection.cpp estimate.cpp )
#target_link_libraries( main ${OpenCV_LIBS} )
target_link_libraries( detect_final ${OpenCV_LIBS} )
