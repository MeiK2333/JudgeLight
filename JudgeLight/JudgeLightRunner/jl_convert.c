#include "jl_convert.h"
#include "jl_rules.h"

#include <stdlib.h>

#define ERROR(msg)                                                             \
    {                                                                          \
        if (PyErr_Occurred() == NULL) PyErr_SetString(PyExc_SystemError, msg); \
        res = -1;                                                              \
        goto END;                                                              \
    }

/**
 * 解析 Python 参数为 C 结构体
 * */
int ParsePythonArgs(PyObject *args, struct RunnerConfig *rconfig) {
    int res = 0;
    PyObject *pconfig, *py_obj, *py_bytes, *py_item;

    if (!PyArg_ParseTuple(args, "O", &pconfig)) {
        ERROR("PyArg_ParseTuple failure!");
    }
    if (!PyDict_Check(pconfig)) {
        ERROR("argument must be a dict");
    }

    /** 解析数字类型的限制 */
    if ((py_obj = PyDict_GetItemString(pconfig, "time_limit")) == NULL) {
        rconfig->time_limit = UNLIMITED;
    } else {
        rconfig->time_limit = PyLong_AsLong(py_obj);
    }

    if ((py_obj = PyDict_GetItemString(pconfig, "memory_limit")) == NULL) {
        rconfig->memory_limit = UNLIMITED;
    } else {
        rconfig->memory_limit = PyLong_AsLong(py_obj);
    }

    if ((py_obj = PyDict_GetItemString(pconfig, "real_time_limit")) == NULL) {
        rconfig->real_time_limit = UNLIMITED;
    } else {
        rconfig->real_time_limit = PyLong_AsLong(py_obj);
    }

    if ((py_obj = PyDict_GetItemString(pconfig, "output_size_limit")) == NULL) {
        rconfig->output_size_limit = UNLIMITED;
    } else {
        rconfig->output_size_limit = PyLong_AsLong(py_obj);
    }

    if ((py_obj = PyDict_GetItemString(pconfig, "stack_limit")) == NULL) {
        rconfig->stack_limit = UNLIMITED;
    } else {
        rconfig->stack_limit = PyLong_AsLong(py_obj);
    }

    if ((py_obj = PyDict_GetItemString(pconfig, "gid")) == NULL) {
        rconfig->gid = UNLIMITED;
    } else {
        rconfig->gid = PyLong_AsLong(py_obj);
    }

    if ((py_obj = PyDict_GetItemString(pconfig, "uid")) == NULL) {
        rconfig->uid = UNLIMITED;
    } else {
        rconfig->uid = PyLong_AsLong(py_obj);
    }

    /** 解析字符串类型的配置 */
    /**
     * Python C API 会用引用计数维护内存清理
     * PyBytes_AsString 返回的是指向 PyObject 内部的指针
     * 不要 free 这种指针，否则会导致**灾难性**后果
     * */
    if ((py_obj = PyDict_GetItemString(pconfig, "exec_file_path")) == NULL) {
        ERROR("exec_file_path is required!")
    } else {
        if ((py_bytes = PyUnicode_AsUTF8String(py_obj)) == NULL) {
            ERROR("PyUnicode_AsUTF8String [exec_file_path] failure!");
        }
        rconfig->exec_file_path = PyBytes_AsString(py_bytes);
    }

    if ((py_obj = PyDict_GetItemString(pconfig, "input_file_path")) == NULL) {
        rconfig->input_file_path = NULL;
    } else {
        if ((py_bytes = PyUnicode_AsUTF8String(py_obj)) == NULL) {
            ERROR("PyUnicode_AsUTF8String [input_file_path] failure!");
        }
        rconfig->input_file_path = PyBytes_AsString(py_bytes);
    }

    if ((py_obj = PyDict_GetItemString(pconfig, "output_file_path")) == NULL) {
        rconfig->output_file_path = NULL;
    } else {
        if ((py_bytes = PyUnicode_AsUTF8String(py_obj)) == NULL) {
            ERROR("PyUnicode_AsUTF8String [output_file_path] failure!");
        }
        rconfig->output_file_path = PyBytes_AsString(py_bytes);
    }

    if ((py_obj = PyDict_GetItemString(pconfig, "error_file_path")) == NULL) {
        rconfig->error_file_path = NULL;
    } else {
        if ((py_bytes = PyUnicode_AsUTF8String(py_obj)) == NULL) {
            ERROR("PyUnicode_AsUTF8String [error_file_path] failure!");
        }
        rconfig->error_file_path = PyBytes_AsString(py_bytes);
    }

    if ((py_obj = PyDict_GetItemString(pconfig, "allow_system_calls_rule")) ==
        NULL) {
        rconfig->allow_system_calls_rule = NULL;
    } else {
        if ((py_bytes = PyUnicode_AsUTF8String(py_obj)) == NULL) {
            ERROR("PyUnicode_AsUTF8String [allow_system_calls_ruls] failure!");
        }
        rconfig->allow_system_calls_rule = PyBytes_AsString(py_bytes);
    }

    /** 解析字符串列表 */
    rconfig->exec_args = NULL;
    if ((py_obj = PyDict_GetItemString(pconfig, "exec_args")) == NULL) {
        ERROR("exec_args is required!");
    } else {
        if (!PyList_Check(py_obj)) {
            ERROR("exec_args must be a list!");
        }
        int len = PyList_GET_SIZE(py_obj);
        /** 这种自行创建的指针数组需要手动释放，如果没释放将会导致内存泄露 */
        rconfig->exec_args = (char **)malloc(sizeof(char *) * (len + 1));
        for (int i = 0; i < len; i++) {
            if ((py_item = PyList_GetItem(py_obj, i)) == NULL) {
                ERROR("PyList_GetItem [exec_args] failure!");
            }
            if ((py_bytes = PyUnicode_AsUTF8String(py_item)) == NULL) {
                ERROR("PyUnicode_AsUTF8String [exec_args] failure!");
            }
            rconfig->exec_args[i] = PyBytes_AsString(py_bytes);
        }
        rconfig->exec_args[len] = NULL;
    }

    rconfig->envs = NULL;
    if ((py_obj = PyDict_GetItemString(pconfig, "envs")) == NULL) {
        ERROR("envs is required!");
    } else {
        if (!PyList_Check(py_obj)) {
            ERROR("envs must be a list!");
        }
        int len = PyList_GET_SIZE(py_obj);
        rconfig->envs = (char **)malloc(sizeof(char *) * (len + 1));
        for (int i = 0; i < len; i++) {
            if ((py_item = PyList_GetItem(py_obj, i)) == NULL) {
                ERROR("PyList_GetItem [envs] failure!");
            }
            if ((py_bytes = PyUnicode_AsUTF8String(py_item)) == NULL) {
                ERROR("PyUnicode_AsUTF8String [envs] failure!");
            }
            rconfig->envs[i] = PyBytes_AsString(py_bytes);
        }
        rconfig->envs[len] = NULL;
    }

    /** 解析是否使用 ptrace */
    if ((py_obj = PyDict_GetItemString(pconfig, "trace")) == NULL) {
        /** 默认使用 */
        rconfig->trace = 1;
    } else {
        if (py_obj == Py_True) {
            rconfig->trace = 1;
        } else {
            rconfig->trace = 0;
        }
    }

    /** 添加已有的系统调用白名单 */
    SetSyscallRules(rconfig);

END:
    if (res != 0) {
        /** 清理内存 */
        if (rconfig->exec_args != NULL) {
            free(rconfig->exec_args);
        }
        if (rconfig->envs != NULL) {
            free(rconfig->envs);
        }
    }
    return res;
}

