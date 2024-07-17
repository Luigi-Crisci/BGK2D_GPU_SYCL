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
  GIT_TAG e69e5f977d458f2650bb346dadf2ad30c5320281) # 10.2.1
FetchContent_MakeAvailable(fmt)

# FetchContent for frozen
FetchContent_Declare(
  frozen
  GIT_REPOSITORY https://github.com/serge-sans-paille/frozen
  GIT_TAG master) # Use the master branch
FetchContent_MakeAvailable(frozen)

# Default build flags
set(CMAKE_CXX_FLAGS_DEBUG
    "-O0 -g -DDEBUG -fno-omit-frame-pointer"
    CACHE STRING "Flags used by the C++ compiler during debug builds." FORCE)
set(CMAKE_CXX_FLAGS_RELEASE
    "-O3 -DNDEBUG -march=native -ffast-math"
    CACHE STRING "Flags used by the C++ compiler during release builds." FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "-O2 -DNDEBUG -march=native -ffast-math -g -fno-omit-frame-pointer"
    CACHE
      STRING
      "Flags used by the C++ compiler during release builds with debug info."
      FORCE)

set(SYCL_IMPL
    ""
    CACHE STRING "SYCL implementation (AdaptiveCpp, dpcpp, triSYCL)")

set(supported_implementations AdaptiveCpp dpcpp)

list(FIND supported_implementations ${SYCL_IMPL} impl_idx)

if(NOT SYCL_IMPL OR impl_idx EQUAL -1)
  message(
    FATAL_ERROR
      "Please specify SYCL_IMPL (one of: ${supported_implementations})")
endif()

message("Selected SYCL implementation: ${SYCL_IMPL}")

if(SYCL_IMPL STREQUAL "AdaptiveCpp")
  find_package(AdaptiveCpp REQUIRED)
