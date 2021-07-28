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
