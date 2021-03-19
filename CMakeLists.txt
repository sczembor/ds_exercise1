cmake_minimum_required (VERSION 2.8.11)

Project (ds_exercise1)

add_executable(client client.c lib.c)
add_executable(server server.c lib.c)

find_library(LIBRT rt) 
if(LIBRT)
target_link_libraries(server ${LIBRT})
target_link_libraries(client ${LIBRT})
endif()

find_package (Threads)
target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT})