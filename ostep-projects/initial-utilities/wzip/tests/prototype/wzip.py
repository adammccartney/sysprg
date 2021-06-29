# wzip.py: prototype of a simple file compression program


class Token:
    "Token holds the char currently being read from a stream"

    def __init__(self, ch):
        self.ch = ch
        self.chi8 = self._chtoi8(ch)  # converts ch to 1 byte ascii

    def __repr__(self):
        return f"(ch={self.ch!r}, ch8={self.ch8!r})"

    def _chtoi8(self, ch):
        chi8 = ord(ch)
        if (chi8 < 0) or (chi8 > 127):
            print("Error: ch outside of ascii range 0 - 127")
        return chi8

    def touch(self, ch):  # udpates attributes
        self.__init__(ch)


class ShortStack:
    "ShortStack holds up to three characters: <ascii char> <\n> <EOF>"

    def __init__(self):
        self.items = []
        self.head = 0
        self.maxsize = 3

    def push(self, item):
        if (self.head >= self.maxsize):
            raise Exception("Error: stack overflow")

        self.items.append(item)
        self.head += 1

    def pop(self):
        if (self.head == 0):
            raise Exception("Error: stack underflow")

        self.items.pop(self.head - 1)  # list is zero index, head not
        self.head -= 1
