#include <prof_i.hh>
#include <file_manager.hh>

namespace bgk {

static inline constexpr char format_1002[sizeof("{:14.6e} {:14.6e} {:14.6e} {:14.6e}\n")] = "{:14.6e} {:14.6e} {:14.6e} {:14.6e}\n";
static inline constexpr char format_1005[sizeof("# t={:7d}\n")] = "# t={:7d}\n";

void prof_i(storage &bgk_storage, const int itime, const int jcoord) {
    std::vector<real_kinds::mykind> u(bgk_storage.l);
    std::vector<real_kinds::mykind> v(bgk_storage.l);
    std::vector<real_kinds::mykind> den(bgk_storage.l);
    real_kinds::mykind cte1;

// Set cte1 based on NOSHIFT
#ifdef NOSHIFT
    cte1 = storage::zero;
#else
    cte1 = storage::uno;
#endif

    //TODO:THOSE WERE DOCONCURRENT. PORT THIS TO SYCL
    // Density calculation
    for(int i = 0; i < bgk_storage.l; ++i) {
        den[i] = (bgk_storage.a01(i,jcoord) + bgk_storage.a03(i,jcoord) + bgk_storage.a05(i,jcoord) + bgk_storage.a08(i,jcoord) + bgk_storage.a10(i,jcoord) + bgk_storage.a12(i,jcoord)
                     + bgk_storage.a14(i,jcoord) + bgk_storage.a17(i,jcoord) + bgk_storage.a19(i,jcoord))
            + cte1;
    }

    // Streamwise velocity calculation
    for(int i = 0; i < bgk_storage.l; ++i) {
        u[i] = (bgk_storage.a01(i,jcoord) + bgk_storage.a03(i,jcoord) + bgk_storage.a05(i,jcoord) - bgk_storage.a10(i,jcoord) - bgk_storage.a12(i,jcoord) - bgk_storage.a14(i,jcoord))
            / den[i];
    }

    // Normal-to-wall velocity calculation
    for(int i = 0; i < bgk_storage.l; ++i) {
        v[i] = (bgk_storage.a03(i,jcoord) + bgk_storage.a08(i,jcoord) + bgk_storage.a12(i,jcoord) - bgk_storage.a01(i,jcoord) - bgk_storage.a10(i,jcoord) - bgk_storage.a17(i,jcoord))
            / den[i];
    }

    auto& file_manager = debug::file_manager::instance();
    file_manager.write_format<format_1005>(61, itime);

    // Write data
    for(int i = 0; i < bgk_storage.l; ++i) {
        file_manager.write_format<format_1002>(61, u[i], v[i], den[i]);
    }

    // Write empty lines
    file_manager.write(61, "\n\n");

// Debug output
#ifdef DEBUG_1
    if(bgk_storage.myrank == 0) { std::cout << "DEBUG1: Exiting from sub. prof_i" << std::endl; }
#endif

}
}