#include <get_mem.hh>
#include <limits>
#include <precision.hh>
#include <storage.hh>

namespace bgk{

void storage::init(){

 auto init_lambda = [](auto type_var, std::unique_ptr<typename decltype(type_var)::type[]> &arr, auto &view, const auto row_num, const auto col_num, const auto init_value) {
        // const auto size = (l + 2 + ipad * (m + 2 + jpad));
        using T = typename decltype(type_var)::type;
        const auto size = row_num * col_num;
        arr = std::make_unique<T[]>(size);
        std::fill(arr.get(), arr.get() + size, init_value);
        view = std::remove_reference_t<decltype(view)>(
            arr.get(), Kokkos::dextents<std::size_t, 2>{row_num, col_num});
    };

    init_lambda(std::type_identity<int>{}, _obs, obs, l, m, 0);

    init_lambda(std::type_identity<real_kinds::mystorage>{},_a01, a01, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{},_a03, a03, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{},_a05, a05, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{},_a08, a08, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{},_a10, a10, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{},_a12, a12, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{},_a14, a14, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{},_a17, a17, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{},_a19, a19, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());

    init_lambda(std::type_identity<real_kinds::mystorage>{}, _b01, b01, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{}, _b03, b03, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{}, _b05, b05, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{}, _b08, b08, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{}, _b10, b10, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{}, _b12, b12, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{}, _b14, b14, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{}, _b17, b17, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(std::type_identity<real_kinds::mystorage>{}, _b19, b19, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());

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
}

} // namespace bgk