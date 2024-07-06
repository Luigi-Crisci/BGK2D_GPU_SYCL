#pragma once

#include <chrono>
#include <ctime>
#include <precision.hh>

#ifdef PGI
// Do nothing
#else
// Include check_mem equivalent in C++, if exists
#include <get_mem.hh>
#endif

namespace timing {

// Assuming real_kinds and check_mem functionalities are handled or not needed
// in C++ version

real_kinds::sp time_init, time_init1;
real_kinds::sp time_coll, time_coll1;
real_kinds::sp time_loop, time_loop1;
real_kinds::sp time_move, time_move1;
real_kinds::sp time_obs, time_obs1;
real_kinds::sp time_bc, time_bc1;
real_kinds::sp time_mp, time_mp1;
real_kinds::sp time_dg, time_dg1;
real_kinds::sp time_dev, time_dev1;
real_kinds::sp time_io, time_io1;
real_kinds::sp time_inn_loop, time_inn_loop1;
real_kinds::sp timeZ, timeY, timeX;

int count_rate, count_max;
int count1, count0;
int count2, count3;
int countA0, countB0, countC0, countD0, countE0, countF0, countO0;
int countA1, countB1, countC1, countD1, countE1, countF1, countO1;
int countH0;
int countH1;

real_kinds::mykind tcount1, tcount0;
real_kinds::mykind tcount2, tcount3;

real_kinds::sp tcountA0, tcountB0, tcountC0;
real_kinds::sp tcountD0, tcountE0, tcountF0;
real_kinds::sp tcountG0, tcountH0;
real_kinds::sp tcountO0;
real_kinds::sp tcountA1, tcountB1, tcountC1;
real_kinds::sp tcountD1, tcountE1, tcountF1;
real_kinds::sp tcountG1, tcountH1;
real_kinds::sp tcountO1;
real_kinds::sp old1, old2, old3, old4, old5, old6;
real_kinds::sp tcountZ0, tcountZ1;
real_kinds::sp tcountY0, tcountY1;
real_kinds::sp tcountX0, tcountX1;

real_kinds::dp mem_start, mem_stop; // double precision

void time(real_kinds::sp &t);

} // namespace timing