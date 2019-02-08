#include "jl_memory.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * 通过 /proc/pid/status 文件获得程序使用的内存
 * */
struct MemoryStatus get_memory_usage(pid_t pid) {
    char status_file[PATH_MAX], buf[PATH_MAX];
    FILE *fr;
    struct MemoryStatus ms;

    sprintf(status_file, "/proc/%d/status", pid);
    if ((fr = fopen(status_file, "r")) == NULL) {
        /** TODO: 触发异常 */
        goto END;
    }

    while (fgets(buf, PATH_MAX - 1, fr) != NULL) {
        if (strncmp(buf, "VmSize:", 7) == 0) {
            sscanf(buf + 7, "%d", &ms.vm_size);
        } else if (strncmp(buf, "VmRSS:", 6) == 0) {
            sscanf(buf + 6, "%d", &ms.vm_rss);
        } else if (strncmp(buf, "VmData:", 7) == 0) {
            sscanf(buf + 7, "%d", &ms.vm_data);
        } else if (strncmp(buf, "VmStk:", 6) == 0) {
            sscanf(buf + 6, "%d", &ms.vm_stk);
        } else if (strncmp(buf, "VmExe:", 6) == 0) {
            sscanf(buf + 6, "%d", &ms.vm_exe);
        } else if (strncmp(buf, "VmLib:", 6) == 0) {
            sscanf(buf + 6, "%d", &ms.vm_lib);
        }
    }

END:
    if (fr) {
        fclose(fr);
    }
    return ms;
}
