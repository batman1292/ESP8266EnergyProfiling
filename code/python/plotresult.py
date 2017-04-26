import argparse
import sys, os, time, datetime
import numpy as np
from matplotlib import pyplot as plt
import fnmatch

ap = argparse.ArgumentParser()
ap.add_argument(
    "-F", "--file", default="",
    help="MQTT host to connect to")
ap.add_argument(
    "-M", "--mode", default="",
    help="MQTT host to connect to")
args = vars(ap.parse_args())

statePowers = []
stateCurrents = []
stateVoltages = []
stateTimes = []
stateTime = []
stateIndex = 0
numState = 4
numSample = 5
# filename = "UDPStaticModemSleep.txt"
# filename = "UDPDHCPModemSleep.txt"
filename = args['file']

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

ticks = ['State1 \n Connect WiFi', 'State2 \n Send UDP data', 'State3 \n Disconnect WiFi', 'State4 \n Sleep Mode']
colorList = ['#D98880', '#C39BD3', '#85C1E9', '#7DCEA0', '#F8C471']
labelList = ['Tx0dbm', 'Tx5dbm', 'Tx10dbm','Tx15dbm','Tx20dbm']

def set_box_color(bp, color):
    plt.setp(bp['boxes'], color=color)
    plt.setp(bp['whiskers'], color=color)
    plt.setp(bp['caps'], color=color)
    plt.setp(bp['medians'], color=color)

# def autolabel(rects):
#     """
#     Attach a text label above each bar displaying its height
#     """
#     for rect in rects:
#         height = rect.get_height()
#         ax.text(rect.get_x() + rect.get_width()/2., 1*height,
#                 '%d' % int(height),
#                 ha='center', va='bottom')
print len(statePowers)
###########################################
########## plot and save power ##########
###########################################
countList = 0
fig = plt.figure(figsize=(12, 12))
plt80MHz = plt.subplot(211)
for i in range(5):
    state = statePowers[i]
    posIndex = np.array(xrange(len(state)))
    bpr = plt80MHz.boxplot(state, positions=posIndex*4.0+(0.6*(countList-2)), widths=0.4)
    set_box_color(bpr, colorList[countList])
    plt80MHz.plot([], c=colorList[countList], label=labelList[countList])
    countList += 1
plt80MHz.set_title('CPU Frequency 80 MHz', loc='left')
plt80MHz.set_xlabel('')
plt80MHz.set_ylabel('Power(mW)')
plt.legend(prop={'size':12}, loc='lower center')
plt.xticks(xrange(0, len(ticks) * 4, 4), ticks)
plt.xlim(-2, len(ticks)*4-2)
countList = 0
plt160MHz = plt.subplot(212)
for i in range(5, 10):
    state = statePowers[i]
    posIndex = np.array(xrange(len(state)))
    bpr = plt160MHz.boxplot(state, positions=posIndex*4.0+(0.6*(countList-2)), sym='', widths=0.4)
    set_box_color(bpr, colorList[countList])
    plt160MHz.plot([], c=colorList[countList], label=labelList[countList])
    countList += 1
plt160MHz.set_title('CPU Frequency 160 MHz', loc='left')
plt160MHz.set_xlabel('')
plt160MHz.set_ylabel('Power(mW)')
plt.legend(prop={'size':12}, loc='lower center')
plt.xticks(xrange(0, len(ticks) * 4, 4), ticks)
plt.xlim(-2, len(ticks)*4-2)
plt.savefig(filename.split('.')[0]+'Power.png')

###########################################
########## plot and save current ##########
###########################################
countList = 0
fig = plt.figure(figsize=(12, 12))
plt80MHz = plt.subplot(211)
for i in range(5):
    state = stateCurrents[i]
    posIndex = np.array(xrange(len(state)))
    bpr = plt80MHz.boxplot(state, positions=posIndex*4.0+(0.6*(countList-2)), widths=0.4)
    set_box_color(bpr, colorList[countList])
    plt80MHz.plot([], c=colorList[countList], label=labelList[countList])
    countList += 1
plt80MHz.set_title('CPU Frequency 80 MHz', loc='left')
plt80MHz.set_xlabel('')
plt80MHz.set_ylabel('Current(mA)')
plt.legend(prop={'size':12}, loc='lower center')
plt.xticks(xrange(0, len(ticks) * 4, 4), ticks)
plt.xlim(-2, len(ticks)*4-2)
countList = 0
plt160MHz = plt.subplot(212)
for i in range(5, 10):
    state = stateCurrents[i]
    posIndex = np.array(xrange(len(state)))
    bpr = plt160MHz.boxplot(state, positions=posIndex*4.0+(0.6*(countList-2)), sym='', widths=0.4)
    set_box_color(bpr, colorList[countList])
    plt160MHz.plot([], c=colorList[countList], label=labelList[countList])
    countList += 1
