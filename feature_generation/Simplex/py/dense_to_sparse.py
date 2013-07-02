#!/usr/bin/env python2.7

import sys

if __name__ == '__main__':
    if len(sys.argv) != 1 + 2:
        print >> sys.stderr, "Usage %s dense_input_file sparse_out_file" % (sys.argv[0], )
        sys.exit(-1)

    with open(sys.argv[1], 'r') as input_file:
        with open(sys.argv[2], 'w+') as output_file:
            for line in input_file.read().split('\n'):
                if line.__len__() == 0:
                    continue
                for idx, col in enumerate(line.strip().split(' ')):
                    if idx > 0:
                        output_file.write(' %d:%s' % (idx, col))
                    else:
                        output_file.write('%s' % (col,))
                output_file.write('\n')




