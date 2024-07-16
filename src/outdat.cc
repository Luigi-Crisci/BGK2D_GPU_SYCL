#include <file_manager.hh>
#include <outdat.hh>

namespace bgk {
void outdat(storage &bgk_storage, const int itfin, const int itstart, const int ivtim, const int isignal,
    const int itsave, const int icheck) {
    auto &file_manager = debug::file_manager::instance();
    file_manager.write(16, " ", "\n");
    file_manager.write(16, "*********** size of the lattice **************", "\n");
    file_manager.write(16, "l (width x)  =", bgk_storage.l, "\n");
    file_manager.write(16, "m (width y)  =", bgk_storage.m, "\n");
    file_manager.write(16, "*********** fluid data **********************", "\n");
    file_manager.write(16, "viscosity    =", bgk_storage.svisc, "\n");
    file_manager.write(16, "u0           =", bgk_storage.u0, "\n");
    file_manager.write(16, "u00          =", bgk_storage.u00, "\n");
    file_manager.write(16, "omega        =", bgk_storage.omega1, "\n");
    file_manager.write(16, "tau          =", 1.0 / bgk_storage.omega1, "\n");
    file_manager.write(16, "Reynolds     =",
        0.5 * bgk_storage.u0 * bgk_storage.l / bgk_storage.svisc
            + 0.5 * bgk_storage.u00 * bgk_storage.l / bgk_storage.svisc,
        "\n");
    file_manager.write(16, "forcing1     =", bgk_storage.fgrad, "\n");
    file_manager.write(16, "forcing2     =", bgk_storage.u00 / (6.0), "\n");
    file_manager.write(16, "u_inflow     =", bgk_storage.u_inflow, "\n");
    file_manager.write(16, "LES: cteS    =", bgk_storage.cteS, "\n");
    file_manager.write(16, "*********** run data ************************", "\n");
    file_manager.write(16, "itfin        =", itfin, "\n");
    file_manager.write(16, "itstart      =", itstart, "\n");
    file_manager.write(16, "ivtim        =", ivtim, "\n");
    file_manager.write(16, "isignal      =", isignal, "\n");
    file_manager.write(16, "itsave       =", itsave, "\n");
    file_manager.write(16, "icheck       =", icheck, "\n");
    file_manager.write(16, "flag1        =", bgk_storage.flag1, "\n");
    file_manager.write(16, "flag2        =", bgk_storage.flag2, "\n");
    file_manager.write(16, "flag3        =", bgk_storage.flag3, "\n");
    file_manager.write(16, "ipad         =", bgk_storage.ipad, "\n");
    file_manager.write(16, "jpad         =", bgk_storage.jpad, "\n");
    file_manager.write(16, "radius       =", bgk_storage.radius, "\n");
    file_manager.write(16, "LES: cteS    =", bgk_storage.cteS, "\n");
    file_manager.write(16, "************** Further check ****************", "\n");
    file_manager.write(16, "zero         =", storage::zero, storage::zero_qp, "\n");
    file_manager.write(16, "uno          =", storage::uno, storage::uno_qp, "\n");
    file_manager.write(16, "tre          =", storage::tre, storage::tre_qp, "\n");
    file_manager.write(16, "rf           =", storage::rf, storage::rf_qp, "\n");
    file_manager.write(16, "qf           =", storage::qf, storage::qf_qp, "\n");
    file_manager.write(16, "p0           =", storage::p0, storage::p0_qp, "\n");
    file_manager.write(16, "p1           =", storage::p1, storage::p1_qp, "\n");
    file_manager.write(16, "p2           =", storage::p2, storage::p2_qp, "\n");
    file_manager.write(16, "*********************************************", "\n");
    file_manager.write(16, " ", "\n");

    std::cout << "*********** size of the lattice **************"
              << "\n";
    std::cout << "l (width x)  = " << bgk_storage.l << "\n";
    std::cout << "m (width y)  = " << bgk_storage.m << "\n";
    std::cout << "****************fluid data*******************"
              << "\n";
    std::cout << "viscosity    = " << bgk_storage.svisc << "\n";
    std::cout << "u0           = " << bgk_storage.u0 << "\n";
    std::cout << "u00          = " << bgk_storage.u00 << "\n";
    std::cout << "omega        = " << bgk_storage.omega1 << "\n";
    std::cout << "tau          = " << 1.0 / bgk_storage.omega1 << "\n";
    std::cout << "Reynolds     = "
              << 0.5 * bgk_storage.u0 * bgk_storage.l / bgk_storage.svisc
            + 0.5 * bgk_storage.u00 * bgk_storage.l / bgk_storage.svisc
              << "\n";
    std::cout << "forcing1     = " << bgk_storage.fgrad << "\n";
    std::cout << "forcing2     = " << bgk_storage.u00 / 6.0 << "\n";
    std::cout << "u_inflow     = " << bgk_storage.u_inflow << "\n";
    std::cout << "LES: cteS    = " << bgk_storage.cteS << "\n";
    std::cout << "**************** run data********************"
              << "\n";
    std::cout << "itfin        = " << itfin << "\n";
    std::cout << "itstart      = " << itstart << "\n";
    std::cout << "ivtim        = " << ivtim << "\n";
    std::cout << "isignal      = " << isignal << "\n";
    std::cout << "itsave       = " << itsave << "\n";
    std::cout << "icheck       = " << icheck << "\n";
    std::cout << "flag1        = " << bgk_storage.flag1 << "\n";
    std::cout << "flag2        = " << bgk_storage.flag2 << "\n";
    std::cout << "flag3        = " << bgk_storage.flag3 << "\n";
    std::cout << "ipad         = " << bgk_storage.ipad << "\n";
    std::cout << "jpad         = " << bgk_storage.jpad << "\n";
    std::cout << "radius       = " << bgk_storage.radius << "\n";
    std::cout << "*********************************************"
              << "\n";
    std::flush(std::cout);

#ifdef DEBUG_1
    if(bgk_storage.myrank == 0) { std::cout << "DEBUG1: Exiting from sub. outdat\n"; }
#endif
}
} // namespace bgk