from .API import API

class FileAPI(API):
    def filepicker(self, folder : str = ""):
        return self.dispatcher.request("files.filepicker", {"folder":folder})
    