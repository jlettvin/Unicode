#!/usr/bin/env python
# -*- coding: utf8 -*-

"""
This module identifies problems in coverage.
"""

import os
import glob

from pprint     import (pprint)

from Self       import (Self)
from DeBrief    import (DeBrief_Module)


## DeBrief is the same name used in all DeBrief_Module derived classes.
class DeBrief(DeBrief_Module):
    "This class is named the same in each specific module."

    ## Initialize instance
    def __init__(self, common):
        "Identify coverage problems"
        super(DeBrief, self).__init__(common, __name__, Self.doc())

    def __call__(self):
        "Evaluate file pairings and coverage contents"

        FAIL = self.failpass[0]

        # Collect names for all .c, .h, and .c.gcov files
        actual = {
            "c": {"all": glob.glob("*.c")},
            "h": {"all": glob.glob("*.h")},
            "g": {"all": glob.glob("*.c.gcov")}
        }

        # Eliminate c files beginning with "test_"
        actual["c"]["all"] = [
            name for name in actual["c"]["all"]
            if not name.startswith("test_")]

        # Trim filenames down to shared basenames.
        cfiles = actual["c"]["all"] = [
            name[0:-2] for name in actual["c"]["all"]]
        hfiles = actual["h"]["all"] = [
            name[0:-2] for name in actual["h"]["all"]]
        gfiles = actual["g"]["all"] = [
            name[0:-7] for name in actual["g"]["all"]]

        # Some header files have no companion C files.
        exclude = ["Types", "Unicode"]
        hfiles = actual["h"]["all"] = [
            name for name in actual["h"]["all"] if name not in exclude]

        # Find files missing companion .c, .h, or .c.gcov files
        cmissing = [name for name in hfiles if name not in cfiles]
        hmissing = [name for name in cfiles if name not in hfiles]
        gmissing = [name for name in cfiles if name not in gfiles]

        # Get lengths of lists
        for key, val in actual.iteritems():
            actual[key]["len"] = len(actual[key]["all"])

        # Check for different lengths of .c and .h file lists
        if actual["c"]["len"] != actual["h"]["len"]:
            self.report("%s .c and .h file counts must match" % (FAIL))

        for cfile in hmissing:
            self.report("%s File(%s) no .h companion" % (FAIL, cfile))

        for hfile in cmissing:
            self.report("%s File(%s) no .c companion" % (FAIL, hfile))

        for cfile in gmissing:
            self.report("%s File(%s) no .c.gcov companion" % (FAIL, hfile))
