import json
import asyncio
import uuid

class EventDispatcher:
    def __init__(self, script_id : str = ""):
        self.id = script_id
        self._pending : dict[str, asyncio.Future] = {}
    def send(self, type : str, payload : dict = {}):
        event = {
            "type":3,
            "id":self.id,
            "name":type,
            "data":payload
        }

        print(json.dumps(event), flush=True)
    async def request(self):
        loop = asyncio.get_running_loop()

        req_id = str(uuid.uuid4())
        future = loop.create_future()

        self._pending[req_id] = future

        event = {
            "type":3,
            "id":
        }
