#include <collision.hh>
#include <time.hh>

namespace bgk {
void collision(storage &bgk_storage, const int itime) {
  auto &timing = utils::timing::instance();
  utils::system_clock(timing.countC0, timing.count_rate, timing.count_max);
  utils::time(timing.tcountC0);

#ifdef FUSED
  call col_MC(itime)
#else
  call col(itime) // TODO: unimplemented
#endif
}
} // namespace bgk