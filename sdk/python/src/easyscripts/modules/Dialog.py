from .API import API

class DialogAPI(API):

    async def input(self) -> str:
        request = await self.dispatcher.request("scripts.api", {"method":"dialog.input"})

        return request or ""

    async def select(self, items : list[str] = []) -> str:
        request = await self.dispatcher.request("scripts.api", {"method":"dialog.select", "list":items})

        return request or ""