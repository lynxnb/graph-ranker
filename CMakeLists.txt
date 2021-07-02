cmake_minimum_required(VERSION 3.16)

project(graph-ranker)

set(C_STANDARD 11)
set(C_STANDARD_REQUIRED True)

add_executable(Main main.c)

target_compile_options(Main PRIVATE -DEVAL -Wall -Werror -O2 -pipe -static -s -lm)
