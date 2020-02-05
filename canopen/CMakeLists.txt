
include_directories(include)
include_directories(config)

file(GLOB_RECURSE SRC_FILES source/*.c)

add_library(${PROJECT_NAME} ${SRC_FILES})
