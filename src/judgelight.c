#include <Python.h>

PyObject *run(PyObject *self, PyObject *args) {
    printf("Hello World!\n");
    return args;
}

/**
 * Extending Python with C or C++
 * Docs: https://docs.python.org/3/extending/extending.html
 * */
static PyMethodDef JudgeLightMethods[] = {
    {"run", run, METH_VARARGS, "JudgeLight.run(config)"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef judgelightmodule = {
    PyModuleDef_HEAD_INIT, "JudgeLight", NULL, -1, JudgeLightMethods};

PyMODINIT_FUNC PyInit_JudgeLight(void) {
    PyObject *m;
    m = PyModule_Create(&judgelightmodule);
    if (m == NULL) {
        return NULL;
    }
    return m;
}
