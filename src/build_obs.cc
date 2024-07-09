#include <build_obs.hh>
#include <iostream>
#include <random>

namespace bgk {

void build_obs(storage &bgk_storage) {
    int i, j;
    int icoord, jcoord;
    int itime;
    real_kinds::mykind d2, R2a, R2b, R;
    real_kinds::mykind hsize;
    real_kinds::mykind soglia, a;
    real_kinds::mykind rand;

    bgk_storage.imin = bgk_storage.l;
    bgk_storage.jmin = bgk_storage.m;
    bgk_storage.imax = 0;
    bgk_storage.jmax = 0;
    bgk_storage.nobs = 0;

    bgk_storage.myrank = 0;
    itime = 0;

    // Creating obstacle
#ifdef SQUARE
    // Creating square
    std::cout << "INFO: creating obstacle (square)" << std::endl;

    // Center of the square
    icoord = 2 * bgk_storage.l / 5;
    jcoord = bgk_storage.m / 2;
    hsize = bgk_storage.radius; // Half size of the square

    std::cout << "INFO: square size   --> " << 2 * hsize << ", " << 2 * hsize / bgk_storage.m << std::endl;
    std::cout << "INFO: Cyl icoord    --> " << icoord << ", " << icoord / bgk_storage.l << std::endl;
    std::cout << "INFO: Cyl jcoord    --> " << jcoord << ", " << jcoord / bgk_storage.m << std::endl;

    for(int j = 1; j <= bgk_storage.m; ++j) {
        if((j >= (jcoord - hsize)) && (j <= (jcoord + hsize))) {
            for(int i = 1; i <= bgk_storage.l; ++i) {
                if((i >= (icoord - hsize)) && (i <= (icoord + hsize))) {
                    bgk_storage.obs(i, j) = 1;
                    bgk_storage.nobs += 1;

                    bgk_storage.imin = std::min(bgk_storage.imin, i);
                    bgk_storage.jmin = std::min(bgk_storage.jmin, j);

                    bgk_storage.imax = std::max(bgk_storage.imax, i);
                    bgk_storage.jmax = std::max(bgk_storage.jmax, j);
                }
            }
        }
    }
#elif defined(POROUS)
    std::cout << "INFO: creating obstacle (porous)" << std::endl;
    std::cout << "INFO: creating obstacle (porous)" << std::endl;

    // Threshold
    soglia = 0.4;
    std::cout << "INFO: threshold (porous) " << soglia << std::endl;
    std::cout << "INFO: threshold (porous) " << soglia << std::endl;

    for(int j = 5; j <= bgk_storage.m - 4; j += 5) {
        for(int i = bgk_storage.l / 4; i <= 3 * bgk_storage.l / 4; i += 5) {
            double a = static_cast<double>(std::rand()) / RAND_MAX;
            if(a > soglia) {
                bgk_storage.obs(i + 1, j) = 1;
                bgk_storage.obs(i, j + 1) = 1;
                bgk_storage.obs(i, j) = 1;
                bgk_storage.obs(i - 1, j) = 1;
                bgk_storage.obs(i, j - 1) = 1;

                bgk_storage.nobs += 5;

                bgk_storage.imin = std::min(bgk_storage.imin, i - 1);
                bgk_storage.jmin = std::min(bgk_storage.jmin, j - 1);

                bgk_storage.imax = std::max(bgk_storage.imax, i + 1);
                bgk_storage.jmax = std::max(bgk_storage.jmax, j + 1);
            }
        }
    }
#else

    std::cout << "INFO: creating obstacle (cylinder)" << std::endl;
    std::cout << "INFO: creating obstacle (cylinder)" << std::endl;

    icoord = 2 * bgk_storage.l / 5;
    jcoord = bgk_storage.m / 2;

    std::cout << "INFO: Cyl radius    --> " << bgk_storage.radius << ", " << bgk_storage.radius / bgk_storage.m
              << std::endl;
    std::cout << "INFO: Cyl icoord    --> " << icoord << ", " << icoord / bgk_storage.l << std::endl;
    std::cout << "INFO: Cyl jcoord    --> " << jcoord << ", " << jcoord / bgk_storage.m << std::endl;

    R = bgk_storage.radius * bgk::storage::uno;
    R2a = (R - 2) * (R - 2); // lower radius
    R2b = (R + 2) * (R + 2); // upper radius

    for(int j = 1; j <= bgk_storage.m; ++j) {
        for(int i = 1; i <= bgk_storage.l; ++i) {
            double d2 = (icoord - i) * (icoord - i) + (jcoord - j) * (jcoord - j);

            if(d2 > R2a && d2 < R2b) {
                bgk_storage.obs(i, j) = 1;
                bgk_storage.nobs += 1;

                bgk_storage.imin = std::min(bgk_storage.imin, i);
                bgk_storage.jmin = std::min(bgk_storage.jmin, j);

                bgk_storage.imax = std::max(bgk_storage.imax, i);
                bgk_storage.jmax = std::max(bgk_storage.jmax, j);
            }
        }
    }
#endif

    std::cout << "INFO: num. obs         --> " << bgk_storage.nobs << std::endl;
    std::cout << "INFO: ratio obs/size   --> " << static_cast<float>(bgk_storage.nobs) / (bgk_storage.l * bgk_storage.m)
              << std::endl;
    std::cout << "INFO: obs (x)             " << bgk_storage.imax << ", " << bgk_storage.imin << std::endl;
    std::cout << "INFO: obs (y)             " << bgk_storage.jmax << ", " << bgk_storage.jmin << std::endl;

#ifdef DEBUG_1
    if(bgk_storage.myrank == 0) { std::cout << "DEBUG1: Exiting from sub. build_obs" << std::endl; }
#endif
}


} // namespace bgk