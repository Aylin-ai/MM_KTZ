cmake_minimum_required(VERSION 3.27.0)
project(MM_LR2 VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_executable(MM_LR2 main.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
