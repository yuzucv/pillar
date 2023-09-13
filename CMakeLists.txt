# cmake minimum required version
cmake_minimum_required(VERSION 3.19)

# ----------------------------------------------
# Start pillar project
# ----------------------------------------------
include(cmake/utils.cmake)
pillar_extract_version()

project(
  yuzu
  VERSION ${PILLAR_VERSION}
  LANGUAGES CXX)
message(STATUS "Build yuzu::pillar: ${PILLAR_VERSION}")

# ----------------------------------------------
# C/CXX Standard
# ----------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ARTICRAFT_OUT "out")
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(INTERNAL_PILLAR OFF)
else()
  set(INTERNAL_PILLAR ON)
endif()

if(NOT DEFINED ENABLE_PILLAR_TESTS)
  if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(ENABLE_PILLAR_TESTS ON)
  else()
    set(ENABLE_PILLAR_TESTS OFF)
  endif()
endif()
message(STATUS "Build pillar tests: ${ENABLE_PILLAR_TESTS}")

# ----------------------------------------------
# Build Type
# ----------------------------------------------
if(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -O0")
  add_definitions(-DDEBUG -D_DEBUG -DDEBUG_MODE)
  message("Debug mode:${CMAKE_C_FLAGS_DEBUG}")
elseif(CMAKE_BUILD_TYPE AND (CMAKE_BUILD_TYPE STREQUAL "Release"))
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -O3")
  add_definitions(-DNDEBUG -DRELEASE_MODE -DNDEBUG_MODE)
  message("Release mode:${CMAKE_C_FLAGS_RELEASE}")
endif()

# ----------------------------------------------
# Identifying the OS
# ----------------------------------------------
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  message("This is Windows.")
  if(MSVC)
    add_compile_options(/utf-8)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DUNICODE -D_UNICODE)
  endif()
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
  message("This is Linux.")
  add_compile_options(-fPIC)
elseif(CMAKE_SYSTEM_NAME MATCHES "Darwin")
  message("This is Mac OS X.")
endif()

file(GLOB_RECURSE SRC_NESTED src/*.cc)
add_library(pillar OBJECT ${SRC_NESTED})
add_library(yuzu::pillar ALIAS pillar)
target_include_directories(pillar PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

if(ENABLE_PILLAR_TESTS)
  add_subdirectory(tests)
endif()
