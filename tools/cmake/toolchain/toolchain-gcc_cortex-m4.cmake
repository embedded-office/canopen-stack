#******************************************************************************
#  Copyright 2020 Embedded Office GmbH & Co. KG
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#*****************************************************************************

# The Generic system name is used for embedded targets in CMake
set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Because the cross-compiler cannot directly generate a binary without
# complaining, just test compiling a static library instead of an executable
# program
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)


#*****************************************************************************
#* Define commands

set(CMAKE_C_COMPILER   "arm-none-eabi-gcc.exe"     CACHE FILEPATH "" FORCE)
set(CMAKE_ASM_COMPILER "arm-none-eabi-gcc.exe"     CACHE FILEPATH "" FORCE)
set(CMAKE_LINKER       "arm-none-eabi-gcc.exe"     CACHE FILEPATH "" FORCE)
set(CMAKE_OBJCOPY      "arm-none-eabi-objcopy.exe" CACHE FILEPATH "" FORCE)
set(CMAKE_OBJDUMP      "arm-none-eabi-objdump.exe" CACHE FILEPATH "" FORCE)


#*****************************************************************************
#* Defince command line options

set(CMAKE_C_FLAGS
  "-mthumb -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Wall -Wextra -Wpedantic -Werror"
  CACHE STRING "" FORCE
)
set(CMAKE_ASM_FLAGS
  "${CMAKE_C_FLAGS} -x assembler-with-cpp"
  CACHE STRING "" FORCE
)
set(
  CMAKE_EXE_LINKER_FLAGS
  "-mthumb -mcpu=cortex-m4 -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -nostartfiles"
  CACHE STRING "" FORCE
)

# additional flags depending on the config command line switch
set(CMAKE_C_FLAGS_DEBUG          # --config Debug
  "-O0 -g -gdwarf-2"
  CACHE STRING ""
)
set(CMAKE_C_FLAGS_RELEASE        # --config Release
  "-O2"
  CACHE STRING ""
)
set(CMAKE_C_FLAGS_MINSIZEREL     # --config MinSizeRel
  ""
  CACHE STRING ""
)
set(CMAKE_C_FLAGS_RELWITHDEBINFO # --config RelWithDebInfo
  ""
  CACHE STRING ""
)

# setup standard flags
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)


#*****************************************************************************
#* functions for application specific appending of mapfile and linker script

function(setOutfile target filename)
  set_target_properties(${target} PROPERTIES OUTPUT_NAME "${filename}")
  set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Map,${filename}.map -Xlinker --cref"
    CACHE STRING "" FORCE
  )
endfunction()

function(setLinkerScript target filename)
  set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -T ${filename}.lds"
    CACHE STRING "" FORCE
  )
endfunction()
