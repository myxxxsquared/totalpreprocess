
cmake_minimum_required ( VERSION 3.5 )

project( totalpreprocess )
set ( CMAKE_CXX_STANDARD 11 )

find_package ( OpenCV REQUIRED )
find_package ( CGAL REQUIRED COMPONENTS Core )

include ( ${CGAL_USE_FILE} )
include_directories ( BEFORE "../../include" )
include_directories ( BEFORE "include" )

add_executable ( totalpreprocess skeleton.cpp data.cpp main.cpp )
target_link_libraries ( totalpreprocess ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} ${OpenCV_LIBS} )
