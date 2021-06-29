import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches
import re

#======================================================================#
# Visualises the routes produced by the metaheuristic approaches.
#======================================================================#

#Creates all the required lists.
allNodes = []
csList = []
depot = []
customers = []
nodes = []
numSolutions = 0
optimums = []

#Empties the lists.
def clearLists():
    csList.clear()
    depot.clear()
    customers.clear()
    allNodes.clear()

#Reads the nodes from a file for plotting.
def readFile():
    clearLists()
    file = open("../Technical Work/Data/storeNodes.txt", "r")
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

#Creates the route for plotting based upon a list of customers.
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

#Reads the tour from a file for visualisation.
def readTour():
    file = open("../Technical Work/Data/storeTour.txt", "r")
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

#Visualises the nodes and a route on top of the nodes
def displayList():
    plt.close()
    nodes = readTour()

    #Generates individual graphs for each run.
    #Uncomment to use.
    # for node in nodes:
    #     patch = createPath(node)
    #     fig2, ax2 = plt.subplots()
    #     ax2.plot(customers[0], customers[1], "b.")
    #     ax2.plot(csList[0], csList[1], "rs")
    #     ax2.plot(depot[0], depot[1], "ys")
    #     ax2.add_patch(patch)
    #     plt.draw()

    #Generates a single plot of all 20 runs, utilises sub-plots.
    fig = plt.figure()
    n = 1
    for node in nodes:
        node.append(0)
        patch = createPath(node)
        # ax.set_title('Run '+str(n))
        ax = fig.add_subplot(4, 5, n)
        n += 1

        ax.plot(csList[0], csList[1], "rs")
        ax.plot(depot[0], depot[1], "ys")
        ax.plot(customers[0], customers[1], "b.")
        ax.add_patch(patch)
        plt.draw()

    plt.show()

#Main body of the program.
readFile()
displayList()
