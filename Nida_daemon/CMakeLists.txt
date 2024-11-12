cmake_minimum_required(VERSION 3.10)
project(nida_daemon 
        VERSION 1.0
        DESCRIPTION "NIDA Daemon with FIFO Queue"
        LANGUAGES C)

# Require C11 standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Compiler flags
set(COMMON_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -Wcast-align
    -Wcast-qual
    -Wconversion
    -Wdouble-promotion
    -Wfloat-equal
    -Wpointer-arith
    -Wshadow
    -Wswitch-default
    -Wuninitialized
    -Wlogical-op
    -Wduplicated-cond
    -Wduplicated-branches
    -Wnull-dereference
)

# Debug specific flags
set(CMAKE_C_FLAGS_DEBUG "-O0 -g3 -DDEBUG")

# Release specific flags
set(CMAKE_C_FLAGS_RELEASE "-O2 -DNDEBUG")

# Add executable
add_executable(nida_daemon src/nida_daemon.c)
# Apply common flags
target_compile_options(nida_daemon PRIVATE ${COMMON_FLAGS})

# Define options for library and include paths
option(PORTALS_LIB_DIR "Path to the Portals library" "")
option(PORTALS_INCLUDE_DIR "Path to the Portals include directory" "")

# Find the Portals library
find_library(
  PORTALS_LIB
  NAMES portals
  PATHS ${PORTALS_LIB_DIR}
  NO_DEFAULT_PATH)

if(NOT PORTALS_LIB)
  message(FATAL_ERROR "Portals library not found at: ${PORTALS_LIB_DIR}")
endif()

message(STATUS "Portals library found at: ${PORTALS_LIB}")
set(PORT_LIB "${PROJECT_NAME}_portals_lib")

# Handle include directory
if(PORTALS_INCLUDE_DIR)
  # Use provided include directory
  if(NOT EXISTS "${PORTALS_INCLUDE_DIR}/portals4.h")
    message(FATAL_ERROR "Could not find portals4.h in specified include directory: ${PORTALS_INCLUDE_DIR}")
  endif()
else()
  # Try to infer include directory from library location
  get_filename_component(PORTALS_LIB_DIR_PARENT ${PORTALS_LIB} DIRECTORY)
  set(POSSIBLE_INCLUDE_DIRS
      "${PORTALS_LIB_DIR_PARENT}/include"
      "${PORTALS_LIB_DIR_PARENT}/../include"
      "${PORTALS_LIB_DIR_PARENT}/../../include"
      "${PORTALS_LIB_DIR_PARENT}/../portals/include"
      "${PORTALS_LIB_DIR_PARENT}/../../portals/include")

  # Search for portals4.h in possible locations
  set(FOUND_INCLUDE_DIR FALSE)
  foreach(DIR ${POSSIBLE_INCLUDE_DIRS})
    if(EXISTS "${DIR}/portals4.h")
      set(PORTALS_INCLUDE_DIR "${DIR}")
      set(FOUND_INCLUDE_DIR TRUE)
      break()
    endif()
  endforeach()

  if(NOT FOUND_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find portals4.h in any standard location relative to ${PORTALS_LIB}")
  endif()
endif()

message(STATUS "Portals include directory: ${PORTALS_INCLUDE_DIR}")



# Add executable
add_executable(test_client tests/test_client.c)
# Apply common flags
target_compile_options(test_client PRIVATE ${COMMON_FLAGS})

# Installation rules
install(TARGETS nida_daemon
        RUNTIME DESTINATION bin)

# Print configuration summary
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}")
message(STATUS "CMAKE_C_FLAGS_DEBUG: ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "CMAKE_C_FLAGS_RELEASE: ${CMAKE_C_FLAGS_RELEASE}")

