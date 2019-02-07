#include <Python.h>

PyObject *run(PyObject *self, PyObject *args) {
    PyObject *stats;
    stats = Py_None;
    return stats;
}

/**
 * Extending Python with C or C++
 * Docs: https://docs.python.org/3/extending/extending.html
 * */
static PyMethodDef JudgeLightRunnerMethods[] = {
    {"run", run, METH_VARARGS, "JudgeLightRunner"}, {NULL, NULL, 0, NULL}};

static struct PyModuleDef judgelightrunnermodule = {
    PyModuleDef_HEAD_INIT, "JudgeLightRunner", NULL, -1,
    JudgeLightRunnerMethods};

PyMODINIT_FUNC PyInit_JudgeLightRunner(void) {
    PyObject *m;
    m = PyModule_Create(&judgelightrunnermodule);
    if (m == NULL) {
        return NULL;
    }
    return m;
}