elseif(SYCL_IMPL STREQUAL "dpcpp")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsycl")

  if(DPCPP_WITH_CUDA_BACKEND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsycl-targets=nvptx64-nvidia-cuda")

    set(CUDA_ARCH
        ""
        CACHE STRING "CUDA device architecture e.g. sm_70")

    if(NOT CUDA_ARCH STREQUAL "")
      set(CMAKE_CXX_FLAGS
          "${CMAKE_CXX_FLAGS} -Xsycl-target-backend --cuda-gpu-arch=${CUDA_ARCH}"
      )
    endif()
  endif()

  if(DPCPP_WITH_ROCM_BACKEND)
    set(ROCM_ARCH
        ""
        CACHE STRING "ROCm device architecture e.g. gfx908")

    if(NOT ROCM_ARCH STREQUAL "")
      set(CMAKE_CXX_FLAGS
          "${CMAKE_CXX_FLAGS} -fsycl-targets=amd_gpu_${ROCM_ARCH}")
    endif()
  endif()

  if(DPCPP_WITH_LZ_BACKEND)
    set(LZ_ARCH
        ""
        CACHE STRING "Level Zero device architecture e.g. acm-g10")

    if(NOT LZ_ARCH STREQUAL "")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsycl-targets=${LZ_ARCH}")
    endif()
  endif()
endif()

set(BGK_SYCL_MALLOC_SHARED
    ""
    CACHE BOOL "Use malloc_shared for SYCL buffers")
if(BGK_SYCL_MALLOC_SHARED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBGK_SYCL_MALLOC_SHARED")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBKG_SYCL_MALLOC_DEVICE")
endif()

set(BGK_SYCL_ENABLE_PREFETCH
    ""
    CACHE BOOL "Enable sycl::malloc_shared prefetch")
if(BGK_SYCL_ENABLE_PREFETCH)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -BGK_SYCL_ENABLE_PREFETCH")
endif()

#----------------------------------------------
# precision used
#----------------------------------------------
# default
set(BGK_PRECISION "SINGLE" CACHE STRING "BGK precision (SINGLE, MIXED1, MIXED2, DOUBLE)")
if (BGK_PRECISION STREQUAL "SINGLE")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSINGLE")
elseif (BGK_PRECISION STREQUAL "MIXED1")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMIXED1")
elseif (BGK_PRECISION STREQUAL "MIXED2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMIXED2")
elseif (BGK_PRECISION STREQUAL "DOUBLE")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDOUBLE")
else()
  message(FATAL_ERROR "Unsupported BGK_PRECISION: ${BGK_PRECISION}")
endif()
#----------------------------------------------
# implementations 
#----------------------------------------------
#
# default, ORIGINAL
set(BGK_KERNEL_TYPE "FUSED" CACHE STRING "BGK kernel type (ORIG, FUSED)")
if (BGK_KERNEL_TYPE STREQUAL "ORIG")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DORIG")
elseif (BGK_KERNEL_TYPE STREQUAL "FUSED")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DFUSED")
else()
  message(FATAL_ERROR "Unsupported BGK_KERNEL_TYPE: ${BGK_KERNEL_TYPE}")
endif()
#----------------------------------------------
# test case
#----------------------------------------------
#
# default, Lid Driven Cavity
set(BGK_USE_CASE "LDC" CACHE STRING "BGK use case (LDC, POF, TGV, VKS)")
if (BGK_USE_CASE STREQUAL "LDC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLDC")
elseif (BGK_USE_CASE STREQUAL "POF")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPOF")
elseif (BGK_USE_CASE STREQUAL "TGV")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTGV")
elseif (BGK_USE_CASE STREQUAL "VKS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVKS")
else()
  message(FATAL_ERROR "Unsupported BGK_USE_CASE: ${BGK_USE_CASE}")
endif()

set(BGK_ENABLE_DEBUG_PRINT "OFF" CACHE BOOL "Enable debug print")
if (BGK_ENABLE_DEBUG_PRINT)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG_1 -DDEBUG_2 -DDEBUG_3")
endif()

set(BGK_DEBUG_HOST_QUEUE "OFF" CACHE BOOL "Enable debug host queue")
if (BGK_DEBUG_HOST_QUEUE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEBUG_HOST_QUEUE")
endif()


set(header_files
    include/bgk2d/diagno.hh
    include/bgk2d/init.hh
    include/bgk2d/save_raw.hh
    include/bgk2d/file_manager.hh
    include/bgk2d/probe.hh
    include/bgk2d/vtk_obstacle.hh
    include/bgk2d/hencol.hh
    include/bgk2d/utils.hh
    include/bgk2d/input.hh
    include/bgk2d/boundaries.hh
    include/bgk2d/build_bcond.hh
    include/bgk2d/prof_i.hh
    include/bgk2d/initialize.hh
    include/bgk2d/setup.hh
    include/bgk2d/col_MC.hh
    include/bgk2d/sycl
    include/bgk2d/sycl/usm_buffer.hh
    include/bgk2d/save.hh
    include/bgk2d/precision.hh
    include/bgk2d/prof_j.hh
    include/bgk2d/get_mem.hh
    include/bgk2d/collision.hh
    include/bgk2d/build_obs.hh
    include/bgk2d/time.hh
    include/bgk2d/vtk_om_binary.hh
    include/bgk2d/storage.hh
    include/bgk2d/outdat.hh
    include/bgk2d/w_obs.hh
    include/bgk2d/vtk_xy_binary.hh
    include/bgk2d/varm.hh
    include/bgk2d/probe_visc.hh
    include/bgk2d/vtk_xy.hh
    include/bgk2d/check_case.hh
    include/bgk2d/vtk_om.hh
    include/bgk2d/restore_raw.hh
    include/bgk2d/vtk_visc_binary.hh
    include/bgk2d/finalize.hh
    include/bgk2d/diagnostic.hh
    include/bgk2d/bcond_driven.hh
    include/bgk2d/alloca.hh
    include/bgk2d/profile.hh)

set(source_files
    src/col_MC.cc
    src/storage.cc
    src/varm.cc
    src/setup.cc
    src/probe.cc
    src/w_obs.cc
    src/vtk_xy.cc
    src/alloca.cc
    src/time.cc
    src/save_raw.cc
    src/probe_visc.cc
    src/collision.cc
    src/build_bcond.cc
    src/bgk2d.cc
    src/outdat.cc
    src/diagno.cc
    src/hencol.cc
    src/get_mem.cc
    src/save.cc
    src/bcond_driven.cc
    src/vtk_visc_binary.cc
    src/vtk_obstacle.cc
    src/check_case.cc
    src/file_manager.cc
    src/vtk_om_binary.cc
    src/vtk_xy_binary.cc
    src/finalize.cc
    src/boundaries.cc
    src/input.cc
    src/vtk_om.cc
    src/restore_raw.cc
    src/diagnostic.cc
    src/initialize.cc
    src/init.cc
    src/build_obs.cc
    src/prof_i.cc
    src/prof_j.cc
    src/profile.cc)

add_executable(bgk2dSYCL ${header_files} ${source_files})

get_property(
  mdspan_INCLUDE_DIRS
  TARGET std::mdspan
  PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

get_property(
  frozen_INCLUDE_DIRS
  TARGET frozen
  PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

target_include_directories(
  bgk2dSYCL PUBLIC include/bgk2d ${mdspan_INCLUDE_DIRS} ${frozen_INCLUDE_DIRS})
target_link_libraries(bgk2dSYCL fmt::fmt)

# Enable colored output for Ninja
if(CMAKE_GENERATOR STREQUAL "Ninja")
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    add_compile_options(-fdiagnostics-color=always)
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options(/experimental:color)
  endif()
endif()

message("################# Configuration parameters #################")
message("   BGK_USE_CASE: ${BGK_USE_CASE}")
message("   BGK_PRECISION: ${BGK_PRECISION}")
message("   BGK_KERNEL_TYPE: ${BGK_KERNEL_TYPE}")
message("   SYCL_IMPL: ${SYCL_IMPL}")
if(BGK_SYCL_MALLOC_SHARED)
  message("   BGK_SYCL_MALLOC_SHARED: ON")
else()
  message("   BGK_SYCL_MALLOC_SHARED: OFF")
endif()
if(BGK_SYCL_MALLOC_SHARED)
  if(BGK_SYCL_ENABLE_PREFETCH)
    message("   BGK_SYCL_ENABLE_PREFETCH: ON")
  else()
    message("   BGK_SYCL_ENABLE_PREFETCH: OFF")
  endif()
endif()
if (BGK_ENABLE_DEBUG_PRINT)
  message("   BGK_ENABLE_DEBUG_PRINT: ON")
else()
  message("   BGK_ENABLE_DEBUG_PRINT: OFF")
endif()
if (BGK_DEBUG_HOST_QUEUE)
  message("   BGK_DEBUG_HOST_QUEUE: ON")
else()
  message("   BGK_DEBUG_HOST_QUEUE: OFF")
endif()
message("###########################################################")
