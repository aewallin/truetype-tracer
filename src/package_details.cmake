
message(STATUS " called with SRC_DIR is = " ${SRC_DIR})
message(STATUS " called with version  = " ${MY_VERSION})

set(CPACK_GENERATOR "DEB" CACHE string "generator" )
set(CPACK_PACKAGE_CONTACT "Anders Wallin <anders.e.e.wallin@gmail.com>" CACHE STRING "email")
set(CPACK_PACKAGE_NAME "truetyeptracer" CACHE STRING "name")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "C++ port with python bindings of true-type-tracer" CACHE STRING "name2")
set(CPACK_PACKAGE_VENDOR https://github.com/aewallin/truetype-tracer CACHE STRING "web")
set(CPACK_DEBIAN_PACKAGE_SECTION "science" CACHE STRING "name3")
set(CPACK_DEBIAN_BUILD_DEPENDS debhelper python libboost-dev libboost-python-dev libfreetype6 libfreetype6-dev git  cmake  CACHE STRING "name4")

# we need to explicitly list the libboost-python versions here. why??
# precise has 1.48.0
# oneiric has 1.46.1
# natty/maverick has 1.42.0
# lucid has 1.40.0
set(DEBSRC_PACKAGE_DEPENDS python git cmake 
                "libboost-python1.48.0 | libboost-python1.46.1 | libboost-python1.42.0 | libboost-python1.40.0"
                libfreetype6 CACHE STRING "name")

set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${DEB_ARCHITECTURE} CACHE STRING "name6")
set(CPACK_DEBIAN_PACKAGE_PRIORITY optional CACHE STRING "name7")
SET(CPACK_PACKAGE_VERSION ${MY_VERSION} CACHE STRING "name8")
set(CPACK_DEBIAN_DISTRIBUTION_NAME ubuntu CACHE STRING "name9")
set(CPACK_DEBIAN_DISTRIBUTION_RELEASES lucid maverick natty oneiric CACHE STRING "name10") 
message(STATUS " CMAKE_SOURCE_DIR is = " ${CMAKE_SOURCE_DIR})
if(${SRC_DIR} MATCHES "")
    set(CPACK_PACKAGE_DESCRIPTION_FILE ${SRC_DIR}/deb/debian_package_description.txt CACHE STRING "package description file")
    set(CPACK_RESOURCE_FILE_LICENSE ${SRC_DIR}/deb/debian_copyright.txt CACHE STRING "name11")
    set(DEB_SRC_DIR ${SRC_DIR} CACHE STRING "name13" )
else(${SRC_DIR} MATCHES "")
    set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_SOURCE_DIR}/deb/debian_package_description.txt CACHE STRING "package description file")
    set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/deb/debian_copyright.txt CACHE STRING "name11")
    set(DEB_SRC_DIR ${CMAKE_SOURCE_DIR} CACHE STRING "name13" )
endif(${SRC_DIR} MATCHES "")

message(STATUS " descr file = ${CPACK_PACKAGE_DESCRIPTION_FILE}")

set(CPACK_DEBIAN_CHANGELOG "  * Latest development version." CACHE STRING "name12")
