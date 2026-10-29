from .API import API

class FileAPI(API):
    async def filepicker(self, folder : str = "") -> str | list[str]:
        return await self.dispatcher.request("scripts.api", {"method":"files.filepicker", "folder":folder})