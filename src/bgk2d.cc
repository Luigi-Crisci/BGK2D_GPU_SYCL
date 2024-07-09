#include <storage.hh>
#include <time.hh>
#include <setup.hh>

int main() {

    int itfin, itstart, ivtim;
    int itime, itsave, icheck, itrestart, init_v;
    int isignal;

    //Setup simulation
    auto& timing_s = bgk::utils::timing::instance();
    bgk::utils::system_clock(timing_s.countH0, timing_s.count_rate, timing_s.count_max);
    bgk::utils::time(timing_s.tcountH0);

    //setup simulation
    bgk::setup(itfin, ivtim, isignal, itsave, icheck, itrestart, init_v);

    

        

    return `0;
}