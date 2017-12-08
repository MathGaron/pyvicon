import pyvicon_module

from enum import Enum


class Result(Enum):
    Unknown = 0
    NotImplemented = 1
    Success = 2
    InvalidHostName = 3
    InvalidMulticastIP = 4
    ClientAlreadyConnected = 5
    ClientConnectionFailed = 6
    ServerAlreadyTransmittingMulticast = 7
    ServerNotTransmittingMulticast = 8
    NotConnected = 9
    NoFrame = 10
    InvalidIndex = 11
    InvalidCameraName = 12
    InvalidSubjectName = 13
    InvalidSegmentName = 14
    InvalidMarkerName = 15
    InvalidDeviceName = 16
    InvalidDeviceOutputName = 17
    InvalidLatencySampleName = 18
    CoLinearAxes = 19
    LeftHandedAxes = 20
    HapticAlreadySet = 21


class StreamMode(Enum):
    ClientPull = 0
    ClientPullPreFetch = 1
    ServerPush = 2


class Direction(Enum):
    Up = 0
    Down = 1
    Left = 2
    Right = 3
    Forward = 4
    Backward = 5


class PyVicon:
    def __init__(self):
        self.client_ = pyvicon_module.new_client()
        major, minor, point = pyvicon_module.pyvicon_version(self.client_)
        self.__version__ = "{}.{}.{}".format(major, minor, point)

    def connect(self, ip):
        return Result(pyvicon_module.pyvicon_connect(self.client_, ip))

    def connect_multicast(self, ip, multicast):
        return Result(pyvicon_module.pyvicon_connect_to_multicast(self.client_, ip, multicast))

    def disconnect(self):
        return Result(pyvicon_module.pyvicon_disconnect(self.client_))

    def is_connected(self):
        return pyvicon_module.pyvicon_isconnected(self.client_)

    def start_server_multicast(self, server_ip, multicast_ip):
        return Result(pyvicon_module.pyvicon_start_transmitting_multicast(self.client_, server_ip, multicast_ip))

    def stop_server_multicast(self):
        return Result(pyvicon_module.pyvicon_stop_transmitting_multicast(self.client_))

    def set_buffer_size(self, size):
        pyvicon_module.pyvicon_set_buffer_size(self.client_, size)

    def set_stream_mode(self, stream_mode):
        self.check_enum(stream_mode, StreamMode, "set_stream_mode")
        return Result(pyvicon_module.pyvicon_set_stream_mode(self.client_, stream_mode.value))

    def set_axis_mapping(self, X, Y, Z):
        """
        Common : Z-up (Forward, Left, Up)
                 Y-up (Forward, Up, Right)
        """
        self.check_enum(X, Direction, "set_axis_mapping")
        self.check_enum(Y, Direction, "set_axis_mapping")
        self.check_enum(Z, Direction, "set_axis_mapping")
        return Result(pyvicon_module.pyvicon_set_axis_mapping(self.client_, X.value, Y.value, Z.value))

    def get_axis_mapping(self):
        X, Y, Z = pyvicon_module.pyvicon_get_axis_mapping(self.client_)
        return Direction(X), Direction(Y), Direction(Z)

    def get_frame(self):
        return Result(pyvicon_module.pyvicon_get_frame(self.client_))

    def get_frame_number(self):
        return pyvicon_module.pyvicon_get_frame_number(self.client_)

    def get_time_code(self):
        return pyvicon_module.pyvicon_get_time_code(self.client_)

    def get_frame_rate(self):
        return pyvicon_module.pyvicon_get_frame_rate(self.client_)

    def get_subject_count(self):
        return pyvicon_module.pyvicon_get_subject_count(self.client_)

    def get_subject_name(self, index):
        return pyvicon_module.pyvicon_get_subject_name(self.client_, index)

    def get_subject_quality(self, name):
        return pyvicon_module.pyvicon_get_object_quality(self.client_, name)

    def get_marker_count(self, name):
        return pyvicon_module.pyvicon_get_marker_count(self.client_, name)

    def get_marker_name(self, name, index):
        return pyvicon_module.pyvicon_get_marker_name(self.client_, name, index)

    def get_marker_global_translation(self, subject_name, marker_name):
        """
        if the value is [0, 0, 0] it means that the marker is occluded in this frame (We keep it like that as it is nearly
            impossible to have a prefect match with the origin.
        """
        return pyvicon_module.pyvicon_get_marker_global_translation(self.client_, subject_name, marker_name)

    def get_unlabeled_marker_count(self):
        return pyvicon_module.pyvicon_get_unlabeled_marker_count(self.client_)

    def get_unlabeled_marker_global_translation(self, index):
        return pyvicon_module.pyvicon_get_unlabeled_marker_global_translation(self.client_, index)

    def get_camera_count(self):
        return pyvicon_module.pyvicon_get_camera_count(self.client_)

    def get_camera_name(self, index):
        return pyvicon_module.pyvicon_get_camera_name(self.client_, index)

    @staticmethod
    def check_enum(value, enum, method):
        if not isinstance(value, enum):
            raise RuntimeError("{} expect a {}, received a {}".format(method, enum, type(value)))

if __name__ == '__main__':
    test = PyVicon()
    print("SDK version : {}".format(test.__version__))
    print("Connection status : {}".format(test.is_connected()))
    print(test.connect("127.0.0.1"))
    print(test.connect_multicast("127.0.0.1", "224.0.0.0"))
    print(test.disconnect())
    print(test.start_server_multicast("localhost", "224.0.0.0"))
    print(test.stop_server_multicast())
    print(test.set_stream_mode(StreamMode.ClientPull))
    print(test.get_axis_mapping())
    print(test.get_subject_name(0))
