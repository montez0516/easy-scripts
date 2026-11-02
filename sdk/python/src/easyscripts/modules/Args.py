import sys
import argparse
from ..EventDispatcher import EventDispatcher

class Args:
    def __init__(self):
        self._parser = argparse.ArgumentParser()

        self._parser.add_argument("--text", type=str)
        self._parser.add_argument("--urls", nargs="*")

        self._args = self._parser.parse_args()

    def getText(self):
        return self._args.text or ""
    def getUrls(self):
        return self._args.urls or []