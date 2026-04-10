################################################################################
# CMakeLists.txt — ARM Cortex-M7 dynamically loaded ELF module
#
# Requirements:
#   - arm-none-eabi-gcc in PATH (Linux/macOS)
#     or TOOLCHAIN_PATH defined in cmake/arm-none-eabi-toolchain.cmake (Windows)
#   - CMake >= 3.25
#   - Ninja (recommended for cross-compilation with Visual Studio)
#
# Usage from command line:
#
#   cmake --preset ${{ matrix.preset }} -DINCLUDES_API_PATH="../Mk/Mk/Includes"
#   cmake --build --preset ${{ matrix.preset }}
#
################################################################################

cmake_minimum_required(VERSION 3.25)

# The toolchain must be defined BEFORE project()
# It is passed as an argument (-DCMAKE_TOOLCHAIN_FILE=...) or via a preset.

project(pong
    LANGUAGES C ASM
    VERSION   1.0.0
    DESCRIPTION "ARM Cortex-M7 dynamically loaded ELF module"
)

################################################################################
# Paths
################################################################################

set(SOURCES_PATH      "${CMAKE_CURRENT_SOURCE_DIR}/Pong/Sources")
set(INCLUDES_PATH     "${CMAKE_CURRENT_SOURCE_DIR}/Pong/Includes")
set(LD_SCRIPT         "${CMAKE_CURRENT_SOURCE_DIR}/Pong/Make/linker.ld")

# Sanity checks
if(NOT EXISTS "${SOURCES_PATH}")
    message(FATAL_ERROR "SOURCES_PATH '${SOURCES_PATH}' not found.")
endif()
if(NOT EXISTS "${INCLUDES_PATH}")
    message(FATAL_ERROR "INCLUDES_PATH '${INCLUDES_PATH}' not found.")
endif()
if(NOT EXISTS "${INCLUDES_API_PATH}")
    message(FATAL_ERROR "INCLUDES_API_PATH '${INCLUDES_API_PATH}' not found.")
endif()
if(NOT EXISTS "${LD_SCRIPT}")
    message(FATAL_ERROR "Linker script '${LD_SCRIPT}' not found.")
endif()

################################################################################
# Sources
################################################################################

# Recursively collect .c and .asm files
file(GLOB_RECURSE C_SOURCES   CONFIGURE_DEPENDS "${SOURCES_PATH}/*.c")
file(GLOB_RECURSE ASM_SOURCES CONFIGURE_DEPENDS "${SOURCES_PATH}/*.asm")

# Recursively collect include directories from both include trees
file(GLOB_RECURSE ALL_HEADERS CONFIGURE_DEPENDS
    "${INCLUDES_PATH}/*.h"
    "${INCLUDES_PATH}/*.asm"
    "${INCLUDES_API_PATH}/*.h"
    "${INCLUDES_API_PATH}/*.asm"
)

# Extract unique directories from header list
set(INCLUDE_DIRS "${INCLUDES_PATH}" "${INCLUDES_API_PATH}")
foreach(_hdr IN LISTS ALL_HEADERS)
    get_filename_component(_dir "${_hdr}" DIRECTORY)
    list(APPEND INCLUDE_DIRS "${_dir}")
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

################################################################################
# Assembler file properties
#
# .asm files require -x assembler-with-cpp to be treated
# as assembly with C preprocessor.
################################################################################

set_source_files_properties(${ASM_SOURCES} PROPERTIES
    LANGUAGE ASM
    COMPILE_OPTIONS "-x;assembler-with-cpp"
)

################################################################################
# Executable
################################################################################

add_executable(${PROJECT_NAME}
    ${C_SOURCES}
    ${ASM_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    SUFFIX      ".elf"
)

target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE MK_EXTERNAL_APPS)

################################################################################
# C dialect compilation options
################################################################################

set(CDIAL_OPTIONS
    -std=c18
    -fno-asm
    -fno-builtin
    -fno-cond-mismatch
    -ffreestanding
    -fsigned-bitfields
    -fsigned-char
    -fsso-struct=little-endian
)

################################################################################
# ARM options
################################################################################

set(ARM_OPTIONS
    -mcpu=cortex-m7
    -mthumb
    -mfloat-abi=hard
    -mfpu=fpv5-sp-d16
    -mno-unaligned-access
    -mlittle-endian
)

################################################################################
# PIC options
#
# Required for dynamically loaded modules:
#   -fPIC       : position-independent code
#   -mlong-calls: all calls use long (32-bit) branches, needed when the module
#                 is relocated into SDRAM far from the kernel text section
################################################################################

set(PIC_OPTIONS
    -fPIC
    -mlong-calls
)

################################################################################
# Warnings
################################################################################

