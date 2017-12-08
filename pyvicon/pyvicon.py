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

if __name__ == '__main__':
    test = PyVicon()
    print("SDK version : {}".format(test.__version__))
    print("Connection status : {}".format(test.is_connected()))
    print(test.connect("127.0.0.1"))
    print(test.connect_multicast("127.0.0.1", "224.0.0.0"))
    print(test.disconnect())
    print(test.start_server_multicast("localhost", "224.0.0.0"))
    print(test.stop_server_multicast())
