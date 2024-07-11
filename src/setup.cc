#include <setup.hh>

#include <alloca.hh>
#include <check_case.hh>
#include <file_manager.hh>
#include <input.hh>
#include <storage.hh>
#include <time.hh>

namespace bgk {

void print_start_info(const storage &bgk_storage) {
    if(bgk_storage.myrank == 0) {
        std::cout << "================================" << std::endl;
        std::cout << "        Uniform LBE...          " << std::endl;

        // Implementation
#ifdef FUSED
        std::cout << " Implementation: FUSED   " << std::endl;
#else
        std::cout << " Implementation: ORIGINAL" << std::endl;
#endif

        // Parallelization
#ifdef DOCONCURRENT
        std::cout << " GPU Parallelization: DO concurrent   " << std::endl;
#elif MULTICORE
        std::cout << " CPU Parallelization: multicore" << std::endl;
#elif OFFLOAD
        std::cout << " GPU Parallelization: OpenMP offload" << std::endl;
#elif OPENACC
        std::cout << " GPU Parallelization: OpenACC" << std::endl;
#else
        std::cout << " Serial" << std::endl;
#endif

        // Test case
#ifdef TGV
        std::cout << " Test Case: Taylor-Green Vortices" << std::endl;
#elif POF
        std::cout << " Test Case: Poiseuille Flow" << std::endl;
#elif VKS
        std::cout << " Test Case: Von Karman Street" << std::endl;
#else
        std::cout << " Test Case: Lid-Driven Cavity" << std::endl;
#endif

        std::cout << "================================" << std::endl;
    }
}

storage setup(int& itfin, int& ivtim, int& isignal, int& itsave, int& icheck, int& itrestart, int& init_v) {
    
    storage bgk_storage;
    bgk_storage.myrank = 0; // Set values for serial version

    // Setup simulation
    auto &file_manager = debug::file_manager::instance();
    file_manager.write(debug::files::time_log, utils::get_date());

    // Print start info
    print_start_info(bgk_storage);
    
    //ORIGINAL: check, read input data and data allocation
    check_case(bgk_storage);

    input(bgk_storage, itfin, ivtim, isignal, itsave, icheck, itrestart, init_v);

    // Allocate memory
    alloc(bgk_storage);

#ifdef DEBUG_1
    if(bgk_storage.myrank == 0) { std::cout << "DEBUG1: Exiting from sub. setup" << std::endl; }
#endif

#ifdef MEM_CHECK
    if(bgk_storage.myrank == 0) {
        auto &timing = utils::timing::instance();
        timing.mem_stop = get_mem();
        std::cout << "MEM_CHECK: after sub. setup mem = " << mem_stop << std::endl;
    }
#endif

return bgk_storage;

}


} // namespace bgk