
include_directories(../canopen/cfg)
include_directories(../canopen/inc)

include_directories(app)
include_directories(driver)
include_directories(testfrm)
include_directories(tests)

file(GLOB_RECURSE SRC_FILES *.c)

add_executable(${PROJECT_NAME}_tests ${SRC_FILES})
target_link_libraries(${PROJECT_NAME}_tests ${PROJECT_NAME})
