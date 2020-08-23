# radio system structures for json

import entry

class RadioSystem:
    def __init__(self, tag, lockout):
        self.tag = tag
        self.lockout = lockout

class AnalogSystem(RadioSystem):
    def __init__(self, tag, lockout):
        RadioSystem.__init__(self, tag, lockout)
        self.entries = []

    def addChannel(self, channel):
        entries.append(channel)
