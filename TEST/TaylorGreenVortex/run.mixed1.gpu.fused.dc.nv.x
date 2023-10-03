#!/bin/tcsh
#
setenv DIR RUN_MIXED1_GPU_FUSED_DC_NV
setenv EXE bgk2d.doconcurrent.x
#
echo "-------------------------------"
echo "starting test TG vortices      "
echo " ---> nvfortran                "
echo " ---> mixed precision (hp/sp)  "
echo " ---> fused                    "
echo " ---> doconcurrent             "
echo " ---> "$EXE
echo " ---> "$DIR
echo "-------------------------------"
#
rm -rf $DIR
mkdir $DIR
cd $DIR

# step 1: compiling
echo "step 1: compiling"
cd ../../../SRC
make clean
make MIXED1=1 FUSED=1 TGV=1 
if ($?) then
   echo "compiling fails..."
   exit 1
else
   cd -
   cp ../../../RUN/$EXE  .
   cp ../bgk.1024.input bgk.input
   echo "compiling  ended succesfully..."
endif

# step 2: running test
echo "step 2: running test"
./$EXE >& out.log
if (-e "bgk.perf") then
   echo "run ended succesfully..."
else
   echo "running test fails..."
   exit 1
endif

