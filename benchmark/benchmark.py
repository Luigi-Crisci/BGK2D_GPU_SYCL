import sys
import subprocess
import os
import numpy as np

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
NUM_REPS = 5

def benchmark(command_vec, command_cwd):
    perf_res = []
    repeat = True
    repeat_count = 0
    while(repeat):
        repeat_count += 1
        perf_res.clear()
        for i in range(NUM_REPS):
            print(f"Run {i+1}...")
            res = subprocess.run(command_vec, cwd=command_cwd, check=True, capture_output=True)
            mlups = float(res.stdout.decode().splitlines()[-2].split("Mlups")[1].strip())
            perf_res.append(mlups)
        if np.std(perf_res) > 5:
            if repeat_count > 3:
                print("Warning: Variance is too high, but already repeated 3 times, skipping...")
                repeat = False
            else:
                print("Warning: Variance is too high, repeating...")
        else:
            repeat = False
        print(f"Values: {perf_res}, Std: {np.std(perf_res)}")
    return np.median(perf_res)

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
                        subprocess.run(["make", f"{i}", f"{uc}=1", f"{h}=1", f"{p}=1", "FUSED=1"], cwd="../fortran/SRC", capture_output=True)
                        median = benchmark([f"{fortran_exec_dir}/{executable_name}"], exp_dir)
                        file.write(f"{uc},{h},fortran,{i},{p},,,{median}\n")
                        file.flush()
                
                    
if __name__ == "__main__":
    for dir in use_case_dir.values():
        os.makedirs(f"{dir}/fortran", exist_ok=True)
        os.makedirs(f"{dir}/sycl", exist_ok=True)
        subprocess.run(["cp", "./bgk.input", f"{dir}/fortran/bgk.input"])
        subprocess.run(["cp", "./bgk.input", f"{dir}/sycl/bgk.input"])
        
    local_hw = ["GNU"]
    benchmark_fortran(local_hw)