from .API import API

class FileAPI(API):
    async def filepicker(self, folder : str = "") -> str | list[str]:
        file = await self.dispatcher.request("files.filepicker", {"folder":folder})
        self.dispatcher.send("scripts.media", payload={"type":"text", "text":f"recieved response from dispatcher returning {file}\n"})
        return file