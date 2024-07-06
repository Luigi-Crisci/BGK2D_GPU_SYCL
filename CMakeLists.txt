cmake_minimum_required(VERSION 3.22)
project(bgk2dSYCL)

set(CMAKE_CXX_STANDARD 20)

# Fetch MDSpan
include(FetchContent)
FetchContent_Declare(
  mdspan
  GIT_REPOSITORY https://github.com/kokkos/mdspan.git
  GIT_TAG stable)
FetchContent_MakeAvailable(mdspan)

set(header_files
include/bgk2d/storage.hh
include/bgk2d/precision.hh
include/bgk2d/get_mem.hh
include/bgk2d/time.hh)

set(source_files
src/bgk2d.cc
src/get_mem.cc
src/time.cc)


add_executable(bgk2dSYCL ${header_files} ${source_files})

get_property(
  mdspan_INCLUDE_DIRS
  TARGET std::mdspan
  PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

target_include_directories(bgk2dSYCL PUBLIC include/bgk2d ${mdspan_INCLUDE_DIRS})