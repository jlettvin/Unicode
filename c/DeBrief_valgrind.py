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
        FAIL, PASS = self.failpass
        
        # expect = {'files': bases}
        for nm in less:
            self.report("%s Missing valgrind file: %s.valgrind" % (FAIL, nm))
        for nm in more:
            self.report("%s Extra valgrind file: %s.valgrind" % (FAIL, nm))
        if len(less) + len(more) == 0:
            self.report("%s Valgrind files all present." % (PASS))
        for nm in have:
            # TODO identify and find memory leaks due to tested code.
            with open(nm+".valgrind") as source:
                for line in source:
                    if "Command:" in line:
                        continue
                    if nm in line:
                        print line,
        self.todo[__name__ + " file count"] = "done"
        self.todo[__name__ + " suppression"] = "Generate and use .supp files"
        return self
