import json
import threading
import sys
from easyscripts import Dispatcher

class EasyScriptsAPI:
    
    def __init__(self, name: str):
        self.shutdown_event = threading.Event()
        self.dispatcher = Dispatcher(name)

        self.thread = threading.Thread(
            target=self.monitorEvents,
            daemon=True
        )

        self.thread.start()

    def monitorEvents(self):
        for line in sys.stdin:
            try:
                message = json.loads(line)
                if message.get("name") == "scripts.shutdown":
                    self.shutdown_event.set()
                    return

            except json.JSONDecodeError:
                pass
    def wait(self, timeout=None):
        return self.shutdown_event.wait(timeout)