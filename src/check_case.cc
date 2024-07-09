#include <iostream>
#include <check_case.hh>
#include <utils.hh>
#include <file_manager.hh>

namespace bgk {

void git_info(){
    //TODO: implement git_info
}

void check_case(const storage &bgk_storage) {

    auto& file_manager = debug::file_manager::instance();
    auto& file16 = file_manager.get_file_stream(16);

    if(bgk_storage.u0 > 0 && bgk_storage.u_inflow > 0) {
        std::cout << "WARNING: inflow and volume force at the same time " << bgk_storage.myrank << " " << bgk_storage.u0 << " " << bgk_storage.u_inflow
                  << std::endl;
        file16 << "WARNING: inflow and volume force at the same time " << bgk_storage.myrank << " " << bgk_storage.u0 << " " << bgk_storage.u_inflow
               << std::endl;
    }

    // Info section
    git_info();

    // Boundary conditions
#ifdef PERIODIC
    std::cout << "INFO: The test case has periodic bc (-DPERIODIC)" << std::endl;
    file16 << "INFO: The test case has periodic bc (-DPERIODIC)" << std::endl;
#elif CHANNEL
    std::cout << "INFO: The test case has channel bc (-DCHANNEL)" << std::endl;
    file16 << "INFO: The test case has channel bc (-DCHANNEL)" << std::endl;
#elif INFLOW
    std::cout << "INFO: The test case has inflow bc (-DINFLOW)" << std::endl;
    file16 << "INFO: The test case has inflow bc (-DINFLOW)" << std::endl;
#elif MYVERSION
    std::cout << "INFO: The test case has ad-hoc bc (-DMYVERSION)" << std::endl;
    file16 << "INFO: The test case has ad-hoc bc (-DMYVERSION)" << std::endl;
#else
    std::cout << "INFO: The test case is driven cavity (default)" << std::endl;
    file16 << "INFO: The test case is driven cavity (default)" << std::endl;
#endif

#ifdef OBSTACLE
    std::cout << "INFO: The test case has an obstacle in the flow" << std::endl;
    file16 << "INFO: The test case has an obstacle in the flow" << std::endl;
#endif

#ifdef NOSHIFT
    std::cout << "INFO: using NOSHIFT preprocessing flag" << std::endl;
    file16 << "INFO: using NOSHIFT preprocessing flag" << std::endl;
#endif

#ifdef DRAG
    std::cout << "INFO: using DRAG preprocessing flag" << std::endl;
    file16 << "INFO: using DRAG preprocessing flag" << std::endl;
    std::cout << "WARNING: the box for drag/lift is hard-coded, please check!" << std::endl;
    file16 << "WARNING: the box for drag/lift is hard-coded, please check!" << std::endl;
#endif

#ifdef QUAD_P
    file16 << "INFO: using quad precision (storage)" << std::endl;
#elif DOUBLE_P
    file16 << "INFO: using double precision (storage)" << std::endl;
#elif HALF_P
    file16 << "INFO: using half precision (storage)" << std::endl;
    std::cout << "WARNING: pure half precision has some problem" << std::endl;
#else
    file16 << "INFO: using single precision (storage)" << std::endl;
#endif

#ifdef MIXEDPRECISION
    file16 << "INFO: using mixed precision" << std::endl;
#else
    file16 << "INFO: using the same precision for computation" << std::endl;
#endif

    file16 << "INFO: mykind= "    << utils::type_name<real_kinds::mykind>() << " range= " << std::numeric_limits<real_kinds::mykind>::max() << std::endl;
    file16 << "INFO: mykind= "    << utils::type_name<real_kinds::mykind>() << " huge= " << std::numeric_limits<real_kinds::mykind>::max() << std::endl;
    file16 << "INFO: mykind= "    << utils::type_name<real_kinds::mykind>() << " epsilon= " << std::numeric_limits<real_kinds::mykind>::epsilon() << std::endl;
    file16 << "INFO: mystorage= " << utils::type_name<real_kinds::mystorage>() << " range= " << std::numeric_limits<real_kinds::mystorage>::max() << std::endl;
    file16 << "INFO: mystorage= " << utils::type_name<real_kinds::mystorage>() << " huge= " << std::numeric_limits<real_kinds::mystorage>::max() << std::endl;
    file16 << "INFO: mystorage= " << utils::type_name<real_kinds::mystorage>() << " epsilon= " << std::numeric_limits<real_kinds::mystorage>::epsilon() << std::endl;

    file16 << "INFO: using RAW I/O" << std::endl;

#ifdef NO_BINARY
    std::cout << "INFO: vtk output in ASCII (debug mode)" << std::endl;
    file16 << "INFO: vtk output in ASCII (debug mode)" << std::endl;
#endif

#ifdef LES
    std::cout << "WARNING: LES (Smagorinsky) enabled UNDER DEVELOPMENT" << std::endl;
    file16 << "WARNING: LES (Smagorinsky) enabled UNDER DEVELOPMENT" << std::endl;
#endif

#ifdef PGI
    file16 << "quad precision not supported" << std::endl;
#endif

#ifdef DOCONCURRENT
    std::cout << "INFO: do concurrent version (GPU)" << std::endl;
    file16 << "INFO: do concurrent version (GPU)" << std::endl;
#elif MULTICORE
    std::cout << "INFO: multicore parallelization (CPU)" << std::endl;
    file16 << "INFO: multicore parallelization (CPU)" << std::endl;
#elif OFFLOAD
    std::cout << "INFO: offload version (GPU)" << std::endl;
    file16 << "INFO: offload version (GPU)" << std::endl;
#elif OPENACC
    std::cout << "INFO: openacc version (GPU)" << std::endl;
    file16 << "INFO: openacc version (GPU)" << std::endl;
#else
    std::cout << "INFO: serial version (CPU)" << std::endl;
    file16 << "INFO: serial version (CPU)" << std::endl;
#endif

#ifdef FUSED
    file16 << "INFO: using Fused version" << std::endl;
#else
    file16 << "INFO: using Move & collide (Original) version" << std::endl;
#endif

#ifdef DEBUG_3
    if(bgk_storage.myrank == 0) { std::cout << "INFO: DEBUG3 mode enabled" << std::endl; }
#endif

#ifdef DEBUG_2
    if(bgk_storage.myrank == 0) { std::cout << "INFO: DEBUG2 mode enabled" << std::endl; }
#endif

#ifdef DEBUG_1
    if(bgk_storage.myrank == 0) {
        std::cout << "INFO: DEBUG1 mode enabled" << std::endl;
        std::cout << "DEBUG1: Exiting from sub. check_case" << std::endl;
    }
#endif

#ifdef TRICK1
    std::cout << "WARNING: square box mandatory (TRICK1)!" << std::endl;
    file16 << "WARNING: square box mandatory (TRICK1)!" << std::endl;
    int l = 1, m = 1; // Example values
    if(l != m) {
        std::cout << "ERROR: box not squared (TRICK1)!" << std::endl;
        file16 << "ERROR: box not squared (TRICK1)!" << std::endl;
        return 1;
    }
#endif

#ifdef TRICK2
    std::cout << "WARNING: forced offload num_threads(TRICK2)!" << std::endl;
    file16 << "WARNING: forced offload num_threads(TRICK2)!" << std::endl;
#endif
}

} // namespace bgk