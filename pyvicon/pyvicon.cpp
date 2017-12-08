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

    // Output_SetStreamMode SetStreamMode( const StreamMode::Enum Mode );
    // Output_SetApexDeviceFeedback SetApexDeviceFeedback( const String& i_rDeviceName, bool i_bOn );
    // Output_SetAxisMapping SetAxisMapping( const Direction::Enum XAxis, const Direction::Enum YAxis, const Direction::Enum ZAxis );
    // Output_GetAxisMapping GetAxisMapping() const;
    // Output_GetServerOrientation GetServerOrientation() const;
    // Output_GetFrame GetFrame();
    // Output_GetFrameNumber GetFrameNumber() const;
    // Output_GetTimecode GetTimecode() const;
    // Output_GetFrameRate GetFrameRate() const;
    // Output_GetLatencySampleCount  GetLatencySampleCount() const;
    // Output_GetLatencySampleName   GetLatencySampleName( const unsigned int LatencySampleIndex ) const;
    // Output_GetLatencySampleValue  GetLatencySampleValue( const String & LatencySampleName ) const;
    // Output_GetLatencyTotal        GetLatencyTotal() const;
    // Output_GetHardwareFrameNumber GetHardwareFrameNumber() const;
    // Output_GetFrameRateCount  GetFrameRateCount() const;
    // Output_GetFrameRateName   GetFrameRateName( const unsigned int FrameRateIndex ) const;
    // Output_GetFrameRateValue  GetFrameRateValue( const String & FrameRateName ) const;
    // Output_GetSubjectCount GetSubjectCount() const;
    // Output_GetSubjectName GetSubjectName( const unsigned int SubjectIndex ) const;
    // Output_GetSubjectRootSegmentName GetSubjectRootSegmentName( const String & SubjectName ) const;
    // Output_GetSegmentCount GetSegmentCount( const String  & SubjectName ) const;
    // Output_GetSegmentName GetSegmentName( const String       & SubjectName, const unsigned int   SegmentIndex ) const;
    // Output_GetSegmentChildCount GetSegmentChildCount( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentChildName GetSegmentChildName( const String       & SubjectName, const String       & SegmentName, const unsigned int   SegmentIndex ) const;
    // Output_GetSegmentParentName GetSegmentParentName( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentStaticTranslation GetSegmentStaticTranslation( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentStaticRotationHelical GetSegmentStaticRotationHelical( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentStaticRotationMatrix GetSegmentStaticRotationMatrix( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentStaticRotationQuaternion GetSegmentStaticRotationQuaternion( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentStaticRotationEulerXYZ GetSegmentStaticRotationEulerXYZ( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentGlobalTranslation GetSegmentGlobalTranslation( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentGlobalRotationHelical GetSegmentGlobalRotationHelical( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentGlobalRotationMatrix GetSegmentGlobalRotationMatrix( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentGlobalRotationQuaternion GetSegmentGlobalRotationQuaternion( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentGlobalRotationEulerXYZ GetSegmentGlobalRotationEulerXYZ( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentLocalTranslation GetSegmentLocalTranslation( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentLocalRotationHelical GetSegmentLocalRotationHelical( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentLocalRotationMatrix GetSegmentLocalRotationMatrix( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentLocalRotationQuaternion GetSegmentLocalRotationQuaternion( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetSegmentLocalRotationEulerXYZ GetSegmentLocalRotationEulerXYZ( const String & SubjectName, const String & SegmentName ) const;
    // Output_GetObjectQuality GetObjectQuality( const String & ObjectName ) const;
    // Output_GetMarkerCount GetMarkerCount( const String  & SubjectName ) const;
    // Output_GetMarkerName GetMarkerName( const String & SubjectName, const unsigned int  MarkerIndex ) const;
    // Output_GetMarkerParentName GetMarkerParentName( const String & SubjectName, const String & MarkerName ) const;
    // Output_GetMarkerGlobalTranslation GetMarkerGlobalTranslation( const String & SubjectName, const String & MarkerName ) const;
    // Output_GetMarkerRayContributionCount GetMarkerRayContributionCount( const String & SubjectName, const String & MarkerName ) const;
    // Output_GetMarkerRayContribution GetMarkerRayContribution( const String & SubjectName, const String & MarkerName, unsigned int MarkerRayContributionIndex ) const;
    // Output_GetUnlabeledMarkerCount GetUnlabeledMarkerCount() const;
    // Output_GetUnlabeledMarkerGlobalTranslation GetUnlabeledMarkerGlobalTranslation( const unsigned int MarkerIndex ) const;
    // Output_GetLabeledMarkerCount GetLabeledMarkerCount() const;
    // Output_GetLabeledMarkerGlobalTranslation GetLabeledMarkerGlobalTranslation( const unsigned int MarkerIndex ) const;
    // Output_GetDeviceCount GetDeviceCount() const;
    // Output_GetDeviceName  GetDeviceName( const unsigned int DeviceIndex ) const;
    // Output_GetDeviceOutputCount GetDeviceOutputCount( const String  & DeviceName ) const;
    // Output_GetDeviceOutputName GetDeviceOutputName( const String  & DeviceName, const unsigned int   DeviceOutputIndex ) const;
    // Output_GetDeviceOutputValue GetDeviceOutputValue( const String & DeviceName, const String & DeviceOutputName ) const;
    // Output_GetDeviceOutputSubsamples GetDeviceOutputSubsamples( const String & DeviceName, const String & DeviceOutputName ) const;
    // Output_GetDeviceOutputValue GetDeviceOutputValue( const String & DeviceName, const String & DeviceOutputName, const unsigned int Subsample ) const;
    // Output_GetForcePlateCount GetForcePlateCount() const;
    // Output_GetGlobalForceVector GetGlobalForceVector( const unsigned int ForcePlateIndex ) const;
    // Output_GetGlobalMomentVector GetGlobalMomentVector( const unsigned int ForcePlateIndex ) const;
    // Output_GetGlobalCentreOfPressure GetGlobalCentreOfPressure( const unsigned int ForcePlateIndex ) const;
    // Output_GetForcePlateSubsamples GetForcePlateSubsamples( const unsigned int ForcePlateIndex ) const;
    // Output_GetGlobalForceVector GetGlobalForceVector( const unsigned int ForcePlateIndex, const unsigned int Subsample ) const;
    // Output_GetGlobalMomentVector GetGlobalMomentVector( const unsigned int ForcePlateIndex, const unsigned int Subsample ) const;
    // Output_GetGlobalCentreOfPressure GetGlobalCentreOfPressure( const unsigned int ForcePlateIndex, const unsigned int Subsample ) const;
    // Output_GetEyeTrackerCount GetEyeTrackerCount() const;
    // Output_GetEyeTrackerGlobalPosition GetEyeTrackerGlobalPosition( const unsigned int EyeTrackerIndex ) const;
    // Output_GetEyeTrackerGlobalGazeVector GetEyeTrackerGlobalGazeVector( const unsigned int EyeTrackerIndex ) const;
    // Output_GetCameraCount GetCameraCount() const;
    // Output_GetCameraName GetCameraName( unsigned int i_CameraIndex ) const;
    // Output_GetCameraId GetCameraId( const std::string & i_rCameraName ) const;
    // Output_GetCameraUserId GetCameraUserId( const std::string & i_rCameraName ) const;
    // Output_GetCameraType GetCameraType( const std::string & i_rCameraName ) const;
    // Output_GetCameraDisplayName GetCameraDisplayName(  const std::string & i_rCameraName ) const;
    // Output_GetCameraResolution GetCameraResolution( const std::string & i_rCameraName ) const;
    // Output_GetIsVideoCamera GetIsVideoCamera( const std::string & i_rCameraName ) const;
    // Output_GetCentroidCount GetCentroidCount( const std::string & i_rCameraName ) const;
    // Output_GetCentroidPosition GetCentroidPosition( const std::string & i_rCameraName, const unsigned int i_CentroidIndex ) const;
    // Output_GetCentroidWeight GetCentroidWeight( const std::string & i_rCameraName, const unsigned int i_CentroidIndex ) const;
    // Output_GetGreyscaleBlobCount GetGreyscaleBlobCount( const std::string & i_rCameraName ) const;
    // Output_GetGreyscaleBlob GetGreyscaleBlob( const std::string & i_rCameraName, const unsigned int i_BlobIndex ) const;
    // Output_SetCameraFilter SetCameraFilter( const std::vector< unsigned int > & i_rCameraIdsForCentroids, const std::vector< unsigned int > & i_rCameraIdsForBlobs );

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