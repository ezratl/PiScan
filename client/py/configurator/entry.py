# data structures for json

class Entry:
    def __init__(self, tag, lockout, delay):
        self.tag = tag
        self.lockout = lockout
        self.delay = delay

class Channel(Entry):
    def __init__(self, frequency, tag, lockout, delay):
        Entry.__init__(self, tag, lockout, delay)
        self.frequency = frequency

class FMChannel(Channel):
    def __init__(self, frequency, tag, lockout, delay):
        Channel.__init__(self, frequency, tag, lockout, delay)

class PLChannel(FMChannel):
    def __init__(self, frequency, tone, tag, lockout, delay):
        FMChannel.__init__(self, frequency, tag, lockout, delay)
        self.tone = tone

class DCChannel(FMChannel):
    def __init__(self, frequency, code, tag, lockout, delay):
        FMChannel.__init__(self, frequency, tag, lockout, delay)
        self.code = code

class AMChannel(Channel):
    def __init__(self, frequency, tag, lockout, delay):
        Channel.__init__(self, frequency, tag, lockout, delay)
