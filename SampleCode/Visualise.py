import sys
import time
import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches
import networkx as nx
import numpy as np
import re

allNodes = []
csList = []
depot = []
customers = []


def clearLists():
    csList.clear()
    depot.clear()
    customers.clear()
    allNodes.clear()


def readFile():
    clearLists()
    file = open("C:\\Users\\wmw13\\Documents\\GitHub\\Dissertation\\SampleCode\\jsonNodes.json", "r")
    csX = []
    csY = []
    cX = []
    cY = []
    for line in file:
        val = re.split(" |\n",line)
        # print("node " + str(val[0]) + " at " + str(val[1]) + "," +str(val[2]))
        allNodes.append([val[1],val[2]])
        if val[3] == 'c':
            cX.append(float(val[1]))
            cY.append(float(val[2]))
        elif val[3] == 's':
            csX.append(float(val[1]))
            csY.append(float(val[2]))
        elif val[3] == 'd':
            depot.append(float(val[1]))
            depot.append(float(val[2]))
    csList.append(csX)
    csList.append(csY)
    customers.append(cX)
    customers.append(cY)


def displayList():
    plt.close()
    verts = [
        (145.,215.),
        (128.,252.),
        (148.,232.),
        (137.,193.),
        (145.,215.),
    ]
    codes = [
        Path.MOVETO,
        Path.LINETO,
        Path.LINETO,
        Path.LINETO,
        Path.LINETO,
    ]

    path = Path(verts, codes)

    patch = patches.PathPatch(path, facecolor='orange', lw=1,fill=False)

    fig,ax = plt.subplots()
    ax.plot(customers[0],customers[1],"b.")
    ax.plot(csList[0],csList[1],"rs")
    ax.plot(depot[0],depot[1],"ks")
    ax.add_patch(patch)
    plt.pause(0.5)


while True:
    readFile()
    displayList()
    print("..")