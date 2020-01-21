
include_directories(inc)
include_directories(cfg)

file(GLOB_RECURSE SRC_FILES src/*.c)

add_library(${PROJECT_NAME} ${SRC_FILES})
