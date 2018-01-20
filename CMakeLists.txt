cmake_minimum_required(VERSION 3.9)
project(fft C)

set(CMAKE_C_STANDARD 11)
add_executable(fft fft_an.c fft.c fft.h)
target_link_libraries(fft m)