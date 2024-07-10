#include <storage.hh>
#include "initialize.hh"
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
    auto bgk_storage = bgk::setup(itfin, ivtim, isignal, itsave, icheck, itrestart, init_v);

    //Initialize the flow
    bgk::initialize(bgk_storage, itrestart,init_v,itfin,itstart,ivtim,isignal,itsave,icheck);
    

        

    return 0;
}