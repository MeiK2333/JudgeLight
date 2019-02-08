#ifndef JL_CONVERT_H
#define JL_CONVERT_H

#include "jl_runner.h"

#include <Python.h>

int ParsePythonArgs(PyObject *, struct RunnerConfig *);

int GenPythonObject(struct RunnerStats *, PyObject *);

#endif
