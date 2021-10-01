cmake_minimum_required(VERSION 3.13)
project(billiards_common)
add_library(billiards_common INTERFACE)
target_include_directories(billiards_common INTERFACE src/)

install(TARGETS billiards_common 
        LIBRARY DESTINATION src/
        PUBLIC_HEADER DESTINATION /usr/include
)
