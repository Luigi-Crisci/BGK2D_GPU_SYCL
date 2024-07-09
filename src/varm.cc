#include <varm.hh>

namespace bgk {




    void varm(storage &bgk_storage, const int itime){
    
    auto u = std::make_unique<real_kinds::mykind[]>(bgk_storage.m); //Mean velocity profiles
    auto w = std::make_unique<real_kinds::mykind[]>(bgk_storage.m); 
    auto den = std::make_unique<real_kinds::mykind[]>(bgk_storage.m); //mean density profile
    real_kinds::mykind rho, rhoinv, rvol;
    real_kinds::mykind cte1;

#ifdef NOSHIFT
    cte1 = storage::zero;
#else
    cte1 = storage::uno;
#endif

    for (int j = 0; j < bgk_storage.m; ++j) {
        u[j]   = storage::zero;
        w[j]   = storage::zero;
        den[j] = storage::zero;
    }

    for (int j = 0; j < bgk_storage.m; ++j) {
        for (int i = 0; i < bgk_storage.l; ++i) {
            rho = (bgk_storage.a01(i,j) + bgk_storage.a03(i,j) + bgk_storage.a05(i,j) + bgk_storage.a08(i,j) +
                   bgk_storage.a10(i,j) + bgk_storage.a12(i,j) + bgk_storage.a14(i,j) + bgk_storage.a17(i,j) + bgk_storage.a19(i,j)) + cte1;

            rhoinv = storage::uno / rho;

            den[j] += rho;

            u[j] += (bgk_storage.a01(i,j) + bgk_storage.a03(i,j) + bgk_storage.a05(i,j) - bgk_storage.a10(i,j) - bgk_storage.a12(i,j) - bgk_storage.a14(i,j)) * rhoinv;
            w[j] += (bgk_storage.a03(i,j) + bgk_storage.a08(i,j) + bgk_storage.a12(i,j) - bgk_storage.a01(i,j) - bgk_storage.a10(i,j) - bgk_storage.a17(i,j)) * rhoinv;
        }
    }

    rvol = 1.0 / static_cast<double>(bgk_storage.l);

    std::cout << "# t=" << std::setw(7) << itime << std::endl;
    for (int j = 0; j < m; ++j) {
        std::cout << std::setw(5) << j + static_cast<int>(offset[2]) << " "
                  << std::scientific << std::setprecision(6)
                  << u[j] * rvol << " " << w[j] * rvol << " " << den[j] * rvol << std::endl;
    }
    std::cout << std::endl << std::endl;

#ifdef DEBUG_1
    if (myrank == 0) {
        std::cerr << "DEBUG1: Exiting from sub. varm" << std::endl;
    }
#endif

#ifdef MEM_CHECK
    if (myrank == 0) {
        // Assuming get_mem() is a function that returns memory usage
        double mem_stop = get_mem();
        std::cerr << "MEM_CHECK: after sub. varm mem = " << mem_stop << std::endl;
    }
#endif
    }
}
