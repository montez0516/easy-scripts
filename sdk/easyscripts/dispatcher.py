import json

class Dispatcher:
    def __init__(self, script_id : str = ""):
        self.id = script_id
    def emit(self, type : str, payload : dict = {}):
        event = {
            "type":3,
            "id":self.id,
            "name":type,
            "data":payload
        }

        print(json.dumps(event), flush=True)