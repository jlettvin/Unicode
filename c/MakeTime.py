#!/usr/bin/env python
#pragma pylint: disable=too-many-locals
#pragma pylint: disable=eval-used
#pragma pylint: disable=too-many-branches
#pragma pylint: disable=too-many-statements

"""\
{COMMAND} (Run this script without arguments to see options.)
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
        'top':    {
            'variant': ["-{N}", "--top"],
            'brief': "Generate report of top N stored timing data.",
            'prose': [
                "Same as '--report' except re-order by descending delta.",
                "If number is specified, only show the slowest N.",
                "Number may be specified after '--top' as in '--top 10'.",
                "Number may bs specified directly as '-3' or '-12'"
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

    # order maintains the sequence of options for output during HELP.
    order = ['clear', 'report', 'top', 'test', 'help']

    @staticmethod
    def brief(key):
        "This method constructs the lines for the OPTIONS portion of help."
        return (
            ", ".join([opt for opt in Option.key[key]['variant'] if opt]) +
            Option.key[key].get('extra', '') +
            "\n        %s\n" % (Option.key[key]['brief'])
        )

    @staticmethod
    def prose(key):
        "This method constructs the lines for the EXAMPLES portion of help."
        command = "\n    {COMMAND} "
        return (
            command+command.join(Option.key[key]['variant']) +
            "\n        " + "\n        ".join(Option.key[key]['prose'])
        )

    @staticmethod
    def help(msg):
        "This method fills and prints the help text."
        helpargs = (
            "\n    ".join(Option.brief(key) for key in Option.order),
            "\n    ".join(Option.prose(key) for key in Option.order)
        )
        print re.sub("{COMMAND}", sys.argv[0], __doc__ % helpargs)
        print msg
        sys.exit(0)

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
    def main(argc, argv):
        "Main entrypoint for command."
        result = {}
        marked = time.time()
        Option.command = argv[0]
        report = Option.key['report']['variant']
        top = Option.key['top']['variant']
        numeric = 0
        if argc > 1 and argv[1][0] == '-' and argv[1][1].isdigit():
            numeric = int(argv[1][1:])

        if argc == 1:
            Option.help("HELP requested when no args are given")

        if argv[1][0] == '-':
            if argv[1] in Option.key['test']['variant']:
                # "Generate the test Makefile"
                print re.sub("{COMMAND}", argv[0], Option.example)
                sys.exit(0)

            if argv[1] in Option.key['clear']['variant']:
                # "Clear the backstore."
                if os.path.exists(Option.store):
                    os.remove(Option.store)

            elif argv[1] in report or argv[1] in top or numeric != 0:
                # "Calculate elapsed times and print report"
                with open(Option.store, "r") as source:
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
                if argv[1] in top or numeric:
                    count = numeric if numeric > 0 else 0x10000
                    if argc > 2 and argv[1][1] == '-':
                        count = int(argv[2])
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

            else:  # -, -h, --help, or anything beginning with '-'
                Option.help("HELP requested")

        elif argc == 2:
            # "Per rule delta"
            result = {argv[1]: {'t1': marked}}
            with open(Option.store, "a") as target:
                print>>target, str(result)

        else:
            # "Per rule t0"
            result = {argv[1]: {'t0': marked, 'title': " ".join(argv[2:])}}
            with open(Option.store, "a") as target:
                print>>target, str(result)

    main(len(sys.argv), sys.argv)
