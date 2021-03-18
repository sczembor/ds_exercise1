cmake_minimum_required (VERSION 2.8.11)
project(ds_excercise1)

add_executable(client client.c lib.c)
add_executable(server server.c lib.c)
