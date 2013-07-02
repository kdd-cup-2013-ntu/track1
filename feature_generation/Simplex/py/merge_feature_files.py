#!/usr/bin/env python3

import sys


def main():
    result_rows = []

    with open(sys.argv[1], 'r') as f:
        for line in f:
            result_rows.append(line.strip())

    num_of_instances = len(result_rows)

    for file_name in sys.argv[2:]:
        line_count = 0
        with open(file_name, 'r') as f:
            for line in f:
                arr = line.strip().split(' ')
                result_rows[line_count] += ' ' + ' '.join(arr[1:])
                line_count += 1
                assert line_count <= num_of_instances
        assert line_count == num_of_instances

    for line in result_rows:
        print(line)


if __name__ == '__main__':
    main()
