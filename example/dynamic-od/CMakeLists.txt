#---
# define the test application executable
#
add_executable(DynamicOD)

#---
# test application with test runner and supporting functions
#
target_sources(DynamicOD
  PRIVATE
    app/app_dict.c
    app/clock_app.c
    app/clock_callbacks.c
    app/clock_spec.c
  )
target_include_directories(DynamicOD
  PRIVATE
    app
)

#---
# specify the dependencies for this application
#
target_link_libraries(DynamicOD Canopen CanopenDriver)
