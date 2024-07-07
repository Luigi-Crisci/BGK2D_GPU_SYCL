#include "storage.hh"
#include <setup.hh>
#include <file_manager.hh>
#include <time.hh>

namespace bgk{

void setup(int itfin, int ivtim, int isignal, int itsave, int icheck, int itrestart, int init_v){
    
    //TODO: Set myrank = 0 in storage
    //Setup simulation
    auto& file_manager = debug::FileManager::instance();
    file_manager.write(debug::files::time_log, utils::get_date());


}


} // namespace bgk