cmake_minimum_required (VERSION 3.5.2)
project (mila_accept)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_BINARY_DIR}/pkg)

######################################################
## install env
IF($ENV{DIST_PKG})
    set(ETC_DIR "/etc/mila_accept")
ELSE($ENV{DIST_PKG})
    set(ETC_DIR "./etc")
ENDIF($ENV{DIST_PKG})
add_definitions(-DETC_DIR=${ETC_DIR})

######################################################
## Set CPack section
SET(CPACK_GENERATOR "DEB")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Vitold Kapshitzer") #required
set(CPACK_PACKAGE_VENDOR "easy-tech.ch")
set(CPACK_PACKAGE_VERSION "0.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")


SET(CPACK_DEBIAN_PACKAGE_DEPENDS "curl (>= 7.56.0), libglib2.0-0 (>= 2.48.1-1), openssl (>= 1.0.2g-1), libjson-glib-1.0-0 (>= 1.1.2-0) ")

INCLUDE(CPack)
############################################################


#############################################################################
## Project def

SET(CMAKE_THREAD_PREFER_PTHREAD ptread)

INCLUDE_DIRECTORIES(/opt/local/include )
INCLUDE_DIRECTORIES("/opt/local/include/mysql57/mysql/")
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/include")
INCLUDE_DIRECTORIES("/usr/include/mysql")


string(TIMESTAMP compile_date "\"%d-%m-%Y %H:%M:%S\"")
add_definitions(-DCOMPILE_DATE=${compile_date})
add_definitions(-DBUILD_NUMBER=0)

#foreach(var ${notserv})
#get_target_property(tmp ${var} COMPILE_FLAGS)
#message("${tmp}")
#endforeach(var)

IF(${CMAKE_BUILD_TYPE} STREQUAL Release)
	add_definitions(-DCOMPILE_FLAGS=\"${CMAKE_C_FLAGS_DEBUG}\")
elseif(${CMAKE_BUILD_TYPE} STREQUAL Release)
	add_definitions(-DCOMPILE_FLAGS=\"${CMAKE_C_FLAGS_RELEASE}\")

#foreach(var ${sources})
#	message("s: ${var}")

#	get_source_file_property(cvtt ${var} SYMBOLIC)
#	message("c: ${cvtt}")
	#	add_definitions(-DCOMPILE_FLAGS=\"${cv}\")
#endforeach(var)
ENDIF()

IF(NOT(APPLE))
SET(GCC_COVERAGE_LINK_FLAGS    "-Wl,-Map,output.map")
ENDIF(NOT(APPLE))
SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}" )

link_directories( /usr/local/lib /lib64 /opt/local/lib/mysql57/mysql /opt/local/lib/ /opt/local/include)

file( GLOB_RECURSE
    sources
    src/*
    )

add_executable(${CMAKE_PROJECT_NAME} ${sources})

######################################################
## CPack install
INSTALL(TARGETS ${CMAKE_PROJECT_NAME}
    RUNTIME
    DESTINATION bin)
#INSTALL(FILES etc/mila_accept.conf
#    DESTINATION etc/mila_accept)
######################################################



############################################################
## Library
FIND_PACKAGE(Threads)
IF(Threads_FOUND)
    INCLUDE_DIRECTORIES(${THREADS_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${THREADS_LIBRARIES})
ENDIF(Threads_FOUND)

message("${CMAKE_THREAD_LIBS_INIT}")

FIND_PACKAGE(GTK)
IF(GTK_FOUND)
    INCLUDE_DIRECTORIES(${GTK_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${GTK_LIBRARIES})
ENDIF(GTK_FOUND)

FIND_PACKAGE(GLIB)
IF(GLIB_FOUND)
    INCLUDE_DIRECTORIES(${GLIB_INCLUDE_DIRS})
    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${GLIB_LIBRARIES})
ENDIF(GLIB_FOUND)

#FIND_PACKAGE(OpenSSL)
#IF(OPENSSL_FOUND)
#    INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIRS})
#    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${OPENSSL_LIBRARIES})
#ENDIF(OPENSSL_FOUND)

#FIND_PACKAGE(json-GLIB)
#IF(json-GLIB_FOUND)
#    INCLUDE_DIRECTORIES(${json-GLIB_INCLUDE_DIRS})
#    TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${json-GLIB_LIBRARIES})
#ENDIF(json-GLIB_FOUND)


IF(APPLE)
    MESSAGE("OSX")
    FIND_LIBRARY(FOUNDATION_LIBRARY Foundation )
    FIND_LIBRARY(SECURITY_LIBRARY Security )
    SET(EXTRA_LIBS ${SECURITY_LIBRARY} ${FOUNDATION_LIBRARY} libmicrohttpd.a c++)
    SET(CURL_INCLUDE_DIR /opt/local/include/curl)
    SET(CURL_LIBRARIES curl.a nghttp2.a z.a ssl.a )

ELSE(APPLE)
#        target_link_libraries(${CMAKE_PROJECT_NAME} libmicrohttpd.so)
    FIND_PACKAGE(CURL)
        IF(CURL_FOUND)
            INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIR})
        ENDIF(CURL_FOUND)
        set(CURL_LIBRARIES /usr/local/lib/libcurl.a /usr/lib/x86_64-linux-gnu/libz.a /usr/lib/x86_64-linux-gnu/libssl.so libcrypto.so)
ENDIF (APPLE)


        TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${CURL_LIBRARIES} ${EXTRA_LIBS} ${CMAKE_THREAD_LIBS_INIT} )

       message("${EXTRA_LIBS}")
##############################################################


IF($ENV{TEST_ENABLE})
    enable_testing()
    add_library(testlib ${sources})

    function(TEST name)
        add_executable(${name} test/${name}.c)
        target_link_libraries(${name} testlib ${CURL_LIBRARIES} ${EXTRA_LIBS} ${CMAKE_THREAD_LIBS_INIT}  ${GLIB_LIBRARIES})
#        target_link_libraries(${name} testlib /usr/local/lib/libcurl.so ${CMAKE_THREAD_LIBS_INIT}  ${GLIB_LIBRARIES})
#IF(APPLE)
#        target_link_libraries(${name} "-e_${name}")
#ELSE(APPLE)
#        target_link_libraries(${name} "-e${name}")
#ENDIF(APPLE)

        add_test(${name} bin/${name}  ${ARGN})
    endfunction(TEST)


#    TEST(getfromemail_ok)
#    TEST(getfromemail_nok)
#    TEST(mila_accept_ok)
#    TEST(mila_ok)
    TEST(mail_ok)

ENDIF($ENV{TEST_ENABLE})


