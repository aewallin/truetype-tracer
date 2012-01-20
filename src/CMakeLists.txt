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


################ create version_string.hpp, http://stackoverflow.com/questions/3780667
# include the output directory, where the version_string.hpp file is generated
include_directories(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE(version_string.cmake)


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

#
# this figures out where to install the Python modules
#
execute_process(
    COMMAND python -c "from distutils.sysconfig import get_python_lib; print get_python_lib(0,0,\"/usr/local\")"
    OUTPUT_VARIABLE Python_site_packages
    OUTPUT_STRIP_TRAILING_WHITESPACE
) # on Ubuntu 11.10 this outputs: /usr/local/lib/python2.7/dist-packages


# strip away /usr/local/  because that is what CMAKE_INSTALL_PREFIX is set to
# also, since there is no leading "/", it makes ${Python_site_packages} a relative path.
STRING(REGEX REPLACE "/usr/local/(.*)$" "\\1" Python_site_packages "${Python_site_packages}" )

MESSAGE(STATUS "CMAKE_INSTALL_PREFIX is : " ${CMAKE_INSTALL_PREFIX})
MESSAGE(STATUS "Python libraries will be installed to: " ${Python_site_packages})

# this installs the python library
install(
    TARGETS ttt
    LIBRARY DESTINATION ${Python_site_packages}
)

# "make package" stuff
 
execute_process(
    COMMAND dpkg --print-architecture
    OUTPUT_VARIABLE DEB_ARCHITECTURE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)


#message( STATUS " git commit id: " ${GIT_COMMIT_ID})
# parse the commit id to get major, minor, patch
STRING(REGEX REPLACE "([0-9][0-9]).*" "\\1" GIT_MAJOR_VERSION "${GIT_COMMIT_ID}" )
#MESSAGE(STATUS "result= ${GIT_MAJOR_VERSION}")
STRING(REGEX REPLACE "[0-9][0-9].([0-9][0-9])-.*" "\\1" GIT_MINOR_VERSION "${GIT_COMMIT_ID}" )
#MESSAGE(STATUS "result= ${GIT_MINOR_VERSION}")
STRING(REGEX REPLACE "[0-9][0-9].[0-9][0-9]-(.*)-.*" "\\1" GIT_PATCH_VERSION "${GIT_COMMIT_ID}" )
#MESSAGE(STATUS "result= ${GIT_PATCH_VERSION}")

set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_CONTACT "Anders Wallin <anders.e.e.wallin@gmail.com>" CACHE STRING "email")
set(CPACK_PACKAGE_NAME "truetyeptracer" CACHE STRING "name")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ port with python bindings of true-type-tracer" CACHE STRING "name")
set(CPACK_PACKAGE_VENDOR https://github.com/aewallin/truetype-tracer)
set(CPACK_DEBIAN_PACKAGE_SECTION "science" CACHE STRING "name")
set(CPACK_DEBIAN_BUILD_DEPENDS debhelper python libboost-dev libboost-python-dev libfreetype libfreetype-dev git  cmake  CACHE STRINGS "name")
set(CPACK_DEBIAN_PACKAGE_DEPENDS python git libboost-python libfreetype cmake  CACHE STRING "name")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${DEB_ARCHITECTURE} CACHE STRING "name")
set(CPACK_DEBIAN_PACKAGE_PRIORITY optional CACHE STRING "name")
SET(CPACK_PACKAGE_VERSION "${GIT_MAJOR_VERSION}.${GIT_MINOR_VERSION}.${GIT_PATCH_VERSION}" CACHE STRING "name")

set_target_properties(ttt PROPERTIES VERSION ${CPACK_PACKAGE_VERSION}) 

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}" CACHE STRING "name")
set(CPACK_DEBIAN_DISTRIBUTION_NAME ubuntu CACHE STRING "name")
set(CPACK_DEBIAN_DISTRIBUTION_RELEASES oneiric CACHE STRING "name") 
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/debian/debian_package_description.txt CACHE STRING "${CPACK_PACKAGE_DESCRIPTION_FILE}")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/debian/debian_copyright.txt CACHE STRING "name")
set(CPACK_DEBIAN_CHANGELOG "  * Latest development version." CACHE STRING "name")
set(DEB_SRC_DIR ${CMAKE_SOURCE_DIR} CACHE STRING "name" )

# run the source package generation as a target, so we don't have to do it every time
add_custom_target(spackage 
              ${CMAKE_COMMAND} -C ${CMAKE_BINARY_DIR}/CMakeCache.txt
              -P ${CMAKE_CURRENT_SOURCE_DIR}/debian/DebSourcePPA.cmake 
              # WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            ) # get the changes file as an output from here?

include(CPack) # this should be last
