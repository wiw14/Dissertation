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
optimums = []


def clearLists():
    csList.clear()
    depot.clear()
    customers.clear()
    allNodes.clear()


def readFile():
    clearLists()
    file = open("../SampleCode/Data/storeNodes.txt", "r")
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
    file = open("../SampleCode/Data/storeTour.txt", "r")
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

    #RANDOM PHEROMONE TWO OPT LOCAL SEARCH POPULATION
    #partitionOne = [0,1,2,5,7,9,13,11,4,3,6,8,10,17,20,18,15,12,16,19,21,14,0]
    partitionOne = [0, 31, 14, 17, 15, 1, 13, 11, 2, 30, 26, 29, 4, 5, 6, 7, 8, 9, 10, 32, 12, 3, 18, 19, 21, 22, 20, 23, 24, 16, 28, 27, 25,0]
    partitionTwo = [ 0, 1, 14, 31, 30, 13, 18, 19, 22, 21, 20, 23, 24, 27, 16, 28, 29, 15, 26, 25, 17, 10, 12, 3, 4, 5, 6, 7, 9, 8, 32, 11, 2, 0]
    #partitionOne = [50, 33, 81, 9, 51, 1, 69, 27, 31, 88, 62, 10, 32, 90, 63, 11, 64, 49, 19, 7, 52, 28, 76, 77, 3, 79, 0, 78, 34, 35, 71, 65, 66, 20, 30, 70, 12, 80, 68, 29, 24, 54, 55, 25, 4, 26, 87, 42, 43, 15, 57, 2, 58, 53, 89, 60, 83, 8, 45, 46, 36, 47, 48, 82, 18, 6, 94, 96, 99, 93, 85, 37, 59, 95, 13, 5, 84, 17, 61, 16, 86, 38, 14, 44, 91, 100, 98, 92, 97, 40, 73, 74, 22, 41, 75, 56, 23, 67, 39, 72, 21,50]
    #partitionTwo = [58, 73, 74, 22, 41, 23, 56, 75, 72, 21, 40, 1, 20, 66, 65, 71, 35, 34, 78, 79, 3, 77, 76, 28, 83, 8, 45, 46, 47, 36, 49, 64, 63, 90, 32, 10, 88, 31, 52, 7, 62, 11, 19, 48, 82, 18, 12, 80, 68, 29, 24, 55, 25, 67, 39, 4, 54, 26, 94, 59, 92, 37, 100, 98, 93, 99, 96, 13, 95, 97, 87, 42, 14, 43, 15, 57, 2, 53, 27, 69, 70, 30, 51, 9, 81, 33, 50, 6, 85, 91, 44, 38, 86, 16, 61, 17, 0, 84, 5, 60, 89, 58]
    partitionThree = [19, 22, 21, 20]
    partitionFour = [0, 31, 1, 25, 2, 30, 14, 15, 13, 26, 17, 27, 29, 18, 11, 10, 24, 16, 28, 4, 3, 32, 9, 12, 8, 7]
    #partitionTwo = [7, 5, 2, 1, 10, 17, 20, 18, 15, 12, 13, 0, 19, 21, 14, 16, 11, 4, 3, 6, 9, 8, 7]
    #partitionTwo = [0,12,8,6,1,2,5,7,9,14,21,19,16,13,11,4,3,10,17,20,18,15,0]
    #partitionThree = [15,0,12]
    #partitionFour = [6,1,2,5,7,9,14,21,19,16,13,11]
    #partitionFive = [3,10,17]

    #partitionSix = [15,12,16,19,21,14,0,1,2,5,7,9,13,11]
    #partitionSeven = [3,6]
    #partitionEight = [10,17]

    #partitionTwo = [0,16,17,20,18,15,12,6,1,2,5,7,9,13,11,4,2,8,10,14,21,19,0]

    #LK-Search POPULATION
    #partitionOne = [17, 20, 21, 19, 13, 11, 4, 3, 8, 10, 12, 15, 18, 14, 0, 16, 6, 1, 2, 5, 7, 9,17]
    #partitionTwo = [6, 1, 2, 5, 7, 9, 13, 11, 4, 3, 8, 10, 12, 0, 15, 18, 20, 17, 14, 21, 19, 16, 6]
    #partitionTwo = [11, 4, 3, 8, 10, 12, 13, 19, 16, 15, 18, 20, 21, 17, 14, 0, 9, 7, 5, 2, 1, 6,11]

    #RANDOM POPULATIONS
    #partitionOne = [11, 3, 0, 10, 14, 18, 9, 1, 7, 15, 12, 8, 4, 5, 6, 2, 13, 16, 21, 19, 20, 17, 11]
    #partitionTwo = [6, 1, 2, 5, 7, 9, 13, 11, 4, 3, 8, 10, 12, 0, 15, 18, 20, 17, 14, 21, 19, 16, 6]
    #partitionTwo = [15, 21, 20, 14, 16, 4, 1, 0, 18, 19, 13, 11, 2, 10, 8, 7, 5, 9, 12, 17, 3, 6,15]

# partitionOne = [0,4]
   # partitionTwo = [0,8]
    patchOne = createPath(partitionOne)
    patchA = createPath(partitionFour)
    patchB = createPath(partitionThree)

    patchTwo = createPath(partitionTwo)
    #patchC = createPath(partitionSeven)
    #patchD = createPath(partitionEight)


    patchOne.set_edgecolor(color='red')
    patchA.set_edgecolor(color='blue')
    patchB.set_edgecolor(color='yellow')
    #patchA.set_edgecolor(color='red')
    #patchB.set_edgecolor(color='red')


    patchTwo.set_linestyle('dashed')
    patchA.set_linestyle(':')
    patchB.set_linestyle('-.')
    #patchC.set_linestyle('dashed')
    #patchD.set_linestyle('dashed')

# ax.set_title('Run '+str(n))
    fig,ax = plt.subplots()

    ax.plot(csList[0], csList[1], "rs")
    ax.plot(depot[0], depot[1], "ys")
    ax.plot(customers[0], customers[1], "b.")

    ax.add_patch(patchOne)
    ax.add_patch(patchTwo)
    ax.add_patch(patchA)
    ax.add_patch(patchB)
    #ax.add_patch(patchC)
    #ax.add_patch(patchD)
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

        ax.plot(csList[0], csList[1], "rs")
        ax.plot(depot[0], depot[1], "ys")
        ax.plot(customers[0], customers[1], "b.")
        ax.add_patch(patch)
        plt.draw()

    plt.show()


readFile()
displayList()
# displayTemp()