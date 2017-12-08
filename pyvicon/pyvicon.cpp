#include <Python.h>
#include <string>
#include <iostream>
#include "DataStreamClient.h"

using namespace ViconDataStreamSDK;
using namespace CPP;

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

    //################
    //Module wrapped functions
    //################

    void destroy_client(PyObject* client)
    {
        delete (Client*)PyCapsule_GetPointer(client, "pyvicon.client");
    }

    static PyObject* new_client(PyObject* self, PyObject* args)
    {
        return PyCapsule_New(new Client(), "pyvicon.client", destroy_client);
    }

    static PyObject* pyvicon_version(PyObject* self, PyObject* args)
    {
        PyObject* capsule;
        if (!PyArg_ParseTuple(args, "O", &capsule)) return NULL;

        Client* client = (Client*)PyCapsule_GetPointer(capsule, "pyvicon.client");
        Output_GetVersion out = client->GetVersion();
        return Py_BuildValue("III", out.Major, out.Minor, out.Point);
    }

    //###################
    //Connection handling
    //###################

    static PyObject* pyvicon_connect(PyObject* self, PyObject* args)
    {
        PyObject* capsule;
        char* ip;
        if (!PyArg_ParseTuple(args, "Os", &capsule, &ip)) return NULL;
        Client* client = (Client*)PyCapsule_GetPointer(capsule, "pyvicon.client");

        //Release GIL for io
        Output_Connect out;
        Py_BEGIN_ALLOW_THREADS
        out = client->Connect(ip);
        Py_END_ALLOW_THREADS
        return Py_BuildValue("I", out.Result);
    }

    static PyObject* pyvicon_connect_to_multicast(PyObject* self, PyObject* args)
    {
        PyObject* capsule;
        char* ip;
        char* multicast;
        if (!PyArg_ParseTuple(args, "Oss", &capsule, &ip, &multicast)) return NULL;
        Client* client = (Client*)PyCapsule_GetPointer(capsule, "pyvicon.client");

        //Release GIL for io
        Output_ConnectToMulticast out;
        Py_BEGIN_ALLOW_THREADS
        out = client->ConnectToMulticast(ip, multicast);
        Py_END_ALLOW_THREADS
        return Py_BuildValue("I", out.Result);
    }

    static PyObject* pyvicon_disconnect(PyObject* self, PyObject* args)
    {
        PyObject* capsule;
        if (!PyArg_ParseTuple(args, "O", &capsule)) return NULL;
        Client* client = (Client*)PyCapsule_GetPointer(capsule, "pyvicon.client");

        Output_Disconnect out;
        out = client->Disconnect();
        return Py_BuildValue("I", out.Result);
    }

    static PyObject* pyvicon_isconnected(PyObject* self, PyObject* args)
    {
        PyObject* capsule;
        if (!PyArg_ParseTuple(args, "O", &capsule)) return NULL;

        Client* client = (Client*)PyCapsule_GetPointer(capsule, "pyvicon.client");
        if (client->IsConnected().Connected)
            Py_RETURN_TRUE;
        else
            Py_RETURN_FALSE;
    }

    static PyObject* pyvicon_start_transmitting_multicast(PyObject* self, PyObject* args)
    {
        PyObject* capsule;
        char* server_ip;
        char* multicast_ip;
        if (!PyArg_ParseTuple(args, "Oss", &capsule, &server_ip, &multicast_ip)) return NULL;
        Client* client = (Client*)PyCapsule_GetPointer(capsule, "pyvicon.client");

        //Release GIL for io
        Output_StartTransmittingMulticast out;
        Py_BEGIN_ALLOW_THREADS
        out = client->StartTransmittingMulticast(server_ip, multicast_ip);
        Py_END_ALLOW_THREADS
        return Py_BuildValue("I", out.Result);
    }

    static PyObject* pyvicon_stop_transmitting_multicast(PyObject* self, PyObject* args)
    {
        PyObject* capsule;
        if (!PyArg_ParseTuple(args, "O", &capsule)) return NULL;
        Client* client = (Client*)PyCapsule_GetPointer(capsule, "pyvicon.client");

        //Release GIL for io
        Output_StopTransmittingMulticast out;
        Py_BEGIN_ALLOW_THREADS
        out = client->StopTransmittingMulticast();
        Py_END_ALLOW_THREADS
        return Py_BuildValue("I", out.Result);
    }

    //###################
    // Configurations
    //###################

    static PyObject* pyvicon_set_buffer_size(PyObject* self, PyObject* args) {
        PyObject* capsule;
        unsigned int size;
        if (!PyArg_ParseTuple(args, "OI", &capsule, &size)) return NULL;
        Client* client = (Client*)PyCapsule_GetPointer(capsule, "pyvicon.client");

        client->SetBufferSize(size);
        Py_RETURN_NONE;
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

    //#####################
    // Methods
    //#####################
    static PyMethodDef PyviconMethods[] = {
         {"new_client", new_client, METH_VARARGS, "Create a Client instance"},
         {"pyvicon_version", pyvicon_version, METH_VARARGS, "Get SDK function"},
         {"pyvicon_connect", pyvicon_connect, METH_VARARGS, "Establish connection"},
         {"pyvicon_connect_to_multicast", pyvicon_connect_to_multicast, METH_VARARGS, "Establish a multicast connection"},
         {"pyvicon_disconnect", pyvicon_disconnect, METH_VARARGS, "Disconnect from connection"},
         {"pyvicon_isconnected", pyvicon_isconnected, METH_VARARGS, "Verify if connection is established"},
         {"pyvicon_start_transmitting_multicast", pyvicon_start_transmitting_multicast, METH_VARARGS, "Start multicast server transmission"},
         {"pyvicon_stop_transmitting_multicast", pyvicon_stop_transmitting_multicast, METH_VARARGS, "Stop multicast server transmission"},
         {"pyvicon_set_buffer_size", pyvicon_set_buffer_size, METH_VARARGS, "Set Frame buffer size"},
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


    //########################
    // Module init function
    //########################
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