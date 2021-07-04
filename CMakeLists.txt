cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_FLAGS_RELEASE "-O2")
set(CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_C_FLAGS_MINSIZEREL "-Os")

project(graph-ranker)

add_executable(Main main.c)
add_executable(Test test.c)

target_compile_options(Main PRIVATE -Wall -Werror -pipe -static -s -lm)
