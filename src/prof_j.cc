#include <prof_j.hh>
#include <file_manager.hh>

#include <sycl/sycl.hpp>

namespace bgk{

    static constexpr char format_1005[sizeof("# t={:7d}\n")] = "# t={:7d}\n";
    static constexpr char format_1002[sizeof("{:14.6e} {:14.6e} {:14.6e} {:14.6e}\n")] = "{:14.6e} {:14.6e} {:14.6e} {:14.6e}\n";

    void prof_j(storage &bgk_storage, const int itime, const int icoord){
        // Variables
    std::vector<real_kinds::mykind> u(bgk_storage.m);
    std::vector<real_kinds::mykind> v(bgk_storage.m);
    std::vector<real_kinds::mykind> den(bgk_storage.m);
    real_kinds::mykind cte1;

    // Set cte1 based on NOSHIFT
#ifdef NOSHIFT
    cte1 = storage::zero;
#else
    cte1 = storage::uno;
#endif

    sycl::queue q{sycl::cpu_selector{}};
    // for (int j = 0; j < bgk_storage.m; ++j) {
    //     den[j] = (bgk_storage.a01(icoord,j) + bgk_storage.a03(icoord,j) + bgk_storage.a05(icoord,j) +
    //               bgk_storage.a08(icoord,j) + bgk_storage.a10(icoord,j) + bgk_storage.a12(icoord,j) +
    //               bgk_storage.a14(icoord,j) + bgk_storage.a17(icoord,j) + bgk_storage.a19(icoord,j)) + cte1;
    // }

    // Density calculation
    q.parallel_for(sycl::range<1>{static_cast<size_t>(bgk_storage.m)}, [
        den = den.data(),
        icoord,
        cte1,
        a01 = bgk_storage.a01,
        a03 = bgk_storage.a03,
        a05 = bgk_storage.a05,
        a08 = bgk_storage.a08,
        a10 = bgk_storage.a10,
        a12 = bgk_storage.a12,
        a14 = bgk_storage.a14,
        a17 = bgk_storage.a17,
        a19 = bgk_storage.a19
    ](sycl::item<1> idx) {
        const auto j = idx.get_linear_id();
        den[j] = (a01(icoord,j) + a03(icoord,j) + a05(icoord,j) + a08(icoord,j) + a10(icoord,j) + a12(icoord,j)
                     + a14(icoord,j) + a17(icoord,j) + a19(icoord,j))
            + cte1;
    }).wait_and_throw();



    // Streamwise velocity calculation
    // for (int j = 0; j < bgk_storage.m; ++j) {
    //     u[j] = (bgk_storage.a01(icoord,j) - bgk_storage.a10(icoord,j) +
    //             bgk_storage.a03(icoord,j) - bgk_storage.a12(icoord,j) +
    //             bgk_storage.a05(icoord,j) - bgk_storage.a14(icoord,j)) / den[j];
    // }
    q.parallel_for(sycl::range<1>{static_cast<size_t>(bgk_storage.m)}, [
        u = u.data(),
        icoord,
        den = den.data(),
        a01 = bgk_storage.a01,
        a03 = bgk_storage.a03,
        a05 = bgk_storage.a05,
        a10 = bgk_storage.a10,
        a12 = bgk_storage.a12,
        a14 = bgk_storage.a14
    ](sycl::item<1> idx) {
        const auto j = idx.get_linear_id();
        u[j] = (a01(icoord,j) - a10(icoord,j) + a03(icoord,j) - a12(icoord,j) + a05(icoord,j) - a14(icoord,j)) / den[j];
    }).wait_and_throw();
    

    // Normal-to-wall velocity calculation
    // for (int j = 0; j < bgk_storage.m; ++j) {
    //     v[j] = (bgk_storage.a03(icoord,j) - bgk_storage.a01(icoord,j) +
    //             bgk_storage.a08(icoord,j) - bgk_storage.a17(icoord,j) +
    //             bgk_storage.a12(icoord,j) - bgk_storage.a10(icoord,j)) / den[j];
    // }
    q.parallel_for(sycl::range<1>{static_cast<size_t>(bgk_storage.m)}, [
        v = v.data(),
        icoord,
        den = den.data(),
        a01 = bgk_storage.a01,
        a03 = bgk_storage.a03,
        a08 = bgk_storage.a08,
        a10 = bgk_storage.a10,
        a12 = bgk_storage.a12,
        a17 = bgk_storage.a17
    ](sycl::item<1> idx) {
        const auto j = idx.get_linear_id();
        v[j] = (a03(icoord,j) - a01(icoord,j) + a08(icoord,j) - a17(icoord,j) + a12(icoord,j) - a10(icoord,j)) / den[j];
    }).wait_and_throw();

    // Write itime
    auto& file_manager = debug::file_manager::instance();
    file_manager.write_format<format_1005>(64, itime);
    // Write data
    for (int j = 0; j < bgk_storage.m; ++j) {
        file_manager.write_format<format_1002>(64, (j - 0.5), u[j], v[j], den[j]);
    }

    // Write empty lines
    file_manager.write(64, "\n\n");

    // Debug output
#ifdef DEBUG_1
    if (bgk_storage.myrank == 0) {
        std::cout << "DEBUG1: Exiting from sub. prof_j" << std::endl;
    }
#endif
    }

}