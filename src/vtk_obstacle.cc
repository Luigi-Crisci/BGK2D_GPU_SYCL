#include <fstream>
#include <iomanip>
#include <iostream>
#include <storage.hh>
#include <string>

namespace bgk {

void vtk_obs(storage &bgk_storage) {
    int itime = 0;
    std::string file_nameVTK = "tec_ob.xxxxxxxx.vtk";

#ifdef NO_OUTPUT
    if(bgk_storage.myrank == 0) { std::cout << "INFO: no output mode enabled, no dump at all" << std::endl; }
#endif

    std::cout << "task " << bgk_storage.myrank << " has " << bgk_storage.nobs << " obstacles" << std::endl;

    std::ostringstream oss;
    oss << std::setw(8) << std::setfill('0') << itime;
    file_nameVTK.replace(7, 8, oss.str());

    std::cout << "INFO: obstacle vtk dump " << file_nameVTK << std::endl;

    std::ofstream file(file_nameVTK);
    if(!file.is_open()) {
        std::cerr << "Error opening file: " << file_nameVTK << std::endl;
        return;
    }

    file << "# vtk DataFile Version 2.0" << std::endl;
    file << "Campo" << std::endl;
    file << "ASCII" << std::endl;
    file << "DATASET RECTILINEAR_GRID" << std::endl;
    file << "DIMENSIONS " << bgk_storage.l << " " << bgk_storage.m << " 1" << std::endl;

    file << "X_COORDINATES " << bgk_storage.l << " double" << std::endl;
    for(int i = 1; i <= bgk_storage.l; ++i) { file << i << std::endl; }

    file << "Y_COORDINATES " << bgk_storage.m << " double" << std::endl;
    for(int j = 1; j <= bgk_storage.m; ++j) { file << j << std::endl; }

    file << "Z_COORDINATES 1 double" << std::endl;
    file << 1 << std::endl;

    file << "POINT_DATA " << bgk_storage.l * bgk_storage.m << std::endl;
    file << "SCALARS obs float" << std::endl;
    file << "LOOKUP_TABLE default" << std::endl;
    for(int j = 1; j <= bgk_storage.m; ++j) {
        for(int i = 1; i <= bgk_storage.l; ++i) { file << bgk_storage.obs_host(i - 1, j - 1) * 1.0 << std::endl; }
    }

    std::cout << "INFO: obs vtk dump done" << std::endl;
    file.close();

#ifdef DEBUG_1
    if(bgk_storage.myrank == 0) { std::cout << "DEBUG1: Exiting from sub. vtk_obs" << std::endl; }
#endif
}

} // namespace bgk