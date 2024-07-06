#include <get_mem.hh>
#include <limits>
#include <precision.hh>
#include <storage.hh>

// Define constructor of the storage class
bgk::storage::storage(std::size_t l, std::size_t ipad, std::size_t m, std::size_t jpad) {
    auto init_lambda = [l, ipad, m, jpad](std::unique_ptr<real_kinds::mystorage[]> &arr, auto &view) {
        const auto size = (l + 2 + ipad * (m + 2 + jpad));
        arr = std::make_unique<real_kinds::mystorage[]>(size);
        std::fill(arr.get(), arr.get() + size, std::numeric_limits<real_kinds::mystorage>::max());
        view = std::remove_reference_t<decltype(view)>(
            arr.get(), Kokkos::dextents<std::size_t, 2>{l + 2 + ipad, m + 2 + jpad});
    };

    init_lambda(_a01, a01);
    init_lambda(_a03, a03);
    init_lambda(_a05, a05);
    init_lambda(_a08, a08);
    init_lambda(_a10, a10);
    init_lambda(_a12, a12);
    init_lambda(_a14, a14);
    init_lambda(_a17, a17);
    init_lambda(_a19, a19);

    init_lambda(_b01, b01);
    init_lambda(_b03, b03);
    init_lambda(_b05, b05);
    init_lambda(_b08, b08);
    init_lambda(_b10, b10);
    init_lambda(_b12, b12);
    init_lambda(_b14, b14);
    init_lambda(_b17, b17);
    init_lambda(_b19, b19);

#ifdef FUSED
    c01 = std::nullptr;
    c03 = std::nullptr;
    c05 = std::nullptr;
    c08 = std::nullptr;
    c10 = std::nullptr;
    c12 = std::nullptr;
    c14 = std::nullptr;
    c17 = std::nullptr;
    c19 = std::nullptr;
#endif

    long mem_start = 0;
    long mem_stop = 0;

#ifndef PGI
    mem_start = get_mem();
#endif

#ifdef DEBUG_1
    if(myrank == 0) { std::cout << "DEBUG1: Exiting from sub. alloca" << std::endl; }
#endif

#ifdef MEM_CHECK
    if(myrank == 0) {
        mem_stop = get_mem();
        std::cout << "MEM_CHECK: after sub. alloca mem = " << mem_stop << std::endl;
    }
#endif
}