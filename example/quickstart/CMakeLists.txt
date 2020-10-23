#---
# define the test application executable
#
add_executable(Quickstart)

#---
# test application with test runner and supporting functions
#
target_sources(Quickstart
  PRIVATE
    app/clock_app.c
    app/clock_callbacks.c
    app/clock_spec.c
  )
target_include_directories(Quickstart
  PRIVATE
    app
)

#---
# specify the dependencies for this application
#
target_link_libraries(Quickstart Canopen CanopenDriver)

#---
# enable warnings and tread as errors
#
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # tread all files as C files 
    target_compile_options(Quickstart PRIVATE /TC)
  endif()
  # enable all warnings
  target_compile_options(Quickstart PRIVATE -Wall
                                            -Wextra
                                            -Wpedantic
                                            -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(Quickstart PRIVATE /W4
                                            /WX)
endif()
