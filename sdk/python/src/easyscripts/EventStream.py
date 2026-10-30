from .EventDispatcher import EventDispatcher

class EventStream:
    def __init__(self, dispatcher : EventDispatcher):
        self._buffer : str = ""
        self._dispatcher = dispatcher

    def write(self, text : str) -> int:
        self._buffer += text

        while "\n" in self._buffer:
            pos = self._buffer.find("\n")

            line = self._buffer[0:pos + 1]

            self._dispatcher.send("scripts.media", {"type":"text", "text":line})

            self._buffer = self._buffer[pos + 1:]

            return len(text)
    def flush(self):
        if self._buffer:
            self._dispatcher.send("scripts.media", {"type":"text", "text": self._buffer + "\n"})
            self._buffer = ""