#ifndef __GET_MEM_H__
#define __GET_MEM_H__

#include <sys/resource.h>
#include <sys/time.h>
#include <stdio.h>
/* 
               struct timeval ru_utime; // user CPU time used 
               struct timeval ru_stime; // system CPU time used 
               long   ru_maxrss;        // maximum resident set size
               long   ru_ixrss;         // integral shared memory size 
               long   ru_idrss;         // integral unshared data size 
               long   ru_isrss;         // integral unshared stack size 
               long   ru_minflt;        // page reclaims (soft page faults)
               long   ru_majflt;        // page faults (hard page faults) 
               long   ru_nswap;         // swaps 
               long   ru_inblock;       // block input operations 
               long   ru_oublock;       // block output operations 
               long   ru_msgsnd;        // IPC messages sent 
               long   ru_msgrcv;        // IPC messages received 
               long   ru_nsignals;      // signals received 
               long   ru_nvcsw;         // voluntary context switches 
               long   ru_nivcsw;        // involuntary context switches 
*/

double get_mem();

#endif // __GET_MEM_H__