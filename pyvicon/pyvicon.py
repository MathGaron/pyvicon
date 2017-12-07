import pyvicon_module


class PyVicon:
    def __init__(self):
        self.client_ = pyvicon_module.new_client()


if __name__ == '__main__':
    test = PyVicon()