plt160MHz.set_title('CPU Frequency 160 MHz', loc='left')
plt160MHz.set_xlabel('')
plt160MHz.set_ylabel('Current(mA)')
plt.legend(prop={'size':12}, loc='lower center')
plt.xticks(xrange(0, len(ticks) * 4, 4), ticks)
plt.xlim(-2, len(ticks)*4-2)
plt.savefig(filename.split('.')[0]+'Current.png')

###########################################
########## plot and save voltage ##########
###########################################
countList = 0
fig = plt.figure(figsize=(12, 12))
plt80MHz = plt.subplot(211)
for i in range(5):
    state = stateVoltages[i]
    posIndex = np.array(xrange(len(state)))
    bpr = plt80MHz.boxplot(state, positions=posIndex*4.0+(0.6*(countList-2)), widths=0.4)
    set_box_color(bpr, colorList[countList])
    plt80MHz.plot([], c=colorList[countList], label=labelList[countList])
    countList += 1
plt80MHz.set_title('CPU Frequency 80 MHz', loc='left')
plt80MHz.set_xlabel('')
plt80MHz.set_ylabel('Voltage(mV)')
plt.legend(prop={'size':12}, loc='upper center')
plt.xticks(xrange(0, len(ticks) * 4, 4), ticks)
plt.xlim(-2, len(ticks)*4-2)
countList = 0
plt160MHz = plt.subplot(212)
for i in range(5, 10):
    state = stateVoltages[i]
    posIndex = np.array(xrange(len(state)))
    bpr = plt160MHz.boxplot(state, positions=posIndex*4.0+(0.6*(countList-2)), sym='', widths=0.4)
    set_box_color(bpr, colorList[countList])
    plt160MHz.plot([], c=colorList[countList], label=labelList[countList])
    countList += 1
plt160MHz.set_title('CPU Frequency 160 MHz', loc='left')
plt160MHz.set_xlabel('')
plt160MHz.set_ylabel('Voltage(mV)')
plt.legend(prop={'size':12}, loc='upper center')
plt.xticks(xrange(0, len(ticks) * 4, 4), ticks)
plt.xlim(-2, len(ticks)*4-2)
plt.savefig(filename.split('.')[0]+'Voltage.png')

###########################################
########## plot and save time ##########
###########################################
countList = 0
fig = plt.figure(figsize=(14, 14))
plt80MHz = plt.subplot(211)
for i in range(5):
    state = stateTimes[i]
    # print state
    posIndex = np.arange(4)
    bpr = plt80MHz.bar(posIndex*4.0+(0.6*(countList-2)), state, 0.4, color=colorList[countList])
    # autolabel(bpr)
    for rect in bpr:
        height = rect.get_height()
        plt80MHz.text((rect.get_x() + rect.get_width()/2.)+0.2, 1*height,
                '%d' % int(height),
                ha='center', va='bottom', size = 12, rotation=45)
    # set_box_color(bpr, colorList[countList])
    plt80MHz.plot([], c=colorList[countList], label=labelList[countList])
    countList += 1
plt80MHz.set_title('CPU Frequency 80 MHz', loc='left')
plt80MHz.set_xlabel('')
plt80MHz.set_ylabel('time(ms)')
plt.legend(prop={'size':12}, loc='upper left')
plt.xticks(xrange(0, len(ticks) * 4, 4), ticks)
plt.xlim(-2, len(ticks)*4-2)
plt.ylim(0, 15000)
# plt.show()
countList = 0
plt160MHz = plt.subplot(212)
for i in range(5, 10):
    state = stateTimes[i]
    posIndex = np.arange(4)
    bpr = plt160MHz.bar(posIndex*4.0+(0.6*(countList-2)), state, 0.4, color=colorList[countList])
    for rect in bpr:
        height = rect.get_height()
        plt160MHz.text((rect.get_x() + rect.get_width()/2.)+0.2, 1*height,
                '%d' % int(height),
                ha='center', va='bottom', size = 12, rotation=45)
    # bpr = plt160MHz.boxplot(state, positions=(i-5)*4.0+(0.6*(countList-2)), sym='', widths=0.4, color=colorList[countList])
    # set_box_color(bpr, colorList[countList])
    plt160MHz.plot([], c=colorList[countList], label=labelList[countList])
    countList += 1
plt160MHz.set_title('CPU Frequency 160 MHz', loc='left')
plt160MHz.set_xlabel('')
plt160MHz.set_ylabel('time(ms)')
plt.legend(prop={'size':12}, loc='upper left')
plt.xticks(xrange(0, len(ticks) * 4, 4), ticks)
plt.xlim(-2, len(ticks)*4-2)
plt.ylim(0, 15000)
plt.savefig(filename.split('.')[0]+'Time.png')
# plt.show()
