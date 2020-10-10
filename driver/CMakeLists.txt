#---
# define the canopen stack library
#
add_library(CanopenDriver)

#---
# define interface
#
target_include_directories(CanopenDriver
  PUBLIC
    include
  )

#---
# specify the implementation files
#
target_sources(CanopenDriver
  PRIVATE
    source/co_can_dummy.c       # dummy CAN driver
    source/co_can_sim.c         # simulated CAN for testsuite
    #---
    source/co_timer_dummy.c     # dummy timer driver
    source/co_timer_swcycle.c   # timer based on cyclic service call
    #---
    source/co_nvm_dummy.c       # dummy nvm driver
    source/co_nvm_sim.c         # simulated nvm for testsuite
)

#---
# specify the dependencies for this application
#
target_link_libraries(CanopenDriver Canopen)
