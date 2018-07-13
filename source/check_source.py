#!/bin/env python
"""
Check source code follows the rules!
"""

import os

CHECK_EXTS = ('.h', '.cpp')
CHECK_NAMES = ('CMakeLists.txt',)
IGNORE_DIRS = ('./platform/include/Gwork/External',)
IGNORE_FILES = ('DebugBreak.h', 'FontData.h')

class Stats:
    def __init__(self):
        self.files = 0
        self.lines = 0
        self.problems = 0

STATS = Stats()


def report(fpath, linenb, line, msg):
    print '[{}:{}] {}'.format(fpath, line, line)
    print '\t{}'.format(msg)
    STATS.problems += 1


def check_line(fpath, line, linenb):
    STATS.lines += 1

    # check for tabs used
    if '\t' in line:
        report(fpath, linenb, line, 'Tab used. Use spaces to indent.')

    # check line length
    # if len(line) > 100:
    #     report(fpath, linenb, line, 'Line too long.')


def check_file(fpath):
    STATS.files += 1    
    with open(fpath, 'rb') as fh:
        for (i, line) in enumerate(fh.readlines()):
            check_line(fpath, line, i+1)


def check():
    def check_dir(arg, dirname, names):
        if dirname in IGNORE_DIRS:
            return
        for fname in names:
            if fname in IGNORE_FILES:
                return
            name,ext = os.path.splitext(fname)
            if (ext in CHECK_EXTS) or (fname in CHECK_NAMES):
                check_file(os.path.join(dirname,fname))
    os.path.walk('.', check_dir, None)

    print '{} files checked.'.format(STATS.files)
    print '{} lines.'.format(STATS.lines)
    print '{} problems were found.'.format(STATS.problems)


check()
