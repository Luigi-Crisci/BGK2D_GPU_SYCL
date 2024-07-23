import sys
import subprocess
import os
import numpy as np

NUM_REPS = 5
fortran_exec_dir = f"{os.getcwd()}/../fortran/RUN"
sycl_exec_dir = f"{os.getcwd()}/../build"

use_case = ["LDC", "VKS"]
use_case_dir = dict(
    LDC = "../fortran/TEST/DrivenCavity",
    VKS = "../fortran/TEST/VonKarmanStreet"
)

hw = ["AMD", "NVIDIA", "INTEL"]
precision = ["SINGLE", "MIXED1", "MIXED2", "DOUBLE"]
implementation = dict(
    NVIDIA = ["offload", "openacpp", "doconcurrent"],
    AMD = ["offload"],
    INTEL = ["offload", "doconcurrent"],
    GNU = ["serial"]
)

DPCPP_ROOT = os.environ.get("DPCPP")
sycl_compilers = ["dpcpp","AdaptiveCpp"]
sycl_compilers_params = dict(
    {
        "AdaptiveCpp" : ["-DSYCL_IMPL=AdaptiveCpp", "-DCMAKE_CXX_COMPILER=acpp"],
        "dpcpp" : ["-DSYCL_IMPL=dpcpp", f"-DCMAKE_CXX_COMPILER={DPCPP_ROOT}/bin/clang++"]
    }
)
sycl_arch_flags = dict(
    {
        "dpcpp" : dict(
            {
                "NVIDIA" : ["-DDPCPP_WITH_CUDA_BACKEND=ON" ,"-DCUDA_ARCH=sm_70"],
                "AMD" :    ["-DDPCPP_WITH_ROCM_BACKEND=ON","-DROCM_ARCH=gfx908"],
                "INTEL" :  ["-DDPCPP_WITH_LZ_BACKEND=ON", "-DLZ_ARCH=intel_gpu_pvc"]      
            }),
        "AdaptiveCpp" : dict(
            {
                "NVIDIA": [],
                "AMD"   : [],
                "INTEL" : []
            })
    }
)

sycl_kernel_types = ["range", "ndrange"]
sycl_kernel_types_params = dict(
    {
        "range" : "-DBGK_SYCL_ND_RANGE=OFF",
        "ndrange" : "-DBGK_SYCL_ND_RANGE=ON"
    }
)
sycl_alloc_types = ["device", "shared"]
sycl_alloc_types_params = dict(
    {
        "device" : "-DBGK_SYCL_MALLOC_SHARED=OFF",
        "shared" : "-DBGK_SYCL_MALLOC_SHARED=ON"       
    }
)

def benchmark(command_vec, command_cwd):
    perf_res = []
    repeat = True
    repeat_count = 0
    print(f"Warmup run...")
    res = subprocess.run(command_vec, cwd=command_cwd, capture_output=True)
    while(repeat):
        repeat_count += 1
        perf_res.clear()
        for i in range(NUM_REPS):
            print(f"Run {i+1}...")
            res = subprocess.run(command_vec, cwd=command_cwd, capture_output=True, env=os.environ)
            mlups = float(res.stdout.decode().splitlines()[-2].split("Mlups")[1].strip())
            perf_res.append(mlups)
        overall_mean = np.mean(perf_res)
        overall_variance = np.var(perf_res)
        threshold = overall_variance * 0.1
        variances_within_threshold = [abs(x - overall_mean) < threshold for x in perf_res]
        if not all(variances_within_threshold):
            if repeat_count > 3:
                print("Warning: Variance is too high, but already repeated 3 times, skipping...")
                repeat = False
            else:
                print("Warning: Variance is too high, repeating...")
        else:
            repeat = False
        print(f"Values: {perf_res}, Mean: {overall_mean}, Variance: {overall_variance}, Threshold: {threshold}, Variance within threshold: {variances_within_threshold}")
    return np.median(perf_res)


def benchmark_sycl(local_hw):
    executable_name = f"bgk2dSYCL"
    for h in local_hw:
        with open(f"results_{h}_SYCL.csv", "w+") as file:
            file.write("usecase,hw,impl,parallelism,precision,queue,range,alloc_type,mlups\n") # Write header
            for compiler in sycl_compilers:
                for uc in use_case:
                    exp_dir = f"{use_case_dir[uc]}/sycl"
                    for p in precision:
                        for kernel_type in sycl_kernel_types:
                            for alloc_type in sycl_alloc_types:
                                # Configure the app
                                print("###############################################\n###############################################")
                                command = ["cmake", "-S", ".", "-B", "build", 
                                                "-DCMAKE_BUILD_TYPE=Release","-DBGK_SYCL_IN_ORDER_QUEUE=ON", "-DBGK_SYCL_DISABLE_CPU_DEVICE=OFF",
                                                f"-DBGK_USE_CASE={uc}", f"-DBGK_PRECISION={p}",
                                                sycl_kernel_types_params[kernel_type], sycl_alloc_types_params[alloc_type]]
                                command.extend(sycl_compilers_params[compiler])
                                command.extend(sycl_arch_flags[compiler][h])
                                print(command)
                                subprocess.run(command, cwd="../")
                                subprocess.run(command, cwd="../") # Run cmake twice to ensure that the flags are set correctly
                                subprocess.run(["cmake", "--build", "../build", "--", "-j", f"{os.cpu_count()}"])
                                print("###############################################\n###############################################")
                                print(F"Running executable: {executable_name}")
                                median = benchmark([f"{sycl_exec_dir}/{executable_name}"], exp_dir)
                                file.write(f"{uc},{h},sycl,{compiler},{p},in_order,{kernel_type},{alloc_type},{median}\n")
                                file.flush()
                        

def benchmark_fortran(local_hw):
    for h in local_hw:
        with open(f"results_{h}.csv", "w+") as file:
            file.write("use_case,hardware,language,implementation,precision,threads,block_size,mlups\n") # Write header
            for uc in use_case:
                exp_dir = f"{use_case_dir[uc]}/fortran"
                for p in precision:
                    for i in implementation[h]:
                        executable_name = f"bgk2d.{i}.x"
                        print(f"Running {uc} on {h} with {p} precision using {i} implementation")
                        print("Compiling...")
                        subprocess.run(["make", "clean"], cwd="../fortran/SRC", check=True, capture_output=True)
                        print("###############################################\n###############################################")
                        subprocess.run(["make", f"{i}", f"{uc}=1", f"{h}=1", f"{p}=1", "FUSED=1"], cwd="../fortran/SRC")
                        print("###############################################\n###############################################")
                        print(F"Running executable: {executable_name}")
                        median = benchmark([f"{fortran_exec_dir}/{executable_name}"], exp_dir)
                        file.write(f"{uc},{h},fortran,{i},{p},,,{median}\n")
                        file.flush()
                
                    
if __name__ == "__main__":
    for dir in use_case_dir.values():
        os.makedirs(f"{dir}/fortran", exist_ok=True)
        os.makedirs(f"{dir}/sycl", exist_ok=True)
        subprocess.run(["cp", "./bgk.input", f"{dir}/fortran/bgk.input"])
        subprocess.run(["cp", "./bgk.input", f"{dir}/sycl/bgk.input"])
        
    local_hw = ["AMD"]
    #benchmark_fortran(local_hw)
    #set environment variable
    os.environ["OMP_PROC_BIND"] = "true"
    os.environ["ACPP_ADAPTABILITY_LEVEL"] = "2"
    benchmark_sycl(local_hw)
