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

# Fetch fmt for I/O
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG        e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

# FetchContent for frozen
FetchContent_Declare(
  frozen
  GIT_REPOSITORY https://github.com/serge-sans-paille/frozen
  GIT_TAG        master) # Use the master branch
FetchContent_MakeAvailable(frozen)

set(header_files
include/bgk2d/prof_j.hh
include/bgk2d/vtk_xy.hh
include/bgk2d/file_manager.hh
include/bgk2d/vtk_xy_binary.hh
include/bgk2d/input.hh
include/bgk2d/setup.hh
include/bgk2d/utils.hh
include/bgk2d/build_obs.hh
include/bgk2d/init.hh
include/bgk2d/get_mem.hh
include/bgk2d/w_obs.hh
include/bgk2d/time.hh
include/bgk2d/check_case.hh
include/bgk2d/hencol.hh
include/bgk2d/build_bcond.hh
include/bgk2d/vtk_obstacle.hh
include/bgk2d/prof_i.hh
include/bgk2d/initialize.hh
include/bgk2d/outdat.hh
include/bgk2d/storage.hh
include/bgk2d/alloca.hh
include/bgk2d/varm.hh
include/bgk2d/precision.hh
include/bgk2d/restore_raw.hh
)

set(source_files
src/initialize.cc
src/restore_raw.cc
src/alloca.cc
src/build_obs.cc
src/time.cc
src/get_mem.cc
src/setup.cc
src/vtk_xy.cc
src/file_manager.cc
src/outdat.cc
src/vtk_obstacle.cc
src/prof_j.cc
src/prof_i.cc
src/hencol.cc
src/varm.cc
src/storage.cc
src/build_bcond.cc
src/init.cc
src/vtk_xy_binary.cc
src/input.cc
src/bgk2d.cc
src/check_case.cc
src/w_obs.cc
)

if(CMAKE_GENERATOR STREQUAL "Ninja")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-fdiagnostics-color=always)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/experimental:color)
  endif()
endif()

add_executable(bgk2dSYCL ${header_files} ${source_files})

get_property(
  mdspan_INCLUDE_DIRS
  TARGET std::mdspan
  PROPERTY INTERFACE_INCLUDE_DIRECTORIES)


get_property(
  frozen_INCLUDE_DIRS
  TARGET frozen
  PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
  
target_include_directories(bgk2dSYCL PUBLIC include/bgk2d ${mdspan_INCLUDE_DIRS} ${frozen_INCLUDE_DIRS})
target_link_libraries(bgk2dSYCL fmt::fmt)