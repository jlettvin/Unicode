#!/usr/bin/env python
# -*- coding: utf8 -*-

import sys

class DeBrief(object):

    def __init__(self, line = "DeBrief Results", **kw):
        self.dash = "+" + ("-"*77) + "+\n"
        self.line = [line]

    def __call__(self, line, **kw):
        self.line.append(line)
        return self

    def __str__(self):
        return "".join([
            self.dash,
            "\n".join(["| %-75s |" % line for line in self.line]),
            "\n| %-75s |\n" % ("%d lines" % (len(self.line))),
            self.dash])

if __name__ == "__main__":
    result = DeBrief()
    result("TODO: No analysis yet.")
    print result
