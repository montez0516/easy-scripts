from .API import API

class MediaAPI(API):
    def image(self, src : str = ""):
        return self.dispatcher.request("scripts.media", {"type":"image", "src":src})
    def video(self, src : str = ""):
        return self.dispatcher.request("scrits.media", {"type":"video", "src":src})
    def text(self, text : str = ""):
        return self.dispatcher.request("scripts.media", {"type":"text", "text":text})