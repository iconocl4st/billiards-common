cmake_minimum_required(VERSION 3.13)
project(billiards_common)
add_library(billiards_common INTERFACE)
target_include_directories(billiards_common INTERFACE src/)

install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.hpp")