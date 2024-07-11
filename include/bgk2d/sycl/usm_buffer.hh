#include <sycl/sycl.hpp>

namespace bgk2d {

template<typename T, sycl::usm::alloc kind>
struct usm_buffer {
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    
    usm_buffer(const std::shared_ptr<sycl::queue> &q) : m_q(q) {}

    void allocate(size_t size) {
        if constexpr(kind == sycl::usm::alloc::shared) {
            m_host_ptr = m_device_ptr = sycl::malloc_shared<T>(size, *m_q);
        } else if constexpr (kind == sycl::usm::alloc::device) {
            m_device_ptr = sycl::malloc_device<T>(size, *m_q);
            m_host_ptr = new value_type[size];
        }
    }

    ~usm_buffer() {
        if constexpr(kind == sycl::usm::alloc::shared) {
            sycl::free(m_host_ptr, *m_q);
        } else if(kind == sycl::usm::alloc::device) {
            sycl::free(m_device_ptr, *m_q);
            delete[] m_host_ptr;
        }
    }

    // Wait for all operations to finish and copy the data
    // Every kernel uses the same elements, so we're not loosing parallelism
    void update_host() {
        m_q->wait();
        if constexpr (kind == sycl::usm::alloc::device) {
            m_q->copy(m_device_ptr, m_host_ptr, m_size).wait();
        }
    }

    // Update host without waiting for operations to finish
    // This is useful when we need to update multiple buffers at the same time
    void unsafe_update_host() {
        if constexpr (kind == sycl::usm::alloc::device) {
            m_q->copy(m_device_ptr, m_host_ptr, m_size).wait();
        }
    }

    // Wait for all operations to finish and copy the data
    // Every kernel uses the same elements, so we're not loosing parallelism
    void update_device() {
        m_q->wait();
        if constexpr (kind == sycl::usm::alloc::device) {
            m_q->copy(m_host_ptr, m_device_ptr, m_size).wait();
        }
    }

     // Update device without waiting for operations to finish
    // This is useful when we need to update multiple buffers at the same time
    void unsafe_update_device() {
        if constexpr (kind == sycl::usm::alloc::device) {
            m_q->copy(m_host_ptr, m_device_ptr, m_size).wait();
        }
    }
    
    friend void swap(const usm_buffer<T, kind> &first, const usm_buffer<T, kind> &other) {
        using std::swap;
        swap(first.m_host_ptr, other.m_host_ptr);
        swap(first.m_device_ptr, other.m_device_ptr);
        swap(first.m_size, other.m_size);
        // This is not really necessary, there is just one queue in the system...
        swap(first.m_q, other.m_q);
    }

    T *m_host_ptr;
    T *m_device_ptr;
    size_t m_size;
    std::shared_ptr<sycl::queue> m_q;
};

} // namespace bgk2d