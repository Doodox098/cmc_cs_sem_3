cmake_minimum_required(VERSION 3.23)
project(Cons C)

set(CMAKE_C_STANDARD 99)

add_executable(Cons func.c make_n_p.c ping-pong.c)
