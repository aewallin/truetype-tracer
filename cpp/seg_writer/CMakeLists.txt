project(TTT_TST)

cmake_minimum_required(VERSION 2.4)

if (CMAKE_BUILD_TOOL MATCHES "make")
    add_definitions(-Wall -Werror -Wno-deprecated -pedantic-errors)
endif (CMAKE_BUILD_TOOL MATCHES "make")

# find BOOST 
find_package( Boost )
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    MESSAGE(STATUS "found Boost: " ${Boost_LIB_VERSION})
    MESSAGE(STATUS "boost-incude dirs are: " ${Boost_INCLUDE_DIRS})
endif()


find_library(TTT_LIBRARY REQUIRED
            NAMES ttt libttt
            PATHS /usr/local/lib/ttt /usr/local/lib
            DOC "ttt"
)
MESSAGE(STATUS "TTT_LIBRARY is: " ${TTT_LIBRARY})
include_directories( /usr/local/include/ttt )

find_package(Freetype REQUIRED)
if( FREETYPE_FOUND )
   MESSAGE(STATUS "FREETYPE_LIBRARY is: " ${FREETYPE_LIBRARY})
   MESSAGE(STATUS "FREETYPE_INCLUDE_DIRS is: " ${FREETYPE_INCLUDE_DIRS})
endif()
include_directories(${FREETYPE_INCLUDE_DIRS})


set(TTT_TST_SRC
    main.cpp
)

add_executable(
    ttt_tst
    ${TTT_TST_SRC}
)
target_link_libraries(ttt_tst ${TTT_LIBRARY} )


