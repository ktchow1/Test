cmake_minimum_required(VERSION 3.18.2)
project(Oms)


set(CMAKE_C_COMPILER /usr/bin/gcc-10)
set(CMAKE_CXX_COMPILER /usr/bin/g++-10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_definitions(-std=c++20)
add_definitions(-g)

###################
### (1) include ###
###################
include_directories(
    include 
    ~/dev/pgsql/include
    /mnt/d/dev/boost_1_72_0 
)

##################
### (2) source ###
##################
file(GLOB SOURCES
     "src/*.cpp"
     "src_hacker/*.cpp"
     "src_psql/*.cpp"
)
add_executable(Test ${SOURCES})       # for building executable
# add_library(Test STATIC ${SOURCES}) # for building static lib .a
# add_library(Test SHARED ${SOURCES}) # for building shared lib .so

##################################
### (3) link path and link lib ###
##################################
target_link_libraries(Test -L/lib/x86_64-linux-gnu)
target_link_libraries(Test -L~/dev/pgsql/lib)

target_link_libraries(Test -lstdc++fs) # filesystem lib
target_link_libraries(Test -ldl) # dynamically loaded lib, for boost::stacktrace's dladdr()
target_link_libraries(Test -lrt) # realtime extension lib, for shm_open()
target_link_libraries(Test -lpq) # psql API
target_link_libraries(Test -lgtest)
target_link_libraries(Test -lpthread)