/**
 * 用 C 结构体生成 Python Object
 * */
int GenPythonObject(struct RunnerStats *rstats, PyObject *stats) {
    PyObject *py_obj;
    int res = 0;

    if ((py_obj = PyLong_FromLong(rstats->time_used)) == NULL) {
        ERROR("PyLong_FromLong [time_used] failure!");
    } else {
        if (PyDict_SetItemString(stats, "time_used", py_obj) != 0) {
            ERROR("PyDict_SetItemString [time_used] failure!");
        }
    }

    if ((py_obj = PyLong_FromLong(rstats->real_time_used)) == NULL) {
        ERROR("PyLong_FromLong [real_time_used] failure!");
    } else {
        if (PyDict_SetItemString(stats, "real_time_used", py_obj) != 0) {
            ERROR("PyDict_SetItemString [real_time_used] failure!");
        }
    }

    if ((py_obj = PyLong_FromLong(rstats->memory_used)) == NULL) {
        ERROR("PyLong_FromLong [memory_used] failure!");
    } else {
        if (PyDict_SetItemString(stats, "memory_used", py_obj) != 0) {
            ERROR("PyDict_SetItemString [memory_used] failure!");
        }
    }

    if ((py_obj = PyLong_FromLong(rstats->signum)) == NULL) {
        ERROR("PyLong_FromLong [signum] failure!");
    } else {
        if (PyDict_SetItemString(stats, "signum", py_obj) != 0) {
            ERROR("PyDict_SetItemString [signum] failure!");
        }
    }

    if ((py_obj = PyLong_FromLong(rstats->re_flag)) == NULL) {
        ERROR("PyLong_FromLong [re_flag] failure!");
    } else {
        if (PyDict_SetItemString(stats, "re_flag", py_obj) != 0) {
            ERROR("PyDict_SetItemString [re_flag] failure!");
        }
    }

    if ((py_obj = PyLong_FromLong(rstats->re_syscall)) == NULL) {
        ERROR("PyLong_FromLong [re_syscall] failure!");
    } else {
        if (PyDict_SetItemString(stats, "re_syscall", py_obj) != 0) {
            ERROR("PyDict_SetItemString [re_syscall] failure!");
        }
    }

END:
    return res;
}
