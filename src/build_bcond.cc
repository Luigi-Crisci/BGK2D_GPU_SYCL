#include <build_bcond.hh>
#include <file_manager.hh>

namespace bgk{

void build_bcond(storage &bgk_storage){
    auto &timing = utils::timing::instance();
    auto& file_manager = debug::file_manager::instance();   
    #ifdef CHANNEL
      bgk_storage.u00 = 0.0;            // boundary condition
      bgk_storage.u0  = bgk_storage.u0;             // volume force
#elif MYVERSION
      bgk_storage.u00 = 0.0;            // boundary condition
      bgk_storage.u0  = bgk_storage.u0;             // volume force
#else
      bgk_storage.u00 = bgk_storage.u0;            // boundary condition
      bgk_storage.u0  = 0.0;            // volume force
#endif

    file_manager.write(16,  "INFO: reference velocities --->", bgk_storage.u0, bgk_storage.u00);
#  ifdef OPENACC
      file_manager.write(38, "#", myrank, ":my GPU  is ------>", mydev, ndev);
#  endif

#ifdef DEBUG_1
      if(bgk_storage.myrank == 0) then
         std::cout << "DEBUG1: Exiting from sub. build_bcond" << "\n";
      endif
#endif
}

} // namespace bgk