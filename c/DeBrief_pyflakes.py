#!/usr/bin/env python
# -*- coding: utf8 -*-

"""
This module reports problems discovered by pyflakes.
"""

import glob

from Self import Self
from DeBrief import DeBrief_Module


## DeBrief is the same name used in all DeBrief_Module derived classes.
class DeBrief(DeBrief_Module):
    "This class is named the same in each specific module."

    ## Initialize instance
    def __init__(self, common):
        "Identify pyflakes errors/warnings except those we ignore"
        super(DeBrief, self).__init__(common, __name__, Self.doc())
