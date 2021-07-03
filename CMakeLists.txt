cmake_minimum_required(VERSION 3.16)

project(graph-ranker)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(all_targets
  Main
  Test)

add_executable(Main main.c)
add_executable(Test test.c)

foreach(TARGET ${all_targets})
  target_compile_options(${TARGET} PUBLIC -DEVAL -Wall -Werror -O2 -pipe -static -s -lm)
endforeach(TARGET)
