#include <get_mem.hh>
#include <limits>
#include <precision.hh>
#include <storage.hh>

namespace bgk {

storage::storage()
    : q(std::make_shared<sycl::queue>(sycl::default_selector{})), _a01(q), _a03(q), _a05(q), _a08(q), _a10(q), _a12(q),
      _a14(q), _a17(q), _a19(q), _b01(q), _b03(q), _b05(q), _b08(q), _b10(q), _b12(q), _b14(q), _b17(q), _b19(q),
      _obs(q) {}


void storage::init() {
    auto init_lambda = [](auto &usm_buffer, auto &view_host, auto &view_device, const auto row_num, const auto col_num,
                           const auto init_value) {
        // const auto size = (l + 2 + ipad * (m + 2 + jpad));
        const auto size = row_num * col_num;
        usm_buffer.allocate(size);
        std::fill(usm_buffer.m_host_ptr, usm_buffer.m_host_ptr + size, init_value);
        view_host = std::remove_reference_t<decltype(view_host)>(
            usm_buffer.m_host_ptr, Kokkos::dextents<std::size_t, 2>{row_num, col_num});
        view_device = std::remove_reference_t<decltype(view_device)>(
            usm_buffer.m_device_ptr, Kokkos::dextents<std::size_t, 2>{row_num, col_num});
    };

    init_lambda(_obs, obs_host, obs_device, l, m, 0);

    init_lambda(
        _a01, a01_host, a01_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _a03, a03_host, a03_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _a05, a05_host, a05_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _a08, a08_host, a08_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _a10, a10_host, a10_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _a12, a12_host, a12_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _a14, a14_host, a14_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _a17, a17_host, a17_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _a19, a19_host, a19_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());

    init_lambda(
        _b01, b01_host, b01_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _b03, b03_host, b03_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _b05, b05_host, b05_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _b08, b08_host, b08_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _b10, b10_host, b10_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _b12, b12_host, b12_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _b14, b14_host, b14_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _b17, b17_host, b17_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());
    init_lambda(
        _b19, b19_host, b19_device, (l + 2 + ipad), (m + 2 + jpad), std::numeric_limits<real_kinds::mystorage>::max());

#ifdef FUSED
    // c01 = std::nullptr;
    // c03 = std::nullptr;
    // c05 = std::nullptr;
    // c08 = std::nullptr;
    // c10 = std::nullptr;
    // c12 = std::nullptr;
    // c14 = std::nullptr;
    // c17 = std::nullptr;
    // c19 = std::nullptr;
#endif
}

void storage::update_host()
{
    _a01.unsafe_update_host();
    _a03.unsafe_update_host();
    _a05.unsafe_update_host();
    _a08.unsafe_update_host();
    _a10.unsafe_update_host();
    _a12.unsafe_update_host();
    _a14.unsafe_update_host();
    _a17.unsafe_update_host();
    _a19.unsafe_update_host();

    _b01.unsafe_update_host();
    _b03.unsafe_update_host();
    _b05.unsafe_update_host();
    _b08.unsafe_update_host();
    _b10.unsafe_update_host();
    _b12.unsafe_update_host();
    _b14.unsafe_update_host();
    _b17.unsafe_update_host();
    _b19.unsafe_update_host();

    _obs.unsafe_update_host();
}

void storage::update_device()
{
    _a01.unsafe_update_device();
    _a03.unsafe_update_device();
    _a05.unsafe_update_device();
    _a08.unsafe_update_device();
    _a10.unsafe_update_device();
    _a12.unsafe_update_device();
    _a14.unsafe_update_device();
    _a17.unsafe_update_device();
    _a19.unsafe_update_device();

    _b01.unsafe_update_device();
    _b03.unsafe_update_device();
    _b05.unsafe_update_device();
    _b08.unsafe_update_device();
    _b10.unsafe_update_device();
    _b12.unsafe_update_device();
    _b14.unsafe_update_device();
    _b17.unsafe_update_device();
    _b19.unsafe_update_device();

    _obs.unsafe_update_device();
}

} // namespace bgk