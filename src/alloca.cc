#include <iostream>

#include <alloca.hh>
#include <get_mem.hh>

namespace bgk {

void alloc(storage &bgk_storage) {
    bgk_storage.init();

    long mem_start = 0;
    long mem_stop = 0;

#ifndef PGI
    mem_start = get_mem();
#endif

#ifdef DEBUG_1
    if(bgk_storage.myrank == 0) { std::cout << "DEBUG1: Exiting from sub. alloca" << std::endl; }
#endif

#ifdef MEM_CHECK
    if(bgk_storage.myrank == 0) {
        mem_stop = get_mem();
        std::cout << "MEM_CHECK: after sub. alloca mem = " << mem_stop << std::endl;
    }
#endif
}

} // namespace bgk