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
        "Follow all suggestions except those we ignore"
        self.todo[__name__] = Self.doc()
        return self
