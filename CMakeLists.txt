cmake_minimum_required (VERSION 2.8.11)

SET(GCC_COVERAGE_COMPILE_FLAGS "-lrt")

INCLUDE_DIRECTORIES("include")

add_executable(client apps/client.c src/lib.c)
add_executable(server apps/server.c src/lib.c)