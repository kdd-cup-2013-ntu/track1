#!/usr/bin/python3

import sys


def duplicate_handler(csv_file):
    with open(csv_file, 'r') as f:
        line_count = 0
        for line in f:
            line_count += 1
            if line_count == 1:
                print(line.strip())  # print the header
                continue
            arr = line.split(',')
            print(arr[0], end=',')
            arr = arr[1].strip().split(' ')
            uniq_list, dupl_list = [], []
            for x in arr:
                if x not in uniq_list:
                    uniq_list.append(x)
                else:
                    dupl_list.append(x)
            print(*(uniq_list + dupl_list), sep=' ')


if __name__ == '__main__':
    duplicate_handler(sys.argv[1])
