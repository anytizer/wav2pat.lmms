cmake_minimum_required(VERSION 3.16)
project(wav2pat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(wav2pat
    src/main.cpp
    src/wav2pat.h
)

target_link_libraries(wav2pat PRIVATE Qt6::Core)
