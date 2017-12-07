#include <Python.h>
#include <string>
#include "DataStreamClient.h"

using namespace ViconDataStreamSDK;
using namespace CPP;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    void destroy_client(PyObject* client)
    {
        delete (Client*)PyCapsule_GetPointer(client, "client");
    }

    static PyObject* new_client(PyObject* self, PyObject* args)
    {
        return PyCapsule_New(new Client(), "client", destroy_client);
    }

    //###################
    //Module initialisation
    //###################

    struct module_state
    {
        PyObject *error;
    };

    #define GETSTATE(m) ((struct module_state*)PyModule_GetState(m))

    static PyObject * error_out(PyObject *m)
    {
        struct module_state *st = GETSTATE(m);
        PyErr_SetString(st->error, "something bad happened");
        return NULL;
    }

    static PyMethodDef PyviconMethods[] = {
         {"new_client", new_client, METH_VARARGS, "Create a Client instance"},
         {NULL, NULL, 0, NULL}
    };

    static int pyvicon_traverse(PyObject *m, visitproc visit, void *arg)
    {
        Py_VISIT(GETSTATE(m)->error);
        return 0;
    }

    static int pyvicon_clear(PyObject *m)
    {
        Py_CLEAR(GETSTATE(m)->error);
        return 0;
    }

    static struct PyModuleDef moduledef =
    {
        PyModuleDef_HEAD_INIT,
        "pyvicon_module",
        NULL,
        sizeof(struct module_state),
        PyviconMethods,
        NULL,
        pyvicon_traverse,
        pyvicon_clear,
        NULL
    };
    #define INITERROR return NULL

    PyMODINIT_FUNC PyInit_pyvicon_module(void) {
        PyObject* m;
        PyObject *module = PyModule_Create(&moduledef);

        if (module == NULL)
        INITERROR;
        struct module_state *st = GETSTATE(module);

        st->error = PyErr_NewException("pyvicon_module.Error", NULL, NULL);
        if (st->error == NULL)
        {
            Py_DECREF(module);
            INITERROR;
        }
    return module;
    }

#ifdef __cplusplus
} // end extern "C"
#endif // __cplusplus