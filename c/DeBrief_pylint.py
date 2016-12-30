#!/usr/bin/env python
# -*- coding: utf8 -*-

import glob

from Self import Self

class DeBrief(object):

    def __init__(self, DeBrief):
        self.DeBrief = DeBrief
        self.report = self.DeBrief.report
        self.todo = self.DeBrief.todo
        self.failpass = self.DeBrief.failpass

    def __call__(self):
        "Handle .out files"
        self.todo[__name__] = Self.doc()
        fails = 0
        with open("PassFail.out") as source:
            for line in source:
                if line.startswith("[FAIL]"):
                    if not "Expect [FAIL]" in line:
                        fails = fails + 1
        self.report("%s %d unexpected unit test failures" % (
            self.failpass[fails==0],
            fails))
        self.todo[Self.name()] = "done"

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

