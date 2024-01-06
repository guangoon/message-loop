cmake_minimum_required(VERSION 3.0.0)
project(message-loop VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(message-loop main.cc message_loop.cc)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
