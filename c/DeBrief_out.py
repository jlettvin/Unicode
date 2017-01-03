#!/usr/bin/env python
# -*- coding: utf8 -*-

"""
This module analyzes *.out files
and reports findings back to the DeBrief.py caller.
"""

import glob

from Self import Self
from DeBrief import DeBrief_Module


## DeBrief is the same name used in all DeBrief_Module derived classes.
class DeBrief(DeBrief_Module):
    "This class is named the same in each specific module."

    ## Initialize instance
    def __init__(self, common):
        "Eliminate problems discoverable in .out files"
        super(DeBrief, self).__init__(common, __name__, Self.doc())

    ## Instance functor
    def __call__(self):
        "Handle .pylint files"
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
        return self

