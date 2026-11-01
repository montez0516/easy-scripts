from .API import API

class FileAPI(API):
    async def filepicker(self, accept : list[str] = ["*"], multiple : bool = False, defaultPath : str = "") -> str | list[str] | None:
        request = await self.dispatcher.request("scripts.api", {"method":"files.filepicker", "options":{"accept":accept, "multiple":multiple, "defaultPath":defaultPath}})
        return request["path"]