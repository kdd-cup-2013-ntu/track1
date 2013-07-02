#!/usr/bin/env python2.7
from subprocess import Popen
import sys
def main(argv):
    if len(argv) != 1 + 5:
        print >> sys.stderr, 'Usage : %s raw_data_directory train.csv valid.csv train.feature valid.feature' % (argv[0],)
        return -1

    makefile = Popen(['make', 'copy-basic-feature','DATA=%s' % (argv[1]), 'TRAIN=%s' % (argv[2]), 'VALID=%s' % (argv[3]), 'TRAIN_FEATURE=%s' % (argv[4],), 'VALID_FEATURE=%s' % (argv[5],)])
    makefile.wait()

if __name__ == '__main__':
    sys.exit(main(sys.argv))

