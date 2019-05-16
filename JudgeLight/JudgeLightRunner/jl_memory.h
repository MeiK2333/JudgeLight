#ifndef JL_MEMORY_H
#define JL_MEMORY_H

#include <sys/types.h>

/**
 * 程序运行占用的内存资源
 * 因为没有统一标准要求使用哪个指标来表示内存占用，此处使用 vm_rss 来表示
 * */
struct MemoryStatus {
    int vm_size;
    int vm_rss;
    int vm_data;
    int vm_stk;
    int vm_exe;
    int vm_lib;
};

int MemoryUsage(int, struct MemoryStatus *);

int GetNumByVmLine(char *);

#endif
