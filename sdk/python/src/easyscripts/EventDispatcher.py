import json
import asyncio
import uuid
import sys
import atexit

class EventDispatcher:
    def __init__(self, script_id : str = ""):
        self.id = script_id
        self._original_stdout = sys.stdout
        
        from .EventStream import EventStream
        self._stream = EventStream(self)
        sys.stdout = self._stream
        
        self._pending : dict[str, asyncio.Future] = {}

        self._shutdown = False


        atexit.register(self.shutdown)
    def send(self, event_type : str, payload : dict | None = None, promiseID : str = ""):
            event = {
            "type":event_type,
            "scriptName":self.id,
            "promiseID":promiseID,
            "data":payload
            }
            
            self._original_stdout.write(json.dumps(event) + "\n")
            self._original_stdout.flush()
    async def request(self, event_type : str, payload : dict | None = None):
        loop = asyncio.get_running_loop()

        req_id = str(uuid.uuid4())
        future = loop.create_future()

        self._pending[req_id] = future

        try:
            self.send(event_type, payload, req_id)
            result = await future
            return result
        finally:
            self._pending.pop(req_id)
    def response(self, payload : dict):


        promiseID = payload.get("promiseID")

        if not promiseID:
            return
        
        future : asyncio.Future = self._pending.get(promiseID)
        
        if not future:
            return    


        loop = future.get_loop()
        loop.call_soon_threadsafe(future.set_result, payload.get("data", ""))

    def shutdown(self):
        if self._shutdown:
            return
        
        self._shutdown = True
        try:
            self._stream.flush()
        finally:
            if sys.stdout is self._stream:
                sys.stdout = self._original_stdout
