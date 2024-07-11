#include <w_obs.hh>

#include <fstream>
#include <iostream>

namespace bgk {

void w_obs(storage &bgk_storage) {
    // Open the file for writing
    std::ofstream file("obstacle.dat");
    if(!file.is_open()) {
        std::cerr << "Error opening file: obstacle.dat" << std::endl;
        return;
    }

    // Write the obstacle data to the file
    for(int j = 1; j <= bgk_storage.m; ++j) {
        for(int i = 1; i <= bgk_storage.l; ++i) {
            file << i << " " << j << " " << bgk_storage.obs_host(i - 1, j - 1) << std::endl;
        }
    }

    // Close the file
    file.close();

#ifdef DEBUG_1
    if(bgk_storage.myrank == 0) { std::cout << "DEBUG1: Exiting from sub. w_obs" << std::endl; }
#endif
}

} // namespace bgk