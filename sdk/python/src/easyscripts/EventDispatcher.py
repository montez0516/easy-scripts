import json
import asyncio
import uuid

class EventDispatcher:
    def __init__(self, script_id : str = ""):
        self.id = script_id
        self._pending : dict[str, asyncio.Future] = {}
    def send(self, event_type : str, payload : dict | None = None, promiseID : str = ""):
        event = {
            "type":event_type,
            "scriptName":self.id,
            "promiseID":promiseID,
            "data":payload
        }

        print(json.dumps(event), flush=True)
    async def request(self, event_type : str, payload : dict | None = None):
        loop = asyncio.get_running_loop()

        req_id = str(uuid.uuid4())
        future = loop.create_future()

        self._pending[req_id] = future

        try:
            self.send(event_type, payload, req_id)
            result = await future
            self.send("scripts.media", payload={"type":"text", "text":f"Future is done returning result {result}\n"})
            return result
        finally:
            self._pending.pop(req_id)
    def response(self, payload : dict):



        self.send("scripts.media", payload={"type":"text", "text":f"Recieved response in dispatcher {payload}\n"})


        self.send("scripts.media", payload={"type":"text", "text":"getting response promiseID\n"})
        promiseID = payload.get("promiseID")

        if not promiseID:
            self.send("scripts.media", payload={"type":"text", "text":f"no promise id found for response\n"})
            return
        self.send("scripts.media", payload={"type":"text", "text":f"got response promiseID {promiseID}\n"})
        
        self.send("scripts.media", payload={"type":"text", "text":f"getting future for promiseID {promiseID}\n"})
        future : asyncio.Future = self._pending.get(promiseID)
        
        if not future:
            self.send("scripts.media", payload={"type":"text", "text":f"no future found for promiseID {promiseID}\n"})
            return    

        self.send("scripts.media", payload={"type":"text", "text":f"found future for promiseID {promiseID} setting result to {payload.get("data")}\n"})

        loop = future.get_loop()
        loop.call_soon_threadsafe(future.set_result, payload.get("data", ""))

