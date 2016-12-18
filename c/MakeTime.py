#!/usr/bin/env python

"""
MakeTime.py(support)
NAME
    MakeTime.py - Maintain storage for Makefile rule elapsed times.
SYNOPSIS
    MakeTime.py [OPTION]
    MakeTime.py [label] [title text]
DESCRIPTION
    Hold timing data in the external file 'MakeTime.json'.
OPTIONS
    -c, --clean, --clear, --initialize
        Delete the external file.

    -s, --summary, --summarize, --report
        Generate summary of stored timing data.

    -h, --help, (anything else beginning with '-')
        Show this help

EXAMPLES
    ./MakeTime.py
    ./MakeTime.py -h
    ./MakeTime.py --help
        Show help

    ./MakeTime.py -c
    ./MakeTime.py --clean
    ./MakeTime.py --clear
    ./MakeTime.py --initialize
        Delete JSON file 'MakeTime.json' to prepare for new timings.

    ./MakeTime.py START How much time does the make execution take?
        Associate title and t0 for START rule

    ./MakeTime.py START
        Associate t1 to JSON for START rule

    ./MakeTime.py -r
    ./MakeTime.py -s
    ./MakeTime.py --report
    ./MakeTime.py --summary
    ./MakeTime.py --summarize
        Perform calculations for execution times then
        summarize collected timings with labels to help.


EXAMPLE MAKEFILE
###############################################################################
all:    START rule1 rule2 END

.PHONY:
START:
        @./MakeTime.py --clear
	@./MakeTime.py $@ Elapsed during the entire make process.

.PHONY:
rule1:
	@./MakeTime.py $@ Elapsed during rule1.
	sleep 3
	@./MakeTime.py $@

.PHONY:
rule2:
	@./MakeTime.py $@ Elapsed during rule2.
	sleep 5
	@./MakeTime.py $@

.PHONY:
END:
	@./MakeTime.py START
	@./MakeTime.py --summary
###############################################################################

COPYRIGHT
    Copyright(c)2016 Jonathan D. Lettvin, All Rights Reserved
"""

from time import time
from sys import argv
from pprint import pprint
from os import remove
from os.path import exists

store = "MakeTime.json"
result = {}
marked = time()


if len(argv) == 1:
    "When no args are given, display the help text."
    print __doc__

elif len(argv) == 2:
    arg1 = argv[1]

    if arg1 in ["-c", "--clean", "--clear", "--initialize"]:
        "Clear the backstore."
        if exists(store):
            remove(store)

    elif arg1 in ["-r", "-s", "--summary", "--summarize", "--report"]:
        "Calculate elapsed times and print summary"
        with open(store, "r") as source:
            names = []
            seen = set()
            for n, line in enumerate(source):
                data = eval(line)
                for k,v in data.iteritems():
                    result[k] = result.get(k, {})
                    result[k].update(v)
                    if not k in seen:
                        seen.add(k)
                        names.append(k)
        for k in names:
            v = result[k]
            timing = "unknown MakeTime"
            if v.has_key('t1') and v.has_key('t0'):
                timing = "%4.3f seconds" % (float(v['t1']) - float(v['t0']))
            print("\t\t%s [%s] %s" % (timing, k, v['title']))

    elif arg1[0] == '-':  # -, -h, --help, or anything beginning with '-'
        print __doc__

    else:
        "Per rule delta"
        result = {arg1: {'t1': marked}}
        with open(store, "a") as target:
            print>>target, str(result)

else:
    "Per rule t0"
    result = {argv[1]: {'t0': marked, 'title': " ".join(argv[2:])}}
    with open(store, "a") as target:
        print>>target, str(result)
