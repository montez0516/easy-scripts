from easyscripts import Dispatcher
from .API import API

class MediaAPI(API):
    
    def image(self, src : str = ""):
        self.dispatcher.emit("scripts.media", {"type":"image", "src":src})
    def video():
        pass
    def text():
        pass
    def graph():
        pass