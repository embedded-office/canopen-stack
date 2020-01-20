cmake_minimum_required(VERSION 3.0.0)
project(canopen-stack VERSION 3.3.0)

include(CTest)
enable_testing()

include_directories(canopen/inc)
include_directories(canopen/cfg)

file(GLOB_RECURSE SOURCES
    canopen/src/*.c
    canopen/inc/*.h
)

add_library(${PROJECT_NAME} ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
