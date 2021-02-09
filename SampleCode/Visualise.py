import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches
import re

allNodes = []
csList = []
depot = []
customers = []
nodes = []
numSolutions = 0


def clearLists():
    csList.clear()
    depot.clear()
    customers.clear()
    allNodes.clear()


def readFile():
    clearLists()
    file = open("C:\\Users\\wmw13\\Documents\\GitHub\\Dissertation\\SampleCode\\storeNodes.txt", "r")
    csX = []
    csY = []
    cX = []
    cY = []
    for line in file:
        val = re.split(" |\n", line)
        allNodes.append([val[1], val[2]])
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


def createPath(nodes):
    verts = []
    for node in nodes:
        verts.append((allNodes[node][0], allNodes[node][1]))
    codes = [Path.MOVETO]
    for x in range(1, len(nodes)):
        codes.append(Path.LINETO)
    path = Path(verts, codes)
    patch = patches.PathPatch(path, facecolor='orange', lw=1, fill=False)

    return patch


def readTour():
    file = open("C:\\Users\\wmw13\\Documents\\GitHub\\Dissertation\\SampleCode\\storeTour.txt", "r")
    nodes = []
    temp = []
    for line in file:
        # print(line);
        if line.split(" ")[0] == '-':
            numSolutions = int(line.split(" ")[1])
            nodes.append(temp[:])
            temp.clear()
        else:
            temp.append(int(line))
    print(numSolutions)
    return nodes


def displayTemp():
    node = [0,14,16,12,15,17,13,18,19,20,21,11,8,10,9,7,5,6,2,1,3,4] #Dijkstra path
    #node = [0,8,6,1,9,4,3,5,7,15,16,10,11,2,19,21,20,18,14,17,12,13]
    patch = createPath(node)
    # ax.set_title('Run '+str(n))
    fig,ax = plt.subplots()
    ax.plot(customers[0], customers[1], "b.")
    ax.plot(csList[0], csList[1], "rs")
    ax.plot(depot[0], depot[1], "ys")
    ax.add_patch(patch)
    plt.draw()

    plt.show()


def displayList():
    plt.close()
    # nodes = [0,11,15,12,29,17,22,13,0,19,29,9,5,6,0,3,29,21,27,14,1,24,10,18,26,8,0,16,4,29,2,23,20,0]
    nodes = readTour()

    # Generates individual maps for each run.
    # for node in nodes:
    #     patch = createPath(node)
    #     fig2, ax2 = plt.subplots()
    #     ax2.plot(customers[0], customers[1], "b.")
    #     ax2.plot(csList[0], csList[1], "rs")
    #     ax2.plot(depot[0], depot[1], "ys")
    #     ax2.add_patch(patch)
    #     plt.draw()
    fig = plt.figure()
    n = 1
    for node in nodes:
        node.append(0)
        patch = createPath(node)
        # ax.set_title('Run '+str(n))
        ax = fig.add_subplot(4, 5, n)
        n += 1
        ax.plot(customers[0], customers[1], "b.")
        ax.plot(csList[0], csList[1], "rs")
        ax.plot(depot[0], depot[1], "ys")
        ax.add_patch(patch)
        plt.draw()

    plt.show()


readFile()
#displayList()
displayTemp()