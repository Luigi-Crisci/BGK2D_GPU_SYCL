#include <varm.hh>
#include <file_manager.hh>
namespace bgk {

static constexpr auto format_1003 = "{:5d} {:14.6e} {:14.6e} {:14.6e} ";
static constexpr auto format_1005 = "# t={:7d}";

void varm(storage &bgk_storage, const int itime) {
    auto u = std::make_unique<real_kinds::mykind[]>(bgk_storage.m); // Mean velocity profiles
    auto w = std::make_unique<real_kinds::mykind[]>(bgk_storage.m);
    auto den = std::make_unique<real_kinds::mykind[]>(bgk_storage.m); // mean density profile
    real_kinds::mykind rho, rhoinv, rvol;
    real_kinds::mykind cte1;

#ifdef NOSHIFT
    cte1 = storage::zero;
#else
    cte1 = storage::uno;
#endif

    for(int j = 0; j < bgk_storage.m; ++j) {
        u[j] = storage::zero;
        w[j] = storage::zero;
        den[j] = storage::zero;
    }

    for(int j = 0; j < bgk_storage.m; ++j) {
        for(int i = 0; i < bgk_storage.l; ++i) {
            rho = (bgk_storage.a01(i, j) + bgk_storage.a03(i, j) + bgk_storage.a05(i, j) + bgk_storage.a08(i, j)
                      + bgk_storage.a10(i, j) + bgk_storage.a12(i, j) + bgk_storage.a14(i, j) + bgk_storage.a17(i, j)
                      + bgk_storage.a19(i, j))
                + cte1;

            rhoinv = storage::uno / rho;

            den[j] += rho;

            u[j] += (bgk_storage.a01(i, j) + bgk_storage.a03(i, j) + bgk_storage.a05(i, j) - bgk_storage.a10(i, j)
                        - bgk_storage.a12(i, j) - bgk_storage.a14(i, j))
                * rhoinv;
            w[j] += (bgk_storage.a03(i, j) + bgk_storage.a08(i, j) + bgk_storage.a12(i, j) - bgk_storage.a01(i, j)
                        - bgk_storage.a10(i, j) - bgk_storage.a17(i, j))
                * rhoinv;
        }
    }

    rvol = 1.0 / static_cast<float>(bgk_storage.l);

    auto& file_manager = debug::file_manager::instance();
    file_manager.write_format(62, format_1005, itime);
    for(int j = 0; j < bgk_storage.m; ++j) {
        file_manager.write_format(62, format_1003,  j+bgk_storage.offset[1], 
        u[j]*rvol,
        w[j]*rvol,
        den[j]*rvol);
    }
    file_manager.write(62, "\n\n"); //TODO: CHeck if this works

#ifdef DEBUG_1
    if(bgk_storage.myrank == 0) { std::cerr << "DEBUG1: Exiting from sub. varm" << std::endl; }
#endif
#ifdef MEM_CHECK
    if(bgk_storage.myrank == 0) {
        // Assuming get_mem() is a function that returns memory usage
        auto& timing = utils::timing::instance();
        timing.mem_stop = get_mem();
        std::cerr << "MEM_CHECK: after sub. varm mem = " << timing.mem_stop << std::endl;
    }
#endif
}
} // namespace bgk
