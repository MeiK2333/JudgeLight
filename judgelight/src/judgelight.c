#include "listen.h"
#include "run.h"
#include <Python.h>

static PyObject *judgelight(PyObject *self, PyObject *args) {
    char tmp[22] = "Hello JudgeLight!";
    return PyBytes_FromString(tmp);
}

static PyObject *init(PyObject *self, PyObject *args) {
    int operin[2]; // 创建operin与operout管道来发送指令和响应
    pipe(operin);

    int operout[2];
    pipe(operout);

    pid_t pid = fork();

    if (pid == 0) {
        close(operin[1]);
        close(operout[0]);

        _listen(operin[0], operout[1]);
        exit(0);
    } else {
        close(operin[0]);
        close(operout[1]);

        PyObject *list = PyList_New(3);
        PyList_SetItem(list, 0, PyLong_FromLong(pid));
        PyList_SetItem(list, 1, PyLong_FromLong(operin[1]));
        PyList_SetItem(list, 2, PyLong_FromLong(operout[0]));
        return list;
    }
}

static PyObject *genResult(struct Result result) {
    PyObject *rst;
    rst = PyDict_New();
    PyDict_SetItemString(rst, "ru_utime", PyLong_FromLong(result.ru_utime));
    PyDict_SetItemString(rst, "ru_stime", PyLong_FromLong(result.ru_stime));
    PyDict_SetItemString(rst, "ru_maxrss", PyLong_FromLong(result.ru_maxrss));
    PyDict_SetItemString(rst, "ru_ixrss", PyLong_FromLong(result.ru_ixrss));
    PyDict_SetItemString(rst, "ru_idrss", PyLong_FromLong(result.ru_idrss));
    PyDict_SetItemString(rst, "ru_isrss", PyLong_FromLong(result.ru_isrss));
    PyDict_SetItemString(rst, "ru_minflt", PyLong_FromLong(result.ru_minflt));
    PyDict_SetItemString(rst, "ru_majflt", PyLong_FromLong(result.ru_majflt));
    PyDict_SetItemString(rst, "ru_nswap", PyLong_FromLong(result.ru_nswap));
    PyDict_SetItemString(rst, "ru_inblock", PyLong_FromLong(result.ru_inblock));
    PyDict_SetItemString(rst, "ru_oublock", PyLong_FromLong(result.ru_oublock));
    PyDict_SetItemString(rst, "ru_msgsnd", PyLong_FromLong(result.ru_msgsnd));
    PyDict_SetItemString(rst, "ru_msgrcv", PyLong_FromLong(result.ru_msgrcv));
    PyDict_SetItemString(rst, "ru_nsignals",
                         PyLong_FromLong(result.ru_nsignals));
    PyDict_SetItemString(rst, "ru_nvcsw", PyLong_FromLong(result.ru_nvcsw));
    PyDict_SetItemString(rst, "ru_nivcsw", PyLong_FromLong(result.ru_nivcsw));
    PyDict_SetItemString(rst, "time_used", PyLong_FromLong(result.time_used));
    PyDict_SetItemString(rst, "memory_used",
                         PyLong_FromLong(result.memory_used));
    PyDict_SetItemString(rst, "status", PyLong_FromLong(result.status));
    PyDict_SetItemString(rst, "error", PyLong_FromLong(result.error));
    PyDict_SetItemString(rst, "reason", PyBytes_FromString(result.reason));
    return rst;
}

static PyObject *run(PyObject *self, PyObject *args) {
    pid_t pid;
    if (!PyArg_ParseTuple(args, "i", &pid)) {
        return NULL;
    }
    // pid_t pid = (pid_t)PyLong_AsLong(args);
    struct Result result = runit(pid);
    return genResult(result);
}

#if PY_MAJOR_VERSION >= 3

static PyMethodDef JudgeLightMethods[] = {
    {"init", init, METH_VARARGS, ""},
    {"judgelight", judgelight, METH_VARARGS, ""},
    {"run", run, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef judgelightmodule = {
    PyModuleDef_HEAD_INIT, "judgelight", "judgelight", -1, JudgeLightMethods};

PyMODINIT_FUNC PyInit_judgelight(void) {
    return PyModule_Create(&judgelightmodule);
}

#else

static PyMethodDef GreateModuleMethods[] = {
    {"init", init, METH_VARARGS, ""},
    {"judgelight", judgelight, METH_VARARGS, ""},
    {"run", run, METH_VARARGS, ""},
    {NULL, NULL, 0, NULL}};

PyMODINIT_FUNC initjudgelight(void) {
    (void)Py_InitModule("judgelight", GreateModuleMethods);
}

#endif