#!/usr/bin/env python
# -*- coding: utf8 -*-

"""
This module identifies PEP8 reports of style problems.
"""

import glob

from Self       import (Self)
from DeBrief    import (DeBrief, DeBrief_Module)


## DeBrief is the same name used in all DeBrief_Module derived classes.
class DeBrief(DeBrief_Module):
    "This class is named the same in each specific module."

    ## Initialize instance
    def __init__(self, common):
        "Follow all PEP8 suggestions except those we ignore"
        super(DeBrief, self).__init__(common, __name__, Self.doc())

    def __call__(self):
        FAIL = self.failpass[0] + " gcov: "
        fails = 0
        for gcov in glob.glob("*.gcov"):
            with open(gcov) as source:
                for line in source:
                    line = line.strip()
                    if line.startswith("0:") or line.startswith("#####:"):
                        fails += 1
                        if self.DeBrief.verbose:
                            material = line.partition(":")[2].strip()
                            self.report(FAIL + material)
            with open(gcov + ".out") as source:
                line = source.readlines()
                second = line[1].partition(":")[2]
                if not second.startswith("100.00%"):
                    self.report(FAIL + "(%s) not at 100%%" % (gcov + ".out"))
            if fails and not self.DeBrief.verbose:
                self.report(FAIL + "(%s) %d lines unexecuted" % (gcov, fails))
