cmake_minimum_required(VERSION 2.6)
 
Project(ttt5)
 
# this figures out the Python include directories and adds them to the
# header file search path
execute_process(
    COMMAND python-config --includes
    COMMAND sed -r "s/-I//g; s/ +/;/g"
    COMMAND tr -d '\n'
    OUTPUT_VARIABLE Python_Includes
)
include_directories(${Python_Includes})

find_package( Boost COMPONENTS python REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    MESSAGE(STATUS "Boost_LIB_VERSION: " ${Boost_LIB_VERSION})
    MESSAGE(STATUS "Boost_INCLUDE_DIRS are: " ${Boost_INCLUDE_DIRS})
    MESSAGE(STATUS "Boost_PYTHON_LIBRARY is: " ${Boost_PYTHON_LIBRARY})
    MESSAGE(STATUS "boost_LIBRARY_DIRS is: " ${Boost_LIBRARY_DIRS})
    MESSAGE(STATUS "Boost_LIBRARIES is: " ${Boost_LIBRARIES})    
endif()

find_package(Freetype REQUIRED)
if( FREETYPE_FOUND )
   MESSAGE(STATUS "FREETYPE_LIBRARY is: " ${FREETYPE_LIBRARY})
   MESSAGE(STATUS "FREETYPE_INCLUDE_DIRS is: " ${FREETYPE_INCLUDE_DIRS})
endif()
include_directories(${FREETYPE_INCLUDE_DIRS})


#### create version_string.hpp, http://stackoverflow.com/questions/3780667
# include the output directory, where the version_string.hpp file is generated
include_directories(${CMAKE_CURRENT_BINARY_DIR})
# a custom target that is always built
add_custom_target(
    version_string 
    ALL
    DEPENDS ver_string 
)
# creates version_string.hpp using cmake script
add_custom_command(
    OUTPUT ver_string ${CMAKE_CURRENT_BINARY_DIR}/version_string.hpp
    COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                                            -P ${CMAKE_CURRENT_SOURCE_DIR}/version_string.cmake
)
# version_string.hpp is a generated file
set_source_files_properties(
    ${CMAKE_CURRENT_BINARY_DIR}/version_string.hpp
    PROPERTIES GENERATED TRUE
    HEADER_FILE_ONLY TRUE
)
#### end create version_string.hpp


# this makes the Python module
add_library(
    ttt
    MODULE
    ttt.cpp
    ttt_py.cpp
)
add_dependencies( 
    ttt
    version_string 
)
set_target_properties(ttt PROPERTIES PREFIX "") 
target_link_libraries(ttt ${Boost_LIBRARIES} ${FREETYPE_LIBRARY}) 

# this figures out where to install the Python modules
execute_process(
    COMMAND python -c "from distutils.sysconfig import get_python_lib; print get_python_lib()"
    OUTPUT_VARIABLE Python_site_packages
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
MESSAGE(STATUS "Python libraries will be installed to: " ${Python_site_packages})

# this installs the python library
install(
    TARGETS ttt
    LIBRARY DESTINATION ${Python_site_packages}
)
