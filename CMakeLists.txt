
cmake_minimum_required ( VERSION 3.5 )

project( totalpreprocess )

list ( INSERT CMAKE_MODULE_PATH 0 "${CMAKE_SOURCE_DIR}/CMake")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    SET(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_CXX_FLAGS "-O0 -ggdb -DDEBUG")
else()
    SET(CMAKE_EXE_LINKER_FLAGS "-flto")
    set(CMAKE_CXX_FLAGS "-Wall -flto -O3 -DNDEBUG")
    set ( CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE )
endif()

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
    angle.cpp
    data.cpp
    main.cpp
    resize.cpp
    main_generator.cpp
    main_viewer.cpp
    main_resize.cpp
    main_union.cpp
    main_iou.cpp
    polygonpreprocess.cpp
    process.cpp
    skeleton.cpp
    union.cpp
    cnpy/cnpy.cpp )

target_link_libraries ( totalpreprocess
    ${CGAL_LIBRARIES}
    ${CGAL_3RD_PARTY_LIBRARIES}
    ${OpenCV_LIBS}
    ${ZLIB_LIBRARIES} )

# if(MSVC)
#     if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
#         string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
#     else()
#         set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
#     endif()
# elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
#     message("GNU")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -flto")
#     SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")
# endif()


cotire ( totalpreprocess )
