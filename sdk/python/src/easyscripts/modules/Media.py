from .API import API

class MediaAPI(API):
    def image(self, src : str = ""):
        self.dispatcher.emit("scripts.media", {"type":"image", "src":src})
    def video(self, src : str = ""):
        self.dispatcher.emit("scrits.media", {"type":"video", "src":src})
    def text(self, text : str = ""):
        self.dispatcher.emit("scripts.media", {"type":"text", "text":text})