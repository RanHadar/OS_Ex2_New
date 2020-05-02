cmake_minimum_required(VERSION 3.15)
project(OS_Ex2_New)

set(CMAKE_CXX_STANDARD 14)

add_executable(OS_Ex2_New main.cpp uthreads.cpp uthreads.h Scheduler.cpp Scheduler.h Thread.cpp Thread.h)