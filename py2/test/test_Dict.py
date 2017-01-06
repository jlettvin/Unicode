# -*- coding: utf8 -*-

__module__     = "test_Dict.py"
__author__     = "Jonathan D. Lettvin"
__copyright__  = "\
Copyright(C) 2016 Jonathan D. Lettvin, All Rights Reserved"
__credits__    = [ "Jonathan D. Lettvin" ]
__license__    = "GPLv3"
__version__    = "0.0.1"
__maintainer__ = "Jonathan D. Lettvin"
__email__      = "jlettvin@gmail.com"
__contact__    = "jlettvin@gmail.com"
__status__     = "Demonstration"
__date__       = "20161107"

import unittest2
import inspect

import sys
sys.path.append('.')
sys.path.append('..')

from pprint import pprint

from Dict   import ( Dict )
from Self   import ( Self )

class DictTestCase(unittest2.TestCase):

    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_empty(self):
        unidict = Dict()
        self.assertEqual(unidict, {})

    def test_simple(self):
        """
        Check for proper conversions of member names and data fields.
        """
        #TODO Fix stripping

        """
        before = {
            "Strip.This*Token": "But: Leave+This$Alone",
            "12BuckleMyShoe34TryItSomeMore": "Does'nt this work?"
        }
        after = {
            "StripThisToken": "But: Leave+This$Alone",
            "BuckleMyShoeTryItSomeMore": "Does\\'nt this work?"
        }
        unidict = Dict(**before)

        print "before: ",
        pprint(before)

        print "after: ",
        pprint(after)

        print "unidict: ",
        pprint(unidict)

        self.assertEqual(unidict, after, Self.doc())
        """
