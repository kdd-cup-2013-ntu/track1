import math


def avg(list_obj):
    return math.fsum(map(float, list_obj)) / len(list_obj)


def mean(list_obj):
    return avg(list_obj)


def median(list_obj):  # slow version # round-up
    return sorted(list_obj)[min(int((len(list_obj) + 1) / 2), len(list_obj) - 1)]

