from ..EventDispatcher import EventDispatcher

class API:
    def __init__(self, dispatcher : EventDispatcher):
        self.dispatcher = dispatcher