import json
import threading
import sys


class EasyScriptsAPI:
    
    def __init__(self, name: str):
        self.name = name
        self.shutdown_event = threading.Event()

        self.thread = threading.Thread(
            target=self.monitorEvents,
            daemon=True
        )

        self.thread.start()

    def monitorEvents(self):
        for line in sys.stdin:
            try:
                message = json.loads(line)
                if message.get("name") == "script.shutdown":
                    self.shutdown_event.set()
                    return

            except json.JSONDecodeError:
                pass
    def wait(self, timeout=None):
        return self.shutdown_event.wait(timeout)

    def sendEvent(self, event_type: str, data=None):
        message = {
            "type": 3,
            "id": self.name,
            "name": event_type,
            "data": data
        }

        print(json.dumps(message), flush=True)