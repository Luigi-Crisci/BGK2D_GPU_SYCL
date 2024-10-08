cmake_minimum_required(VERSION 3.20.2)
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
	"-O3 -DNDEBUG -march=native -Wno-nan-infinity-disabled"
    CACHE STRING "Flags used by the C++ compiler during release builds." FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
    "-O3 -DNDEBUG -march=native -ffast-math -ggdb -fno-omit-frame-pointer"
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

set(BGK_SYCL_DISABLE_CPU_DEVICE "OFF" CACHE BOOL "Disable CPU device. CPU jobs will be run on the Host Device")
if (BGK_SYCL_DISABLE_CPU_DEVICE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSYCL_DISABLE_CPU_DEVICE")
endif()

if(SYCL_IMPL STREQUAL "AdaptiveCpp")
  find_package(AdaptiveCpp REQUIRED)
elseif(SYCL_IMPL STREQUAL "dpcpp")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsycl")
  
  set(DPCPP_TARGETS "spir64_x86_64")
  if (BGK_SYCL_DISABLE_CPU_DEVICE)
	  set(DPCPP_TARGETS "")
  endif()
  if(DPCPP_WITH_CUDA_BACKEND)
    if (DPCPP_TARGETS STREQUAL "")
    	set(DPCPP_TARGETS "nvptx64-nvidia-cuda")
    else()
	set(DPCPP_TARGETS "${DPCPP_TARGETS},nvptx64-nvidia-cuda")
    endif()

    set(CUDA_ARCH
        ""
        CACHE STRING "CUDA device architecture e.g. sm_70")

    if(NOT CUDA_ARCH STREQUAL "")
      set(CMAKE_CXX_FLAGS
          "${CMAKE_CXX_FLAGS} -Xsycl-target-backend=nvptx64-nvidia-cuda --cuda-gpu-arch=${CUDA_ARCH}"
      )
    endif()
  endif()

  if(DPCPP_WITH_ROCM_BACKEND)
    set(ROCM_ARCH
        ""
        CACHE STRING "ROCm device architecture e.g. gfx908")

    if(NOT ROCM_ARCH STREQUAL "")
    if (DPCPP_TARGETS STREQUAL "")
       set(DPCPP_TARGETS
          "amd_gpu_${ROCM_ARCH}")
    else()
       set(DPCPP_TARGETS
          "${DPCPP_TARGETS},amd_gpu_${ROCM_ARCH}")
    endif()    
    endif()
  endif()

  if(DPCPP_WITH_LZ_BACKEND)
    set(LZ_ARCH
        ""
        CACHE STRING "Level Zero device architecture e.g. acm-g10")

    if(NOT LZ_ARCH STREQUAL "")
	    if (DCPPP_TARGETS STREQUAL "")
                set(DPCPP_TARGETS "${LZ_ARCH}")
	    else() 
	     set(DPCPP_TARGETS "${DPCPP_TARGETS},${LZ_ARCH}")
    		endif()
endif()
  endif()
  message(STATUS ${DPCPP_TARGETS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsycl-targets=${DPCPP_TARGETS}")
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
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMIXEDPRECISION -DHALF_P")
elseif (BGK_PRECISION STREQUAL "MIXED2")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DMIXEDPRECISION")
elseif (BGK_PRECISION STREQUAL "DOUBLE")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDOUBLE_P")
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
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPOF -DCHANNEL")
elseif (BGK_USE_CASE STREQUAL "TGV")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DTGV -DPERIODIC")
elseif (BGK_USE_CASE STREQUAL "VKS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DINFLOW -DOBSTACLE -DDRAG -DVKS")
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

set(BGK_SYCL_IN_ORDER_QUEUE "ON" CACHE BOOL "Enable in-order SYCL queue")
if (BGK_SYCL_IN_ORDER_QUEUE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSYCL_IN_ORDER_QUEUE")
endif()

set(BGK_SYCL_ND_RANGE "OFF" CACHE BOOL "Enable nd_range kernel")
if (BGK_SYCL_ND_RANGE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSYCL_ND_RANGE")
endif()



set(header_files
include/bgk2d/prof_j.hh
include/bgk2d/vtk_xy.hh
include/bgk2d/file_manager.hh
include/bgk2d/vtk_xy_binary.hh
include/bgk2d/input.hh
include/bgk2d/setup.hh
include/bgk2d/utils.hh
include/bgk2d/bcond
include/bgk2d/bcond/bcond_periodic.hh
include/bgk2d/bcond/boundaries.hh
include/bgk2d/bcond/build_bcond.hh
include/bgk2d/bcond/bcond_obs.hh
include/bgk2d/bcond/bcond_inflow.hh
include/bgk2d/bcond/bcond_channel.hh
include/bgk2d/bcond/bcond_driven.hh
include/bgk2d/build_obs.hh
include/bgk2d/vtk_om.hh
include/bgk2d/probe_visc.hh
include/bgk2d/init.hh
include/bgk2d/vtk_om_binary.hh
include/bgk2d/get_mem.hh
include/bgk2d/w_obs.hh
include/bgk2d/time.hh
include/bgk2d/collision.hh
include/bgk2d/check_case.hh
include/bgk2d/hencol.hh
include/bgk2d/diagnostic.hh
include/bgk2d/vtk_visc_binary.hh
include/bgk2d/finalize.hh
include/bgk2d/vtk_obstacle.hh
include/bgk2d/profile.hh
include/bgk2d/prof_i.hh
include/bgk2d/sycl
include/bgk2d/sycl/usm_buffer.hh
include/bgk2d/initialize.hh
include/bgk2d/outdat.hh
include/bgk2d/storage.hh
include/bgk2d/alloca.hh
include/bgk2d/diagno.hh
include/bgk2d/save.hh
include/bgk2d/save_raw.hh
include/bgk2d/probe.hh
include/bgk2d/varm.hh
include/bgk2d/col_MC.hh
include/bgk2d/precision.hh
include/bgk2d/restore_raw.hh
include/bgk2d/draglift.hh
include/bgk2d/kernel_utils.hh
    )

set(source_files
src/initialize.cc
src/vtk_visc_binary.cc
src/restore_raw.cc
src/bcond
src/bcond/bcond_channel.cc
src/bcond/bcond_driven.cc
src/bcond/bcond_periodic.cc
src/bcond/bcond_obs.cc
src/bcond/boundaries.cc
src/bcond/bcond_inflow.cc
src/bcond/build_bcond.cc
src/diagno.cc
src/alloca.cc
src/build_obs.cc
src/time.cc
src/get_mem.cc
src/setup.cc
src/vtk_xy.cc
src/file_manager.cc
src/outdat.cc
src/probe_visc.cc
src/vtk_obstacle.cc
src/save.cc
src/prof_j.cc
src/vtk_om.cc
src/col_MC.cc
src/vtk_om_binary.cc
src/prof_i.cc
src/hencol.cc
src/collision.cc
src/profile.cc
src/varm.cc
src/storage.cc
src/save_raw.cc
src/init.cc
src/vtk_xy_binary.cc
src/finalize.cc
src/probe.cc
src/diagnostic.cc
src/input.cc
src/bgk2d.cc
src/check_case.cc
src/w_obs.cc
src/draglift.cc
src/kernel_utils.cc
    )

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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /experimental:color")
  endif()
endif()

message("################# Configuration parameters #################")
message("   BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
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
if (BGK_SYCL_IN_ORDER_QUEUE)
  message("   BGK_SYCL_IN_ORDER_QUEUE: ON")
else()
  message("   BGK_SYCL_IN_ORDER_QUEUE: OFF")
endif()
if (BGK_SYCL_ND_RANGE)
  message("   BGK_SYCL_ND_RANGE: ON")
else()
  message("   BGK_SYCL_ND_RANGE: OFF")
endif()
if (BGK_SYCL_DISABLE_CPU_DEVICE)
  message("   BGK_SYCL_DISABLE_CPU_DEVICE: ON")
else()
  message("   BGK_SYCL_DISABLE_CPU_DEVICE: OFF")
endif()
message("###########################################################")
