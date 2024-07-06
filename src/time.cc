#include <time.hh>

void time(real_kinds::sp &t) {
  auto now = std::chrono::system_clock::now();
  auto now_c = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm = *std::localtime(&now_c);

  t = 3600.f * now_tm.tm_hour + 60.f * now_tm.tm_min + now_tm.tm_sec +
      std::chrono::duration_cast<std::chrono::milliseconds>(
          now.time_since_epoch())
              .count() %
          1000 / 1000.f;
}