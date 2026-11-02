from .API import API

class FileAPI(API):
    async def filepicker(self, accept : list[str] = ["*"], multiple : bool = False, defaultPath : str = "") -> str | list[str] | None:
        request = await self.dispatcher.request("scripts.api", {"method":"files.filepicker", "options":{"accept":accept, "multiple":multiple, "defaultPath":defaultPath}})

        if not request:
            return ""
        
        return request["path"]
    async def directorypicker(self, existingDir : str = "") -> str:
        request = await self.dispatcher.request("scripts.api", {"method":"files.directorypicker", "existingDir":existingDir})

        if not request:
            return ""

        return request["path"]