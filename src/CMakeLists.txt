# Copyright © Christophe Demko <christophe.demko@univ-lr.fr>, 2024
# Licensed under the BSD-3 License. See the LICENSE file for details.
cmake_minimum_required(VERSION 3.1)

project(NonoGram C)

set(CMAKE_C_STANDARD 11)

if(POLICY CMP0110)
  cmake_policy(SET CMP0110 NEW)
endif()

# Add your source files here
set(SOURCES nonogram.c cJSON.c pnmio.c)

# Add your header files here
set(HEADERS nonogram.h cJSON.h pnmio.h)

# Create the static library
add_library(nonogram-static STATIC ${SOURCES} ${HEADERS})
set_target_properties(nonogram-static PROPERTIES OUTPUT_NAME nonogram)

# Create the shared library
add_library(nonogram-shared SHARED ${SOURCES} ${HEADERS})
set_target_properties(nonogram-shared PROPERTIES OUTPUT_NAME nonogram)

# Enable testing
enable_testing()

find_program(VALGRIND valgrind)

find_library(EFENCE efence)
if(EFENCE)
  target_link_libraries(nonogram-shared debug ${EFENCE})
  target_link_libraries(nonogram-static debug ${EFENCE})
endif()

file(GLOB FILES "${CMAKE_CURRENT_SOURCE_DIR}/test-*.c")

foreach(FILENAME ${FILES})
  get_filename_component(SRC ${FILENAME} NAME)
  get_filename_component(TEST ${FILENAME} NAME_WE)
  add_executable(${TEST} ${SRC} ${SOURCES} ${HEADERS} nonogram.inc)
  add_dependencies(${TEST} nonogram-shared)
  target_link_libraries(${TEST} nonogram-shared)
  if(VALGRIND)
    add_test(
      "${TEST}[valgrind]" ${VALGRIND} --leak-check=full --quiet --error-exitcode=1 ./${TEST}
    )
  endif()
  add_test(${TEST} ./${TEST})
endforeach()

# Enable packaging
set(CPACK_SOURCE_GENERATOR "ZIP")
set(CPACK_SOURCE_IGNORE_FILES "/build/;\.vscode;.*~;${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

add_executable(nonogram-solve nonogram-solve.c ${SOURCES} ${HEADERS} nonogram.inc)
add_dependencies(nonogram-solve nonogram-shared)