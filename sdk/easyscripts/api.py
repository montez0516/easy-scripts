import json
import threading
import sys


class EasyScriptsAPI:
    
    def __init__(self, name: str):
        self.name = name
        self.shutdown_event = threading.Event()

        self.thread = threading.Thread(
            target=self.monitorEvents
        )

        self.thread.start()

    def monitorEvents(self):
        file = open("file.txt", "a")
        for line in sys.stdin:
            try:
                message = json.loads(line)
                file.write(message.get("name"))
                if message.get("name") == "script.shutdown":
                    file.write("shutting down\n")
                    self.shutdown_event.set()
                    file.write("shut down\n")
                    file.close()
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