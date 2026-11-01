import json
import threading
import sys
from .EventDispatcher import EventDispatcher
from .EventMonitor import EventMonitor
from .modules import FileAPI, MediaAPI, Args
import time
class EasyScriptsAPI:
    
    def __init__(self, name: str):
        self.dispatcher = EventDispatcher(name)
        self.monitor = EventMonitor(self.dispatcher)
        self.files : FileAPI = FileAPI(self.dispatcher)
        self.media : MediaAPI = MediaAPI(self.dispatcher)
        self.args : Args = Args()

    def wait(self, timeout : float | None = None):
        return self.monitor.wait(timeout)
    