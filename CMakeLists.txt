
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

if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

add_executable ( totalpreprocess
    angle.cpp
    data.cpp
    main.cpp
    polygonpreprocess.cpp
    process.cpp
    skeleton.cpp
    cnpy/cnpy.cpp )

target_link_libraries ( totalpreprocess
    ${CGAL_LIBRARIES}
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${OpenCV_LIBS}
    ${ZLIB_LIBRARIES} )

cotire ( totalpreprocess )
