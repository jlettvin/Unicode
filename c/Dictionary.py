#!/usr/bin/env python
# -*- coding: utf8 -*-

"""Dictionary.py
This class converts dictionary element names into object member names
It strips out all non-alpha characters before this conversion.

Justification: Most of these modules make use of simplified dictionary access.
"""

__module__     = "Dictionary.py"
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
__date__       = "20161107"


## Dictionary converts the["syntax"] to the.syntax.
class Dictionary(dict):
    "Convert a dictionary into a namespace."

    @staticmethod
    def _escape(string):
        if isinstance(string, str):
            string = string.replace("'", "\\'")
        return string

    ## Connect and initialize the elements.
    def __init__(self, **kw):  # pylint: disable=super-init-not-called
        self.__dict__ = self
        self(**kw)

    ## Update the elements.
    def __call__(self, **kw):
        self.update({
            filter(str.isalpha, k): Dictionary._escape(w)
            for k, w in kw.iteritems()
        })

    ## Serialize the elements for use in javascript.
    def javascript(self, var):
        "Emit as javascript readable string"
        string  = "var %s = {\n    " % (var)
        string += ",\n    ".join([
            "%s => '%s'" % (k, w)
            for k, w in self.iteritems()])
        string += "\n};"
        return string


if __name__ == "__main__":

    SAMPLE = {
        " Strip.This*Token": "But: Leave+This$Alone",
        "12BuckleMyShoe34TryItSomeMore": "Does'nt this work?"
    }
    DICTIONARY = Dictionary(**SAMPLE)

    print '-' * 78
    print DICTIONARY.javascript("sample")
    print '-' * 78
    print DICTIONARY.StripThisToken  # pylint: disable=no-member
    print '-' * 78

