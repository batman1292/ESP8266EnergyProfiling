import argparse
import sys, os, time, datetime
import numpy as np
from matplotlib import pyplot as plt
import fnmatch

statePowers = []
stateCurrents = []
stateVoltages = []
stateTimes = []
stateTime = []
stateIndex = 0
numState = 4
numSample = 5
filename = "UDPStaticModemSleep.txt"

for file in os.listdir("./"):
    if fnmatch.fnmatch(file, filename):
        f = open("./" + file, "r")
        for line in f:
            if line != '\n':
                if len(line.split('\t')) == 1:
                    if stateIndex != 0:
                        statePowers.append([[ele // numSample for ele in state] for state in statePower])
                        stateCurrents.append([[ele // numSample for ele in state] for state in stateCurrent])
                        stateVoltages.append([[ele // numSample for ele in state] for state in stateVoltage])
                        stateTimes.append([ele // numSample for ele in stateTime])
                    statePower = [ [0 for y in range(3)] for x in range(numState) ]
                    stateCurrent = [ [0 for y in range(3)] for x in range(numState) ]
                    stateVoltage = [ [0 for y in range(3)] for x in range(numState) ]
                    stateTime = [ 0 for y in range(numState) ]
                else:
                    for i in range(3):
                        statePower[stateIndex%numState][i] += float(line.split('\t')[1].split(',')[i])
                        stateCurrent[stateIndex%numState][i] += float(line.split('\t')[2].split(',')[i])
                        stateVoltage[stateIndex%numState][i] += float(line.split('\t')[3].split(',')[i])
                    stateTime[stateIndex%numState] += float(line.split('\t')[4].split(',')[0])
                    stateIndex += 1

ticks = ['State1', 'State2', 'State3', 'State4']
colorList = ['#D98880', '#C39BD3', '#85C1E9', '#7DCEA0', '#F8C471']
labelList = ['Tx0dbm', 'Tx5dbm', 'Tx10dbm','Tx15dbm','Tx20dbm']

def set_box_color(bp, color):
    plt.setp(bp['boxes'], color=color)
    plt.setp(bp['whiskers'], color=color)
    plt.setp(bp['caps'], color=color)
    plt.setp(bp['medians'], color=color)

# plt.figure()
# print np.array(xrange(len(data_b)))*0.6+0.8

countList = 0
# pltPower = plt.subplot(311)
for state in stateVoltages:
    posIndex = np.array(xrange(len(state)))
    # print np.array(xrange(len(state)))+4*countList
    print posIndex*0.6+0.8
    bpr = plt.boxplot(state, positions=posIndex*4.0+(0.6*(countList-2)), sym='', widths=0.4)
    set_box_color(bpr, colorList[countList])
    plt.plot([], c=colorList[countList], label=labelList[countList])
    # pltPower.legend()
    # pltPower.xticks(xrange(0, len(ticks) * 4, 4), ticks)
    # pltPower.xlim(-4, len(ticks)*4)
    countList += 1

# countList = 0
# pltCurrent = plt.subplot(312)
# for state in stateCurrents:
#     posIndex = np.array(xrange(len(state)))
#     # print np.array(xrange(len(state)))+4*countList
#     print posIndex*0.6+0.8
#     bpr = pltCurrent.boxplot(state, positions=posIndex*4.0+(0.6*(countList-2)), sym='', widths=0.4)
#     set_box_color(bpr, colorList[countList])
#     pltCurrent.plot([], c=colorList[countList], label=labelList[countList])
#     # pltCurrent.legend()
#     # pltCurrent.xticks(xrange(0, len(ticks) * 4, 4), ticks)
#     # pltCurrent.xlim(-4, len(ticks)*4)
#     countList += 1

# bpl = plt.boxplot(statePowers[0], positions=np.array(xrange(len(statePowers[0])))*2.0-0.4, sym='', widths=0.6)
# bpr = plt.boxplot(data_b, positions=np.array(xrange(len(data_b)))*2.0+0.4, sym='', widths=0.6)
# set_box_color(bpl, '#D7191C') # colors are from http://colorbrewer2.org/
# set_box_color(bpr, '#2C7BB6')
#
# # draw temporary red and blue lines and use them to create a legend
# plt.plot([], c='#D7191C', label='Apples')
# plt.plot([], c='#2C7BB6', label='Oranges')
plt.legend()

plt.xticks(xrange(0, len(ticks) * 4, 4), ticks)
plt.xlim(-4, len(ticks)*4)
# plt.ylim(0, 8)
# plt.tight_layout()
plt.show()
