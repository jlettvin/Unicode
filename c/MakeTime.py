#!/usr/bin/env python

"""
{COMMAND} (run without arguments to see options)
NAME
    {COMMAND} - Maintain json granular elapsed time database for Makefiles.
SYNOPSIS
    {COMMAND} [OPTION]
    {COMMAND} {label} [{title text}]
DESCRIPTION
    Hold timing data in the external file 'MakeTime.json'.
OPTIONS
    %s
EXAMPLES
    {COMMAND} START Elapsed time for Makefile execution.
        Add t0 and title to JSON for START rule

    {COMMAND} START
        Add t1 to JSON for START rule
    %s

COPYRIGHT
    Copyright(c)2016 Jonathan D. Lettvin, All Rights Reserved
"""

###############################################################################
import re
import os
import sys
import time

###############################################################################
class Option(object):
    """
    The first argument to {COMMAND} must be either an option or a label.
    This class handles all data and display related to options.
    All viable options are keys in the dictionary.
    Each option has a list of variants which may appear on the command-line.
    This class is not intended to be instanced.
    All operations use static members and methods.
    """

    store = "MakeTime.json"

    key = {
        'clear':    {
            'variant': ["-c", "-i", "--clean", "--clear", "--initialize"],
            'brief': "Delete timing backstore file (MakeTime.json).",
            'prose': [
                "Delete JSON file 'MakeTime.json' to prepare for new timings."
            ]
        },
        'report':   {
            'variant': ["-r", "-s", "--report", "--summary", "--summarize"],
            'brief': "Generate report of stored timing data.",
            'prose': [
                "Perform calculations for execution times then",
                "report collected timings with labels to help."
            ]
        },
        'help':     {
            'variant': ["", "-h", "--help", "--info"],
            'extra': " (or other args beginning with '-')",
            'brief': "Show this help.",
            'prose': ["Show this help message."]
        },
        'test':     {
            'variant': ["-t", "--test", "--unittest", "--unit-test"],
            'brief': "Generate test Makefile on stdout (use as example)",
            'prose': [
                "A functioning Makefile is output on stdout.",
                "Redirect it to a test Makefile for testing (i.e. test.mk).",
                "Run 'make -f {filename}' using that test Makefile."
            ]
        }
    }

    # order maintains the sequence of options for output during HELP.
    order = ['clear', 'report', 'test', 'help']

    @staticmethod
    def brief(key):
        return (
            ", ".join([opt for opt in Option.key[key]['variant'] if opt]) +
            Option.key[key].get('extra', '') +
            "\n        %s\n" % (Option.key[key]['brief'])
        )

    @staticmethod
    def prose(key):
        command = "\n    {COMMAND} "
        return (
            command+command.join(Option.key[key]['variant']) +
            "\n        " + "\n        ".join(Option.key[key]['prose'])
        )

    @staticmethod
    def help(msg):
        helpargs = (
            "\n    ".join(Option.brief(key) for key in Option.order),
            "\n    ".join(Option.prose(key) for key in Option.order)
        )
        print re.sub("{COMMAND}", sys.argv[0], __doc__ % helpargs)
        sys.exit(0)

###############################################################################
example = """\
#!/usr/bin/env make
#### EXAMPLE Makefile to unit test {COMMAND} ###
all:    START rule1 rule2 END
\t@echo '>>>\trule: $@: rule END should output something like:'
\t@echo '     8.128 seconds [START] Elapsed during the entire make process.'
\t@echo '     3.029 seconds [rule1] Elapsed during rule1.'
\t@echo '     5.025 seconds [rule2] Elapsed during rule2.'

.PHONY:
START:
\t@echo '>>>\trule: $@'
\t@{COMMAND} --clear
\t@{COMMAND} $@ Elapsed during the entire make process.

.PHONY:
rule1:
\t@echo '>>>\trule: $@'
\t@{COMMAND} $@ Elapsed during rule1.
\tsleep 3
\t@{COMMAND} $@

.PHONY:
rule2:
\t@echo '>>>\trule: $@'
\t@{COMMAND} $@ Elapsed during rule2.
\tsleep 5
\t@{COMMAND} $@

.PHONY:
END:    # Note how the label 'START' is re-used from the START rule.
\t@echo '>>>\trule: $@'
\t@{COMMAND} START
\t@{COMMAND} --report
###############################################################################
"""

###############################################################################
if __name__ == "__main__":

    def main(argc, argv):
        "Main entrypoint for command."
        result = {}
        marked = time.time()
        Option.command = argv[0]

        if argc == 1:
            Option.help("HELP requested when no args are given")

        elif argc == 2:
            arg1 = argv[1]

            if arg1 in Option.key['test']['variant']:
                print re.sub("{COMMAND}", argv[0], example)
                sys.exit(0)

            if arg1 in Option.key['clear']['variant']:
                "Clear the backstore."
                if os.path.exists(Option.store):
                    os.remove(Option.store)

            elif arg1 in Option.key['report']['variant']:
                "Calculate elapsed times and print report"
                with open(Option.store, "r") as source:
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
                        timing = "%6.3f seconds" % (float(v['t1']) - float(v['t0']))
                    print("    %s [%s] %s" % (timing, k, v['title']))

            elif arg1[0] == '-':  # -, -h, --help, or anything beginning with '-'
                help("HELP requested")

            else:
                "Per rule delta"
                result = {arg1: {'t1': marked}}
                with open(Option.store, "a") as target:
                    print>>target, str(result)

        else:
            "Per rule t0"
            result = {argv[1]: {'t0': marked, 'title': " ".join(argv[2:])}}
            with open(Option.store, "a") as target:
                print>>target, str(result)

    main(len(sys.argv), sys.argv)