set(WARN_OPTIONS
    -Wpedantic -pedantic-errors -Werror -Wall -Wextra
    -Wdouble-promotion -Wnull-dereference -Wshift-negative-value
    -Wswitch-default -Wswitch-enum -Wswitch-bool -Wswitch-unreachable
    -Wstrict-overflow=5 -Wduplicated-branches -Wduplicated-cond
    -Wdiscarded-qualifiers -Wdiscarded-array-qualifiers
    -Wincompatible-pointer-types -Wint-conversion -Wint-to-pointer-cast
    -Wpointer-to-int-cast -Wdiv-by-zero -Wsystem-headers -Wtrampolines
    -Wfloat-equal -Wno-traditional -Wdeclaration-after-statement
    -Wshadow -Wshadow=local -Wunsafe-loop-optimizations -Wpointer-compare
    -Wunused-macros -Wbad-function-cast
    -Wcast-align -Wcast-align=strict -Wwrite-strings -Wconversion
    -Wjump-misses-init -Waddress-of-packed-member -Wlogical-op
    -Waggregate-return -Waggressive-loop-optimizations -Wpointer-arith
    -Wattributes -Wstrict-prototypes -Wold-style-definition
    --warn-missing-parameter-type -Wmissing-prototypes -Wmissing-declarations
    -Wmultichar -Wnormalized=none -Wattribute-warning
    -Wdeprecated -Wdeprecated-declarations -Woverflow
    -Woverride-init-side-effects -Wpacked -Wno-packed-bitfield-compat
    -Wpadded -Wredundant-decls -Wnested-externs -Winline
    -Wno-long-long -Wvla -Woverlength-strings
)

################################################################################
# Common options (C + ASM)
################################################################################

set(COMMON_OPTIONS
    ${ARM_OPTIONS}
    ${PIC_OPTIONS}
    -fanalyzer          # static analyzer
    -fmessage-length=0
    -nostdinc
    -fstack-usage
)

################################################################################
# Debug / Release options
#
# CMake automatically injects CMAKE_BUILD_TYPE into flags;
# we disable its default flags (CACHE "") and manage our own.
################################################################################

set(CMAKE_C_FLAGS_DEBUG          "" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELEASE        "" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_RELWITHDEBINFO "" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS_MINSIZEREL     "" CACHE STRING "" FORCE)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_OPT_OPTIONS
        -g3 -ggdb -fno-eliminate-unused-debug-symbols
        -O0 -fno-inline-functions -fno-short-enums -fno-move-loop-invariants
    )
else()  # Release (default)
    set(BUILD_OPT_OPTIONS
        -Ofast -fno-inline-functions -fno-short-enums -fno-move-loop-invariants
    )
endif()

################################################################################
# Apply flags to target
################################################################################

target_compile_options(${PROJECT_NAME} PRIVATE
    # C-only options
    $<$<COMPILE_LANGUAGE:C>:
        ${CDIAL_OPTIONS}
        ${COMMON_OPTIONS}
        ${BUILD_OPT_OPTIONS}
        ${WARN_OPTIONS}
        -x c
    >
    # ASM options (-x assembler-with-cpp is in set_source_files_properties)
    $<$<COMPILE_LANGUAGE:ASM>:
        ${COMMON_OPTIONS}
        ${BUILD_OPT_OPTIONS}
        ${WARN_OPTIONS}
    >
)

################################################################################
# Linker
#
# -shared              : produce a shared object (dynamically loadable ELF)
# -z max-page-size=... : set the maximum page size to 64 KB to match the
#                        Mk dynamic loader page granularity
################################################################################

target_link_options(${PROJECT_NAME} PRIVATE
    -nostartfiles
    -nodefaultlibs
    -nolibc
    -nostdlib
    -shared
    -z max-page-size=65536
    # Linker script
    -T "${LD_SCRIPT}"
    # Map file
    -Wl,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map
)

################################################################################
# Post-build: strip + size
################################################################################

set(ELF_FILE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_STRIP_TOOL}" -s "${ELF_FILE}"
        COMMAND "${CMAKE_SIZE_TOOL}" --format=berkeley --totals "${ELF_FILE}"
        COMMENT "Post-build: strip -> size"
        VERBATIM
    )
else()
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_SIZE_TOOL}" --format=berkeley --totals "${ELF_FILE}"
        COMMENT "Post-build: size (no strip in Debug)"
        VERBATIM
    )
endif()

################################################################################
# Utility target 'print' (size only, no rebuild)
################################################################################

add_custom_target(print
    COMMAND ${CMAKE_SIZE_TOOL} --format=berkeley --totals "${ELF_FILE}"
    COMMENT "  SIZE  ${PROJECT_NAME}.elf"
)

################################################################################
# Configuration summary
################################################################################

message(STATUS "Build type   : ${CMAKE_BUILD_TYPE}")
message(STATUS "Sources      : ${SOURCES_PATH}")
message(STATUS "Includes     : ${INCLUDES_PATH}")
message(STATUS "Includes API : ${INCLUDES_API_PATH}")
message(STATUS "Linker script: ${LD_SCRIPT}")
