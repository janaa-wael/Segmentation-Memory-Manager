cmake_minimum_required(VERSION 4.1)
project(Segmentation_Memory_Manager)

set(CMAKE_CXX_STANDARD 20)

add_executable(Segmentation_Memory_Manager main.cpp
        Process.cpp
        Process.h
        Segment.cpp
        Segment.h
        Allocater_Manager.cpp
        Allocater_Manager.h
        Allocating_Algorithm.cpp
        Allocating_Algorithm.h
        First_Fit_Algorithm.cpp
        First_Fit_Algorithm.h
        Best_Fit_Algorithm.cpp
        Best_Fit_Algorithm.h
        MemoryManager.cpp
        MemoryManager.h)
