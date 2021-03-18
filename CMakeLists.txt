cmake_minimum_required (VERSION 2.8.11)

add_executable(client client.c lib.c)
add_executable(server server.c lib.c)

find_library(LIBRT rt) 
if(LIBRT)
target_link_libraries(target_name ${LIBRT})
endif()