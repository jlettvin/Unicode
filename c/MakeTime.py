#!/usr/bin/env python
# -*- coding: utf8 -*-

#pragma pylint: disable=too-many-locals
#pragma pylint: disable=eval-used
#pragma pylint: disable=too-many-branches
#pragma pylint: disable=too-many-statements


"""MakeTime.py

Usage:
    MakeTime.py (LABEL [<text>...])
    MakeTime.py (-i | --initialize)
    MakeTime.py (-r | --report)
    MakeTime.py (-u | --unit)
    MakeTime.py (-t N | --top=N)
    MakeTime.py (-h | --help)
    MakeTime.py (--version)

Options:
    -i, --initialize                Clear historical data
    -r, --report                    Generate total granular report of timings
    -t N, --top=N                   Report top N longest times
    -u, --unit                      Generate unit test Makefile on stdout
    -h, --help                      Show this Usage message
    --version                       Show version

Description:
    Generate granular timings for make rule execution.
    Hold timing data in the external file 'MakeTime.json'.
    Report timing data.
    The first argument to MakeTime.py must be either an option or a label.

Examples:
    MakeTime.py -i
        Delete JSON file 'MakeTime.json' to prepare for new timings.

    MakeTime.py START Elapsed time for Makefile execution.
        Add t0 and title to JSON for START rule

    MakeTime.py START
        Add t1 to JSON for START rule

    MakeTime.py -r
        Perform calculations for execution times then
        report collected timings with labels to help.

    MakeTime.py -t 2
        Same as -r except sort by descending elapsed time then
        report this many lines of report starting from the top.

    MakeTime.py -u
        Generate a sample unit test makefile to stdout.
        Recommended execution of unit test:
            $ MakeTime.py -u > test.mk
            $ make -f test.mk

    MakeTime.py -h
        Show this help

Author  : Jonathan D. Lettvin (jlettvin@gmail.com)
Date    : 20161220
Legal   : Copyright(c)2016 Jonathan D. Lettvin, All Rights Reserved
License : GPL 3.0
"""

###############################################################################
import re
import os
import sys
import time
import docopt

import Dictionary

__module__     = "MakeTime.py"
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
__date__       = "20161220"

unit = """\
#!/usr/bin/env make
#### EXAMPLE Makefile to unit test MakeTime.py ###
all:    START rule1 rule2 END
\t@echo '>>>\trule: $@: rule END should output something like:'
\t@echo '     8.128 seconds [START] Elapsed during the entire make process.'
\t@echo '     3.029 seconds [rule1] Elapsed during rule1.'
\t@echo '     5.025 seconds [rule2] Elapsed during rule2.'

.PHONY:
START:
\t@echo '>>>\trule: $@'
\t@./MakeTime.py -i
\t@./MakeTime.py $@ Elapsed during the entire make process.

.PHONY:
rule1:
\t@echo '>>>\trule: $@'
\t@./MakeTime.py $@ Elapsed during $@.
\tsleep 3
\t@./MakeTime.py $@

.PHONY:
rule2:
\t@echo '>>>\trule: $@'
\t@./MakeTime.py $@ Elapsed during $@.
\tsleep 5
\t@./MakeTime.py $@

.PHONY:
END:    # Note how the label 'START' is re-used from the START rule.
\t@echo '>>>\trule: $@'
\t@./MakeTime.py START
\t@./MakeTime.py -r
###############################################################################
"""

backstore = "MakeTime.json"

###############################################################################
# Implementation Details:
# Goal: Document time elapsed during execution of rules.
# Makefile functionality does not include performance measurements for make.
# To remedy this, this script enables thread-safe (I believe) time recording.
# The reason I believe it is thread-safe is that writes to the backstore file
# are shorter than the maximum buffer size guaranteed to be atomic when writing.
# The backstore file is opened in append mode, so records accumulate
# to be ingested and merged into a single dictionary by the reporting mechanism.

if __name__ == "__main__":

    cmd = sys.argv[0]
    arg = Dictionary.Dictionary(**docopt.docopt(__doc__, version=__version__))

    def main(arg):
        "Main entrypoint for command."
        result = {}
        marked = time.time()
        #Option.command = argv[0]
        #report = Option.key['report']['variant']
        #top = Option.key['top']['variant']
        numeric = 0
        if arg.top is not None:
            numeric = int(arg.top)

        if arg.unit:
            # "Generate the test Makefile"
            print unit
            sys.exit(0)

        elif arg.initialize:
            # "Clear the backstore."
            if os.path.exists(backstore):
                os.remove(backstore)

        elif arg.LABEL is not None:
            label, text = (arg.LABEL, arg.text)
            if text == []:
                # "Per rule delta"
                result = {label: {'t1': marked}}
                with open(backstore, "a") as target:
                    print>>target, str(result)
            else:
                # "Per rule t0"
                result = {label: {'t0': marked, 'title': " ".join(text)}}
                with open(backstore, "a") as target:
                    print>>target, str(result)

        elif arg.report or arg.top:
            # "Calculate elapsed times and print report"
            with open(backstore, "r") as source:
                names = []
                seen = set()
                for line in source:
                    data = eval(line)
                    for key, val in data.iteritems():
                        result[key] = result.get(key, {})
                        result[key].update(val)
                        if key not in seen:
                            seen.add(key)
                            names.append(key)
            assemble = []
            timed = {}
            for k in names:
                # "Assemble the report"
                field = result[k]
                timing = "unknown MakeTime"
                if 't0' in field and 't1' in field:
                    delta = float(field['t1']) - float(field['t0'])
                    timing = "%6.3f seconds" % (delta)
                text = "    %s [%s] %s" % (timing, k, field['title'])
                assemble += [text]
                timed[delta] = timed.get(delta, [])
                timed[delta].append(text)
            if not arg.report:
                count = numeric if numeric > 0 else 0x10000
                descending = sorted(timed.keys())[::-1]
                index = 0
                assemble = []
                while count > 0:
                    count -= len(timed[descending[index]])
                    for line in timed[descending[index]]:
                        assemble += [line]
                    index = index + 1
                    if index >= len(descending):
                        break
            print '\n'.join(assemble)

    main(arg)
