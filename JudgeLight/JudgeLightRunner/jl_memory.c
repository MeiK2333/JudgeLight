#include "jl_memory.h"

#include <Python.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 * 通过 /proc/pid/status 文件获得程序使用的内存
 * */
int GetMemoryUsage(pid_t pid, struct MemoryStatus *ms) {
    char status_file[PATH_MAX], buf[PATH_MAX];
    FILE *fr;
    int res = 0;

    sprintf(status_file, "/proc/%d/status", pid);
    if ((fr = fopen(status_file, "r")) == NULL) {
        PyErr_SetString(PyExc_SystemError, "fopen failure!");
        res = -1;
        goto END;
    }

    /** 依次解析需要的数据 */
    while (fgets(buf, PATH_MAX - 1, fr) != NULL) {
        if (buf[0] != 'V') {
            continue;
        }
        // if (strncmp(buf, "VmSize:", 7) == 0) {
        //     sscanf(buf + 7, "%d", &ms->vm_size);
        // } else
        if (strncmp(buf, "VmRSS:", 6) == 0) {
            sscanf(buf + 6, "%d", &ms->vm_rss);
            break;
        }
        //  else if (strncmp(buf, "VmData:", 7) == 0) {
        //     sscanf(buf + 7, "%d", &ms->vm_data);
        // } else if (strncmp(buf, "VmStk:", 6) == 0) {
        //     sscanf(buf + 6, "%d", &ms->vm_stk);
        // } else if (strncmp(buf, "VmExe:", 6) == 0) {
        //     sscanf(buf + 6, "%d", &ms->vm_exe);
        // } else if (strncmp(buf, "VmLib:", 6) == 0) {
        //     sscanf(buf + 6, "%d", &ms->vm_lib);
        // }
    }

END:
    if (fr) {
        fclose(fr);
    }
    return res;
}
