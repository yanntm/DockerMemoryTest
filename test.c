
#if defined(__APPLE__)
#include <sys/sysctl.h>
#include <sys/types.h>
#endif

#include <dlfcn.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

size_t RTmemSize();
size_t RTpageSize();
int RTnumCPUs();

#if defined(__APPLE__)

size_t RTmemSize(){
    int mib[4];
    int64_t physical_memory;
    size_t len = sizeof(int64_t);
    mib[0] = CTL_HW;
    mib[1] = HW_MEMSIZE;
    len = sizeof(int64_t);
    sysctl(mib, 2, &physical_memory, &len, NULL, 0);
    return physical_memory;
}

int RTcacheLineSize(){
    int mib[4];
    int line_size;
    size_t len = sizeof(int);
    mib[0] = CTL_HW;
    mib[1] = HW_CACHELINE;
    len = sizeof(int);
    sysctl(mib, 2, &line_size, &len, NULL, 0);
    return line_size;
}

#else

size_t RTmemSize(){
    long res=sysconf(_SC_PHYS_PAGES);
    size_t pagesz=RTpageSize();
    return pagesz*((size_t)res);
}

#if defined(__linux__)

int RTcacheLineSize(){
    long res=sysconf(_SC_LEVEL1_DCACHE_LINESIZE);
    return (int)res;
}

#else

int RTcacheLineSize(){
    Abort("generic implementation for RTcacheLineSize needed");
}

#endif

#endif

int RTnumCPUs(){
    long res=sysconf(_SC_NPROCESSORS_ONLN);
    return (size_t)res;
}

size_t RTpageSize(){
    long res=sysconf(_SC_PAGESIZE);
    return (size_t)res;
}

int main () {
  printf("Reported page size : %zu\n",RTpageSize());
  printf("Reported memory available : %zu GB\n",RTmemSize() >> 30 );
  return 0;
}

