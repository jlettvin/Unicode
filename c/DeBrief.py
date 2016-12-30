#!/usr/bin/env python
# -*- coding: utf8 -*-

import sys
import glob

from Self import Self

class DeBrief(object):

    def __init__(self, line = "DeBrief Results", **kw):
        self.width = 75
        self.failpass = ["[FAIL]", "[PASS]"]
        self.term = ['|', '']
        self.line = []
        self.todo = {}
        self.dash()
        self.report(line)

    def __call__(self):
        modules = [module[:-3] for module in glob.glob("DeBrief_*.py")]
        loaded = {module:__import__(module) for module in modules}
        self.dash().empty()
        for name, code in loaded.iteritems():
            code.DeBrief(self)()
        self.dash().summary().dash()
        return self

    def __str__(self):
        fmt = "%-" + "%ds" % (self.width) + "%s"
        return "\n".join([fmt % (
            line, self.term[n==0]) for n, line in enumerate(self.line)])

    def report(self, line):
        self.line.append(line)
        return self

    def summary(self):
        ignore = ["", "done"]
        done = {k: w for k, w in self.todo.iteritems() if w.lower() in ignore}
        need = {k: w for k, w in self.todo.iteritems() if k not in done.keys()}
        self.report("%d review results" % (len(done)))
        if len(need) != 0:
            self.dash()
            self.empty()
        for todo, explain in need.iteritems():
            self.report("[TODO] " + todo)
            self.report("       " + explain)
        return self

    def empty(self):
        self.line.append(" ")
        return self

    def dash(self):
        self.line.append("_" * self.width)
        return self


if __name__ == "__main__":
    portfolio = DeBrief()
    print portfolio()
