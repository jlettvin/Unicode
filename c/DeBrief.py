#!/usr/bin/env python
# -*- coding: utf8 -*-

import sys
import glob

from Self import Self

class DeBrief(object):

    def __init__(self, line = "DeBrief Results", **kw):
        self.failpass = ["[FAIL]", "[PASS]"]
        self.term = ['|', '']
        self.line = []
        self.done = 0
        self.todo = []
        self.dash()
        self.report(line)

    def __str__(self):
        self. \
                dash(). \
                empty(). \
                valgrind(). \
                gprof(). \
                out(). \
                pep8(). \
                pyflakes(). \
                pylint(). \
                dash(). \
                summary(). \
                dash()
        return "\n".join(["%-75s%s" % (
            line, self.term[n==0]) for n, line in enumerate(self.line)])

    def report(self, line):
        self.line.append(line)
        return self

    def summary(self):
        self.report("%d review results" % (self.done))
        if len(self.todo) != 0:
            self.dash()
            self.empty()
        for todo in self.todo:
            self.report("[TODO] " + todo)
        return self

    def empty(self):
        self.line.append(" ")
        return self

    def dash(self):
        self.line.append("_"*75)
        return self

    def valgrind(self):
        """Examine valgrind output files for noteworthy memory leaks.
        Update the list on line[1] when the test_*.valgrind file list changes.
        """
        base = """\
test_B64
test_Classify
test_Endian
test_Page
test_PassFail
test_UTF8"""
        want = [nm.strip() for nm in base.split("\n")]
        have = [nm[0:-9] for nm in glob.glob("*.valgrind")]
        less = [nm for nm in want if not nm in have]
        more = [nm for nm in have if not nm in want]
        
        # expect = {'files': bases}
        for nm in less:
            self.report("[FAIL] Missing valgrind file: %s.valgrind" % (nm))
        for nm in more:
            self.report("[FAIL] Extra valgrind file: %s.valgrind" % (nm))
        if len(less) + len(more) == 0:
            self.report("[PASS] Valgrind files all present.")
        for nm in have:
            # TODO identify and find memory leaks due to tested code.
            with open(nm+".valgrind") as source:
                for line in source:
                    if "Command:" in line:
                        continue
                    if nm in line:
                        print line,
        self.done += 1
        return self

    def gprof(self):
        """Collect gnu C profiling data and identify uncovered code"""
        self.todo.append("Check gprof")
        return self

    def out(self):
        """
        """
        fails = 0
        with open("PassFail.out") as source:
            for line in source:
                if line.startswith("[FAIL]"):
                    if not "Expect [FAIL]" in line:
                        fails = fails + 1
        self.report("%s %d unexpected unit test failures" % (
            self.failpass[fails==0],
            fails))
        self.done += 1

        base = """\
B64.c.gcov
Page.c.gcov
UTF8.c.gcov
test_B64
test_Page
Classify.c.gcov
PassFail.c.gcov
doxygen
test_Classify
test_PassFail
Endian.c.gcov
PassFail
lint
test_Endian
test_UTF8"""
        return self

    def pep8(self):
        self.todo.append("Check pep8")
        return self

    def pyflakes(self):
        self.todo.append("Check pyflakes")
        return self

    def pylint(self):
        self.todo.append("Check pylint")
        return self


if __name__ == "__main__":
    portfolio = DeBrief()
    print portfolio
