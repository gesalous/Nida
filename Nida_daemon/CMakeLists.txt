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

