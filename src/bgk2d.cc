#include <bcond/boundaries.hh>
#include <collision.hh>
#include <diagno.hh>
#include <diagnostic.hh>
#include <file_manager.hh>
#include <finalize.hh>
#include <initialize.hh>
#include <prof_i.hh>
#include <prof_j.hh>
#include <profile.hh>
#include <setup.hh>
#include <storage.hh>
#include <time.hh>
#include <varm.hh>

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

    // #ifdef NOMANAGED
    //     #pragma acc data copyin(a01, a03, a05, a08, a10, a12, a14, a17, a19,
    //     b01, b03, b05, b08, b10, b12, b14, b17, b19, obs)
    // #endif

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

    // #ifdef OFFLOAD
    //     // #pragma omp target data map(
    //     //     tofrom
    //     //     : a01, a03, a05, a08, a10, a12, a14, a17, a19, &!$OMP & b01,
    //     b03, b05, b08, b10, b12, b14, b17, obs)
    // #endif

    // Load data on device
    // bgk_storage.update_host();
    //     std::cout << std::fixed << std::setprecision(15);
    //     std::cout << "Checking iteration " << 0 << "\n";
    //     for (int i = 0; i <= bgk_storage.l; i++){
    //         for (int j = 0; j <= bgk_storage.m; j++){
    //             std::cout << " a01[            " << i << " ][            " << j << " ] =     " << bgk_storage.a01_host(i, j) << "     \n";
    //             std::cout << " a03[            " << i << " ][            " << j << " ] =     " << bgk_storage.a03_host(i, j) << "     \n";
    //             std::cout << " a05[            " << i << " ][            " << j << " ] =     " << bgk_storage.a05_host(i, j) << "     \n";
    //             std::cout << " a08[            " << i << " ][            " << j << " ] =     " << bgk_storage.a08_host(i, j) << "     \n";
    //             std::cout << " a10[            " << i << " ][            " << j << " ] =     " << bgk_storage.a10_host(i, j) << "     \n";
    //             std::cout << " a12[            " << i << " ][            " << j << " ] =     " << bgk_storage.a12_host(i, j) << "     \n";
    //             std::cout << " a14[            " << i << " ][            " << j << " ] =     " << bgk_storage.a14_host(i, j) << "     \n";
    //             std::cout << " a17[            " << i << " ][            " << j << " ] =     " << bgk_storage.a17_host(i, j) << "     \n";
    //             std::cout << " a19[            " << i << " ][            " << j << " ] =     " << bgk_storage.a19_host(i, j) << "     \n";
    //             std::cout << " b01[            " << i << " ][            " << j << " ] =     " << bgk_storage.b01_host(i, j) << "     \n";
    //             std::cout << " b03[            " << i << " ][            " << j << " ] =     " << bgk_storage.b03_host(i, j) << "     \n";
    //             std::cout << " b05[            " << i << " ][            " << j << " ] =     " << bgk_storage.b05_host(i, j) << "     \n";
    //             std::cout << " b08[            " << i << " ][            " << j << " ] =     " << bgk_storage.b08_host(i, j) << "     \n";
    //             std::cout << " b10[            " << i << " ][            " << j << " ] =     " << bgk_storage.b10_host(i, j) << "     \n";
    //             std::cout << " b12[            " << i << " ][            " << j << " ] =     " << bgk_storage.b12_host(i, j) << "     \n";
    //             std::cout << " b14[            " << i << " ][            " << j << " ] =     " << bgk_storage.b14_host(i, j) << "     \n";
    //             std::cout << " b17[            " << i << " ][            " << j << " ] =     " << bgk_storage.b17_host(i, j) << "     \n";
    //             std::cout << " b19[            " << i << " ][            " << j << " ] =     " << bgk_storage.b19_host(i, j) << "     \n";
    //         }
    //     }
    bgk_storage.update_device();

    for(itime = itstart + 1; itime <= itfin; ++itime) {
#ifdef DEBUG_2
        if(bgk_storage.myrank == 0) { std::cout << "DEBUG2: starting time step = " << itime << "\n"; }
#endif

        boundaries(bgk_storage); // boundary conditions
        // propagation();    // TODO propagation step. Ignored for now as we're only
        // interested in  the fused use case
        collision(bgk_storage, itime);

        // get macroscopic values
        diagnostic(bgk_storage, itime, ivtim, icheck, itsave);

        // get timing/profiling values
        if(itime % isignal == 0) {
            if(bgk_storage.myrank == 0) { profile(bgk_storage, itime, itfin, isignal); }
        }
    }

    // some global timings
    bgk::utils::system_clock(timing_s.countE1, timing_s.count_rate, timing_s.count_max);
    bgk::utils::time(timing_s.tcountE1);
    timing_s.time_loop = static_cast<float>(timing_s.countE1 - timing_s.countE0) / timing_s.count_rate;
    timing_s.time_loop1 = timing_s.tcountE1 - timing_s.tcountE0;

    // final diagnostic (for check)
    bgk_storage.update_host();
    diagno(bgk_storage, itime - 1);
    varm(bgk_storage, itime - 1);
    prof_i(bgk_storage, itime - 1, bgk_storage.m / 2);
    prof_j(bgk_storage, itime - 1, bgk_storage.l / 2);

    // No need to deallocate anything

    finalize(bgk_storage, itstart, itfin); // finalize all

    if(bgk_storage.myrank == 0) {
        file_manager.write(0, bgk::utils::get_date());
        std::cout << "That's all folks!!!!" << "\n";
    }

    return 0;
}