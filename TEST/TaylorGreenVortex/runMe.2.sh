echo "TG for different precision"
echo "------------------------------"
echo "run #1 of 4: single precision "
sleep 5
./run.mixed1.serial.fused.nvidia.x
echo "------------------------------"
echo "run #2 of 4: single precision "
sleep 5
./run.single.serial.fused.nvidia.x
echo "------------------------------"
echo "run #3 of 4: mixed (single/double) precision "
sleep 5 
./run.mixed2.serial.fused.nvidia.x
echo "------------------------------"
echo "run #4 of 4: double precision "
sleep 5 
./run.double.serial.fused.nvidia.x
echo "------------------------------"
echo "to plot: use gnuplot and load 'plot.small.gnu'"

