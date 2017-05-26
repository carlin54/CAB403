cmake_minimum_required(VERSION 3.3.2)

project(CAB403)

find_package(Threads)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")
set(SOURCE_FILES main.c)

add_definitions(-D_GNU_SOURCE)

add_executable(Server ${SOURCE_FILES} main.c)

target_link_libraries(Server ${CMAKE_THREAD_LIBS_INIT} )



