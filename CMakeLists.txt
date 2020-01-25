cmake_minimum_required( VERSION 3.8 )

project( PwsCore )

add_compile_options( "-Wall" "-Wextra" 
     "-g3" "-ggdb"
     "-std=c++17"
     "-lpthread"
     "-lnlohmanjson"
     )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lX11 -Wall -Wextra -lpthread -g" )

file( GLOB
     CPPS "${PROJECT_SOURCE_DIR}/deamon/*.cc"
     CHH "${PROJECT_SOURCE_DIR}/deamon/*.hpp"
     )

#include_directories( "${PROJECT_SOURCE_DIR}/deamon/" )
add_executable( ${PROJECT_NAME} ${CHH} ${CPPS} )

