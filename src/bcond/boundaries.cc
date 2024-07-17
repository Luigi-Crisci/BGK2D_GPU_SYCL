#include <bcond/boundaries.hh>
#include <bcond/bcond_driven.hh>
#include <bcond/bcond_obs.hh>
#include <bcond/bcond_periodic.hh>


#include <fmt/core.h>

namespace bgk {
void boundaries(storage &bgk_storage) {
    // Different boundary conditions (selected at compile-time)
#ifdef PERIODIC
    // Periodic boundary condition
    bcond_periodic();
#elif CHANNEL
    // Channel boundary condition
    bcond_channel();
#elif INFLOW
    // Inflow boundary condition
    bcond_inflow();
#elif MYVERSION
    // Periodic boundary condition (my version)
    bcond_periodic();
#else
    // Lid-driven cavity boundary condition (default)
    bcond_driven(bgk_storage);
#endif

#ifdef OBSTACLE
    bcond_obs();
#endif

#ifdef DEBUG_2
    if(bgk_storage.myrank == 0) { fmt::print("DEBUG2: Exiting from function boundaries\n"); }
#endif
}
} // namespace bgk