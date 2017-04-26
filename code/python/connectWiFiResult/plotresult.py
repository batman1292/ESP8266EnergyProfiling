import argparse
import sys, os, time, datetime
import numpy as np
from matplotlib import pyplot as plt
import fnmatch

statePower = []
stateCurrent = []
stateVoltage = []
stateTime = []

print stateTime
filename = "Summary.txt"
for file in os.listdir("./"):
    if fnmatch.fnmatch(file, filename):
        f = open("./" + file, "r")
        for line in f:
            if line != '\n':
                print line
                print len(line.split('\t'))
                # pass
                # print line.split('\t')[1].split(',')
