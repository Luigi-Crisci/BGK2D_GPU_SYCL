#include <initialize.hh>
#include <file_manager.hh>
#include <setup.hh>
#include <storage.hh>
#include <time.hh>
#include <varm.hh>
#include <prof_i.hh>
#include <prof_j.hh>

int main() {
    int itfin, itstart, ivtim;
    int itime, itsave, icheck, itrestart, init_v;
    int isignal;
    
    auto &file_manager = bgk::debug::file_manager::instance();
    // Setup simulation
    auto &timing_s = bgk::utils::timing::instance();
    bgk::utils::system_clock(timing_s.countH0, timing_s.count_rate, timing_s.count_max);
    bgk::utils::time(timing_s.tcountH0);

    // setup simulation
    auto bgk_storage = bgk::setup(itfin, ivtim, isignal, itsave, icheck, itrestart, init_v);

    // Initialize the flow
    bgk::initialize(bgk_storage, itrestart, init_v, itfin, itstart, ivtim, isignal, itsave, icheck);

#ifdef NOMANAGED
    !$acc data copyin(a01, a03, a05, a08, a10, a12, a14, a17, a19, b01, b03, b05, b08, b10, b12, b14, b17, b19, obs)
#endif

        bgk::utils::system_clock(timing_s.countH1, timing_s.count_rate, timing_s.count_max);
    bgk::utils::time(timing_s.tcountH1);
    timing_s.time_init = static_cast<float>(timing_s.countH1 - timing_s.countH0) / timing_s.count_rate;
    timing_s.time_init1 = (timing_s.tcountH1 - timing_s.tcountH0);

    bgk::utils::system_clock(timing_s.countE0, timing_s.count_rate, timing_s.count_max);
    bgk::utils::time(timing_s.tcountE0);

    bgk::utils::system_clock(timing_s.countD0, timing_s.count_rate, timing_s.count_max);
    bgk::utils::time(timing_s.tcountD0);

    // Main loop starts here

    if(bgk_storage.myrank == 0) { file_manager.write(0, bgk::utils::get_date()); }

#ifdef OFFLOAD
    !$OMP target data map(
        tofrom
        : a01, a03, a05, a08, a10, a12, a14, a17, a19, &!$OMP & b01, b03, b05, b08, b10, b12, b14, b17, obs)
#endif

        for(int itime = itstart + 1; itime <= itfin; ++itime) {
#ifdef DEBUG_2
        if(myrank == 0) { std::cout << "DEBUG2: starting time step = " << itime << std::endl; }
#endif

        // boundaries();     // boundary conditions
        // propagation();    // TODO propagation step
        // collision(itime); // TODO collision step

        // get macroscopic values
        // diagnostic(itime, ivtim, icheck, itsave); TODO

        // get timing/profiling values
        if(itime % isignal == 0) {
            if(bgk_storage.myrank == 0) { 
                // profile(itime, itfin, isignal); TODO
                }
        }
    }

    // some global timings
    bgk::utils::system_clock(timing_s.countE1, timing_s.count_rate, timing_s.count_max);
    bgk::utils::time(timing_s.tcountE1);
    timing_s.time_loop = static_cast<float>(timing_s.countE1 - timing_s.countE0) / timing_s.count_rate;
    timing_s.time_loop1 = timing_s.tcountE1 - timing_s.tcountE0;

    // final diagnostic (for check)
    // diagno(itime - 1); //TODO
    varm(bgk_storage, itime - 1);
    prof_i(bgk_storage, itime - 1, bgk_storage.m / 2);
    prof_j(bgk_storage, itime - 1, bgk_storage.l / 2);

#ifdef OFFLOAD
#pragma omp end target data
#endif

#ifdef NOMANAGED
#pragma acc end data
#endif

    // finalize(itstart, itfin); // finalize all //TODO

    if(bgk_storage.myrank == 0) {
        file_manager.write(0, bgk::utils::get_date());
        std::cout << "That's all folks!!!!" << std::endl;
    }

    return 0;
}