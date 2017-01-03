#!/usr/bin/env python
# -*- coding: utf8 -*-

"""DeBrief.py

Usage:
    DeBrief [--verbose=V]
    DeBrief (-h | --help)
    DeBrief (--version)

Options:
    --verbose=V                     Output detail lines [default:0]
    -h, --help                      Show this Usage message
    --version                       Show version

Description:
    Review outputs occuring during a process (i.e. make) and
    reports back on two conditions:
        1) outputs indicating problems identified by  absence of data
        2) outputs indicating problems identified by presence of data

    It achieves this by importing and instancing modules named DeBrief_*.py.
    Each module is responsible for analyzing one kind of output file.
    For instance: DeBrief_gcov.py identifies lines of code not executed

    in all local files named "*.gcov".
    output lines of interest have one of the two forms:
        1) "#####:\s*%d:%s"
        2) "%d:\s*%d:%s" (if "%d" == "0")

    In verbose mode, it reports all such lines (per file).
    In non-verbose mode, it reports the count of all such lines (per file).

    in all local files named "*.gcov.out".
    output files include a second line of the form:
        1) "Lines executed:\s*%f%% of %d" (if "%f" != "100.00%")

TODO: Each module should identify missing and extra file/output associations.
"""

import sys
import glob

from docopt     import (docopt)
from pprint     import (pprint)

from Self       import (Self)
from Dictionary import (Dictionary)

__module__     = "DeBrief.py"
__author__     = "Jonathan D. Lettvin"
__copyright__  = "\
Copyright(C) 2016 Jonathan D. Lettvin, All Rights Reserved"
__credits__    = ["Jonathan D. Lettvin"]
__license__    = "GPLv3"
__version__    = "0.0.1"
__maintainer__ = "Jonathan D. Lettvin"
__email__      = "jlettvin@gmail.com"
__contact__    = "jlettvin@gmail.com"
__status__     = "Demonstration"
__date__       = "20161231"


## This class is the core driver of all DeBrief functionality (run by main).
class DeBrief(object):
    "Root of the DeBrief analysis code tree."

    verbose = 0

    ## Initialize shared resources for all DeBrief modules.
    def __init__(self, title = "DeBrief Results", **kw):
        "Initialize shared resources for all DeBrief modules."
        self.arg = kw
        ## text-area width of the final summary
        self.width = 75
        ## common display strings
        self.failpass = ["[FAIL]", "[PASS]"]
        ## boundary characters for text-area
        self.term = ['|', '']
        ## accumulator for report lines
        self.line = []
        ## accumulator for todo messages
        self.todo = {}
        ## insert top boundary
        self.dash()
        ## accumulate title
        self.report(title)
        ## Choose loquacity
        DeBrief.verbose = int(kw.get("verbose", 0) or 0)

    ## Functor to execute module functors.
    def __call__(self):
        "Functor to execute module functors."
        modules = [module[:-3] for module in glob.glob("DeBrief_*.py")]
        loaded = {module:__import__(module) for module in modules}
        self.dash().empty()
        for name, code in loaded.iteritems():
            code.DeBrief(self)()
        self.dash().summary().dash()
        return self

    ## Display the accumulated analytics.
    def __str__(self):
        "Display the accumulated analytics."
        fmt = "%-" + "%ds" % (self.width) + "%s"
        return "\n".join([fmt % (
            line, self.term[n==0]) for n, line in enumerate(self.line)])

    ## Mechanism used by all modules for accumulating analytics.
    def report(self, line):
        "Mechanism used by all modules for accumulating analytics."
        self.line.append(line)
        return self

    ## Assemble accumulated analytics.
    def summary(self):
        "Assemble accumulated analytics."
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

    ## Put an empty line in the analytics accumulator.
    def empty(self):
        "Put an empty line in the analytics accumulator."
        self.line.append(" ")
        return self

    ## Put a horizontal line in the analytics accumulator.
    def dash(self):
        "Put a horizontal line in the analytics accumulator."
        self.line.append("_" * self.width)
        return self

## This is the parent class for all modules.
class DeBrief_Module(object):
    "This is the parent class for all modules."

    ## Initialize instance
    def __init__(self, root, name, contract):
        "This is the common initialization done for all modules."
        ## DeBrief is the link back to the child class
        self.DeBrief = root
        ## report links back to the report function
        self.report = self.DeBrief.report
        ## todo links back to the todo accumulator
        self.todo = self.DeBrief.todo
        ## report links back to the common strings for fail and pass
        self.failpass = self.DeBrief.failpass
        ## name is the child instance name
        self.name = name
        ## contract is what the child is supposed to report
        self.contract = contract

    ## Instance functor
    def __call__(self):
        "Default DeBrief_Module functor"
        self.todo[self.name or __name__] = self.contract
        return self


if __name__ == "__main__":

    cmd = sys.argv[0]
    arg = Dictionary(**docopt(__doc__, version=__version__))

    portfolio = DeBrief(**arg)
    print portfolio()
