cmake_minimum_required(VERSION 3.18.2)
project(algo)


set(CMAKE_C_COMPILER /usr/bin/gcc-10)
set(CMAKE_CXX_COMPILER /usr/bin/g++-10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_definitions(-std=c++20)
add_definitions(-g)

message("-----------------------------------------------")
message("CMAKE_BUILD_TYPE        = ${CMAKE_BUILD_TYPE}")
message("CMAKE_C_FLAGS           = ${CMAKE_C_FLAGS}")
message("CMAKE_C_FLAGS_DEBUG     = ${CMAKE_C_FLAGS_DEBUG}")
message("CMAKE_C_FLAGS_RELEASE   = ${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_CXX_FLAGS         = ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_DEBUG   = ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE = ${CMAKE_CXX_FLAGS_RELEASE}")
message("-----------------------------------------------")

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_C_FLAGS   "-g -O0")
    set(CMAKE_CXX_FLAGS "-g -O0")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(CMAKE_C_FLAGS   "-g -O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS "-g -O3 -DNDEBUG")
endif()

message("-----------------------------------------------")
message("CMAKE_BUILD_TYPE        = ${CMAKE_BUILD_TYPE}")
message("CMAKE_C_FLAGS           = ${CMAKE_C_FLAGS}")
message("CMAKE_C_FLAGS_DEBUG     = ${CMAKE_C_FLAGS_DEBUG}")
message("CMAKE_C_FLAGS_RELEASE   = ${CMAKE_C_FLAGS_RELEASE}")
message("CMAKE_CXX_FLAGS         = ${CMAKE_CXX_FLAGS}")
message("CMAKE_CXX_FLAGS_DEBUG   = ${CMAKE_CXX_FLAGS_DEBUG}")
message("CMAKE_CXX_FLAGS_RELEASE = ${CMAKE_CXX_FLAGS_RELEASE}")
message("-----------------------------------------------")


###################
### (1) include ###
###################
include_directories(
    include 
    ~/dev/pgsql/include
    ~/dev/yaml-cpp/include
    ~/dev/reckless/reckless/include
    /mnt/d/dev/boost_1_72_0 
)

##################
### (2) source ###
##################
file(GLOB SOURCES
     "src/*.cpp"
     "src_hacker/*.cpp"
     "src_ylib/*.cpp"
)
add_executable(Test ${SOURCES})       # for building executable
# add_library(Test STATIC ${SOURCES}) # for building static lib .a
# add_library(Test SHARED ${SOURCES}) # for building shared lib .so

##################################
### (3) link path and link lib ###
##################################
target_link_libraries(Test -L/lib/x86_64-linux-gnu)
target_link_libraries(Test -L~/dev/pgsql/lib)
# target_link_libraries(Test -L~/dev/yaml-cpp/build/debug) # Does not work, why?

target_link_libraries(Test -lstdc++fs) # filesystem lib
target_link_libraries(Test -ldl) # dynamically loaded lib, for boost::stacktrace's dladdr()
target_link_libraries(Test -lrt) # realtime extension lib, for shm_open()
target_link_libraries(Test -lpq) # psql API
target_link_libraries(Test -lgtest)
target_link_libraries(Test -lpthread)
target_link_libraries(Test "/home/ktchow1/dev/yaml-cpp/build/debug/libyaml-cppd.a") 
target_link_libraries(Test "/home/ktchow1/dev/reckless/build/debug/libreckless.a") 

# target_link_libraries(Test -lyaml-cppd) # Does not work, why?
# This problem happens in both my home machine and office machine, 
# this problem happens in yaml-cpp only, in which full path is needed. Why? 




