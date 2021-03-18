cmake_minimum_required (VERSION 2.8.11)

SET(GCC_COVERAGE_LINK_FLAGS "-lrt")
add_definitions(${GCC_COVERAGE_LINK_FLAGS})

add_executable(client client.c lib.c)
add_executable(server server.c lib.c)