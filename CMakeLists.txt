cmake_minimum_required (VERSION 2.8.11)

SET(GCMAKE_EXE_LINKER_FLAGS -lrt)

add_executable(client client.c lib.c)
add_executable(server server.c lib.c)

add_library(-lrt)
target_link_libraries (client -lrt)
target_link_libraries (server -lrt)