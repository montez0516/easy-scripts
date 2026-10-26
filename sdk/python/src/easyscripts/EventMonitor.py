import threading
from .EventDispatcher import EventDispatcher
import sys
import json

class EventMonitor:
    def __init__(self, dispatcher : EventDispatcher):
        self.dispatcher : EventDispatcher = dispatcher
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
                if message.get("type") == "scripts.shutdown":
                    self.shutdown_event.set()
                    return
                else:
                    self.dispatcher.response(message)

            except json.JSONDecodeError:
                pass
    def wait(self, timeout : float | None = None):
        return self.shutdown_event.wait(timeout)