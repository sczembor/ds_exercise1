cmake_minimum_required (VERSION 2.8.11)

Project (ds_exercise1)

add_executable(client client.c keys.c)
add_executable(server server.c keys.c)
add_library(keys SHARED keys.c)
target_include_directories(keys PUBLIC)
find_library(LIBRT rt) 
if(LIBRT)
target_link_libraries(server ${LIBRT})
target_link_libraries(client ${LIBRT})
endif()

find_package (Threads)
target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT})