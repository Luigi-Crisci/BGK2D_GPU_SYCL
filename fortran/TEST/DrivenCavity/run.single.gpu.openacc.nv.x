#!/bin/tcsh
#
setenv DIR RUN_SINGLE_GPU_FUSED_OPENACC_NV
setenv EXE bgk2d.openacc.x
#
echo "---------------------------"
echo "starting test driven cavity"
echo " ---> nvfortran            "
echo " ---> single precision     "
echo " ---> fused                "
echo " ---> GPU                  "
echo " ---> openacc              "
echo " ---> "$EXE
echo " ---> "$DIR
echo "---------------------------"
#
rm -r $DIR
mkdir $DIR
cd $DIR

# step 1: compiling
echo "step 1: compiling"
cd ../../../SRC
make clean
make openacc FIX="-DFUSED"
if ($?) then
   echo "compiling fails..."
   exit 1
else
   cd -
   cp ../../../RUN/$EXE  .
   cp ../../../UTIL/bgk.1024*  bgk.input
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
