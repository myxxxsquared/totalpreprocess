
cmake_minimum_required ( VERSION 3.5 )

list ( INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/CMake")

project( totalpreprocess )
set ( CMAKE_CXX_STANDARD 11 )

find_package ( OpenCV REQUIRED )
find_package ( CGAL REQUIRED COMPONENTS Core )
find_package ( ZLIB REQUIRED )

include ( ${CGAL_USE_FILE} )
include ( cotire )

include_directories ( BEFORE "../../include" )
include_directories ( BEFORE "include" )
include_directories ( "cnpy" ${ZLIB_INCLUDE_DIRS} )

add_executable ( totalpreprocess
    skeleton.cpp
    data.cpp
    main.cpp
    process.cpp
    cnpy/cnpy.cpp )

target_link_libraries ( totalpreprocess
    ${CGAL_LIBRARIES}
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${OpenCV_LIBS}
    ${ZLIB_LIBRARIES} )

cotire ( totalpreprocess )
