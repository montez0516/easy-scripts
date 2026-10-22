from easyscripts import Dispatcher
from .API import API

class FileAPI(API):

    def filepicker(self, folder : str = ""):
        self.dispatcher.emit("files.filepicker", {"folder":folder})