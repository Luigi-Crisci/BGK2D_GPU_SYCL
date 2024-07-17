# Porting to C++ w/ SYCL

- [ ] Porting to C++
- [ ] Linearize vectors
  - [ ] Mdspans?

# Issues to track
- [x]`write` adds a newline by default, check the porting 
- [ ] 'malloc_shared' do not work on my local machine. Could be a device issue, check it on data-center level gpu

# Ideas
- Flag in `usm_buffer` to avoid duplicated copy host-device or device-host
  - Pointers stored in mdspans views, can be modified
  - Make views member of usm_buffer?
    - Can still be modified, but i can take track of when the views are requested
    - In the current code status, it is possible
  - Annotate host and device pointers?
    - Flag set when modified
    - Introduce overhead at each access
- Check if I'm copying too many times obb

# File porting status
- [x] input.F90
- [ ] col_MC.F90
- [ ] bcond.channel.F90
- [x] precision.F90
- [ ] probe.F90
- [x] init.F90
- [ ] collision.F90
- [ ] bcond.inflow.F90
- [x] vtk_obstacle.F90
- [ ] build.obs.F90
- [x] storage.F90
  - Ported to struct `Storage`.
  - Note: Multi-dimensional vector ported as vector of vectors. `Mdspan` could be a solution
- [ ] draglift.F90
- [ ] diagno.F90
- [ ] build.bcond.F90
- [ ] bgk2d.F90
- [x] varm.F90
- [ ] boundaries.F90
- [x] setup.F90
- [ ] vtk_om.F90
- [x] alloca.F90
- [x] w.obs.F90
- [x] vtk_xy.F90
- [x] save_raw.F90
- [ ] initialize.F90
- [ ] bcond.obs.F90
- [x] hencol.F90
- [x] vtk_xy.binary.F90
- [ ] r.obs.F90
- [x] get_mem_mod.F90
	- Deleted, no C binding needed
- [ ] propagation.F90
- [ ] profile.F90
- [ ] col.F90
- [ ] diagnostic.F90
- [ ] probe.visc.F90
- [ ] bcond.driven.F90
- [x] prof_i.F90
  - `Docouncurrent` statement
- [ ] vtk_visc.binary.F90
- [x] check_case.F90
- [ ] bcond.periodic.F90
- [ ] vtk_om.binary.F90
- [ ] movef.F90
- [x] prof_j.F90
- [x] mod_tools.F90
  - Called `time.hh`
- [ ] outdat.F90
- [ ] restore_raw.F90
- [ ] dealloca.F90
- [ ] finalize.F90
- [ ] save.F90

# Old readme
BGK2D_GPU is a open-source software foe 2D single phase incompressible flows.
The code was orignally written and developed by:

* Giorgio Amati		CINECA, Rome branch	Italy

This is an experimental code. The author accept no responsibility
for the performance of the code or for the correctness of the results.

The code is licensed under MIT Software License 


------------------------------------------------------------------------------
1) Description

This is a 2D CFD Lattice Boltzmann Method code developed to 
	1) Give a (quite) efficient implementation of LBM
 	2) Exploit mulithreading
  	3) assess performance using (different) GPUs 

It is a "downsize" of a more complex code (3D one with mpi+openacc). 
Different options can be activated/decativated via preprocessing flags

You can choose

	* 2 different implementations 
  		* ORIGINAL (default): Classical implementation, with two different subroutine streaming + collision
  		* FUSED Implementation with one single fused subroutine (no streaming subroutine).  It asks for less BW but it uses pointers.

	* 5 different exe
  		* SERIAL 
  		* DO_CONCURRENT
  			* for CPU 
			* for GPU  (default)
  		* OPENMP OFFLOAD 
			* GPU (NVIDIA and AMD)
  		* OPENACC
   			* GPU 

	* 5 different tested compilers
		* GNU (gfortran): default
  		* NVIDIA (nvfortran)
  		* INTEL (ifx)
  		* AMD (flang)
  		* CRAY (ftn)
 

	* 4 different precision 
  		* SINGLE: single precision (default)
  		* DOUBLE: double precision
  		* MIXED1: mixed precision1 (half/single)
  		* MIXED2: mixed precision2 (single/double)

	* 4 different test cases
		* Lid Driven Cavity (default)
		* Poiseuille flow
		* taylor-green vortex
		* Von Karman street

To be noted that not all possible combinations are supported (e.g. GNU doesn't support half precision)

------------------------------------------------------------------------------
2) Directory Structure

.
├── BENCH
├── CI
├── DOC
├── FIX
├── MIT
├── README.md
├── SRC
├── TEST
└── UTIL

where:
	* DOC directory: some documentation, in pdf format
	* SRC directory: all source files + Makefile. The exe will be copied in ../RUN directory (to create)
	* UTIL directory: some utilities like scripts or input file
	* TEST directory: four test case
	  * lid-driven cavity
	  * Taylor-green vortex
	  * flow around an obstacle (cylinder: von Karman streets)
	  * poiseuille flow
	* BENCH directory: script and input file for bechmarking at core, node and GPU level
	* CI: an hand-made script to perform compilation of all possible combination between compiler, precision, implementation, test case and targets (still under development)

------------------------------------------------------------------------------
3) How to compile

the sintax is (in the SRC directory)

make <target> <precision>=1 <compiler>=1 <version>=1 w<testcase>=1 

where 
	* <target>=serial,multicore,offload,openacc,doconcurrent (default)
	* <precision>=mixed1,single (default),mixed2,double
	* <compiler>=INTEL,CRAY,AMD,NVIDIA,GNU (default)
	* <testcase>=POF,TGV,VKS,LDC(default) where
		* POF= poiseuille flow
		* TGV= Taylor Green Vortex
		* LDC= lid-driven cavity
		* VKS= Von karman Street
	

4) Other stuff

Other preprocessing flags are:

* TRICK1 (to reduce the impact of BC on performance --> square box")
* DRAG (activate drag/lift computing, time-consuming)

5) Licence

This SW is under The MIT licence

