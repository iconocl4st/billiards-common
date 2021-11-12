cmake_minimum_required(VERSION 3.13)
project(billiards_common)
add_library(billiards_common INTERFACE src/billiards_common/utils/fs.h src/billiards_common/unql/RecordInfo.h src/billiards_common/unql/Connection.h src/billiards_common/unql/Store.h src/billiards_common/unql/Record.h src/billiards_common/unql/Buffer.h src/billiards_common/utils/current_time.h)

target_include_directories(billiards_common INTERFACE src/)

install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ FILES_MATCHING PATTERN "*.h")
install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ FILES_MATCHING PATTERN "*.hpp")
