#include "jl_memory.h"

#include <Python.h>
#include <ctype.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERROR(msg)                                                             \
    {                                                                          \
        if (PyErr_Occurred() == NULL) PyErr_SetString(PyExc_SystemError, msg); \
        res = -1;                                                              \
        goto END;                                                              \
    }

/**
 * 通过 /proc/pid/status 文件获得程序使用的内存
 * 理论上效率比起每次读取文件来说要更高
 * */
int MemoryUsage(int fd, struct MemoryStatus *ms) {
    int res = 0, i;
    ssize_t len;
    char body[4096];

    if ((len = pread(fd, body, 4096, 0)) == -1) {
        ERROR("pread failure!");
    }

    for (i = 0; i < len; i++) {
        switch (body[i]) {
            case 'V':
                goto V;
            default:
                goto NEXTLINE;
        }
    V:
        i++;
        switch (body[i]) {
            case 'm':
                goto Vm;
            default:
                goto NEXTLINE;
        }
    Vm:
        i++;
        switch (body[i]) {
            case 'R':
                i += 2;
                goto VmRSS;
            case 'D':
                i += 3;
                goto VmData;
            case 'S':
                goto VmS;
            case 'E':
                i += 2;
                goto VmExe;
            case 'L':
                goto VmL;
            default:
                goto NEXTLINE;
        }

    VmRSS:
        i += 2;
        ms->vm_rss = GetNumByVmLine(body + i);
        goto NEXTLINE;

    VmData:
        i += 2;
        ms->vm_data = GetNumByVmLine(body + i);
        goto NEXTLINE;

    VmS:
        i++;
        switch (body[i]) {
            case 't':
                i++;
                goto VmStk;
            case 'i':
                i += 2;
                goto VmSize;
            default:
                goto NEXTLINE;
        }

    VmStk:
        i += 2;
        ms->vm_stk = GetNumByVmLine(body + i);
        goto NEXTLINE;

    VmSize:
        i += 2;
        ms->vm_size = GetNumByVmLine(body + i);
        goto NEXTLINE;

    VmExe:
        i += 2;
        ms->vm_exe = GetNumByVmLine(body + i);
        goto NEXTLINE;

    VmL:
        i++;
        switch (body[i]) {
            case 'i':
                i++;
                goto VmLib;
        }

    VmLib:
        i += 2;
        ms->vm_lib = GetNumByVmLine(body + i);
        goto NEXTLINE;

    NEXTLINE:
        while (body[i] != '\n') {
            i++;
        }
    }

END:
    return res;
}

/**
 * in: '     42964 kB'
 * out: 42964
 * */
int GetNumByVmLine(char body[]) {
    int offset, ans, start;
    offset = ans = 0;
    start = 0;  // FALSE on C
    while (1) {
        if (!start) {
            if (isdigit(body[offset])) {
                start = 1;  // TRUE on C
            } else {
                offset++;
                continue;
            }
        }
        if (start) {
            if (isdigit(body[offset])) {
                ans *= 10;
                ans += (int)(body[offset] - '0');
                offset++;
            } else {
                break;
            }
        }
    }
    return ans;
}
