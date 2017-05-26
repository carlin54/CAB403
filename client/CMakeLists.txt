cmake_minimum_required(VERSION 3.3.2)

project(CAB403)

find_package(Threads)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ")
set(SOURCE_FILES main.c menu.c network.c read.c utility.c write.c)

add_executable(Client ${SOURCE_FILES} main.c)

target_link_libraries( Client ${CMAKE_THREAD_LIBS_INIT} )
