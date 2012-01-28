cmake_minimum_required(VERSION 2.6)
 
Project(ttt)
 
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
# we can now parse the git-commit-id:
STRING(REGEX REPLACE "([0-9][0-9]).*" "\\1" GIT_MAJOR_VERSION "${GIT_COMMIT_ID}" )
STRING(REGEX REPLACE "[0-9][0-9].([0-9][0-9])-.*" "\\1" GIT_MINOR_VERSION "${GIT_COMMIT_ID}" )
STRING(REGEX REPLACE "[0-9][0-9].[0-9][0-9]-(.*)-.*" "\\1" GIT_PATCH_VERSION "${GIT_COMMIT_ID}" )
SET(MY_VERSION "${GIT_MAJOR_VERSION}.${GIT_MINOR_VERSION}.${GIT_PATCH_VERSION}" CACHE STRING "name")

set(TTT_SRC_FILES
    ttt.cpp
)

set(TTT_INCLUDE_FILES
    ttt.hpp
    p.hpp
    ngc_writer.hpp
    extents.hpp
    dxf_writer.hpp
    writer.hpp
)

########################################################################
# this makes the Python module
add_library(
    ttt
    MODULE
    ${TTT_SRC_FILES}
    ttt_py.cpp # the boost-python bindings are here.
)

set_target_properties(ttt PROPERTIES PREFIX "") 
set_target_properties(ttt PROPERTIES VERSION ${MY_VERSION}) 
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

########################################################################
# the c++ shared library
add_library(
    libttt
    SHARED
    ${TTT_SRC_FILES}
)
set_target_properties(libttt PROPERTIES PREFIX "") 
set_target_properties(libttt PROPERTIES VERSION ${MY_VERSION}) 
target_link_libraries(libttt ${FREETYPE_LIBRARY}) # no need to link with boost-python here.

install(
    TARGETS libttt
    LIBRARY 
    DESTINATION lib/ttt
    ARCHIVE DESTINATION lib/ttt
    PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

# this installs the c++ include headers
install(
    FILES ${TTT_INCLUDE_FILES}
    DESTINATION include/ttt
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
)

########################################################################
# install readme and example(s)
# 'other' documentation files
set(DOC_FILES ${CMAKE_SOURCE_DIR}/../README.asciidoc ${CMAKE_SOURCE_DIR}/../LICENSE.txt)
set(DOC_PATH "share/doc/ttt")
install(FILES ${DOC_FILES}
        DESTINATION ${DOC_PATH}
)

########################################################################
# "make package" stuff
 
execute_process(
    COMMAND dpkg --print-architecture
    OUTPUT_VARIABLE DEB_ARCHITECTURE
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

include(package_details.cmake)

# "make spackage" will build a source package
add_custom_target(spackage 
              ${CMAKE_COMMAND} 
              -D SRC_DIR:STRING=${CMAKE_SOURCE_DIR} 
              -D MY_VERSION:STRING=${MY_VERSION} 
              -C ${CMAKE_SOURCE_DIR}/package_details.cmake
              -P ${CMAKE_CURRENT_SOURCE_DIR}/deb/DebSourcePPA.cmake 
            ) 
            
# "make package" builds a binary debian package
include(CPack) # this should be last
