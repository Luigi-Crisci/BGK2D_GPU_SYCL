#include <bcond/bcond_obs.hh>
#include <time.hh>
#include <file_manager.hh>

namespace bgk {

    void bcond_obs(storage& bgk_storage){
        auto& timing = utils::timing::instance();
        utils::system_clock(timing.countO0, timing.count_rate, timing.count_max);
        utils::time(timing.tcountO0);
        auto& q = *bgk_storage.dev_q;

        const auto range_j = bgk_storage.jmax - bgk_storage.jmin + 1;
        const auto range_i = bgk_storage.imax - bgk_storage.imin + 1;
        q.parallel_for(sycl::range(range_j, range_i), [
            obs = bgk_storage.obs_device,
            a01 = bgk_storage.a01_device,
            a03 = bgk_storage.a03_device,
            a05 = bgk_storage.a05_device,
            a08 = bgk_storage.a08_device,
            a10 = bgk_storage.a10_device,
            a12 = bgk_storage.a12_device,
            a14 = bgk_storage.a14_device,
            a17 = bgk_storage.a17_device,
            jmin = bgk_storage.jmin,
            imin = bgk_storage.imin
        ](sycl::item<2> id){
            const auto j = id.get_id(0) + jmin;
            const auto i = id.get_id(1) + imin;
             if(obs(i,j)==1){
                a01(i,j) = a12(i+1,j-1);
                a03(i,j) = a10(i+1,j+1);
                a05(i,j) = a14(i+1,j  );
                a08(i,j) = a17(i  ,j+1);
                a10(i,j) = a03(i-1,j-1);
                a12(i,j) = a01(i-1,j+1);
                a14(i,j) = a05(i-1,j  );
                a17(i,j) = a08(i  ,j-1);
             }
        }).wait_and_throw();
    
        // Stop timing
        utils::time(timing.tcountO1);
        utils::system_clock(timing.countO1, timing.count_rate, timing.count_max);
        timing.time_obs = timing.time_obs + static_cast<float>((timing.tcountO1 - timing.tcountO0) / timing.count_rate);
        timing.time_obs1 = timing.time_obs1 + (timing.tcountO1 - timing.tcountO0);

        #ifdef DEBUG_2
            if (bgk_storage.myrank == 0){
                fmt::print("DEBUG2: Exiting from sub. bcond_obs {} {} {} {}\n", bgk_storage.imin, bgk_storage.imax, bgk_storage.jmin, bgk_storage.jmax);
            }
        #endif
    }

}