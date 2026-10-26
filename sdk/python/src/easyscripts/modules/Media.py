from .API import API

class MediaAPI(API):
    def image(self, src : str = ""):
        return self.dispatcher.send("scripts.media", {"type":"image", "src":src})
    def video(self, src : str = ""):
        return self.dispatcher.send("scrits.media", {"type":"video", "src":src})
    def text(self, text : str = ""):
        return self.dispatcher.send("scripts.media", {"type":"text", "text":text})