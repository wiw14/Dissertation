import numpy as np
import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches

iterations = []
scores = []

def readFile(runNum):
    file = open("C:/Users/wmw13/Documents/Github/Dissertation/SampleCode/Data/RunData"+str(runNum)+".txt","r")
    for line in file:
        iteration, score = line.split(" ")
        iterations.append(int(iteration))
        scores.append(float(score))
    file.close()

def plotRunData(runNum):
    fig,ax = plt.subplots()
    plt.title("Run "+str(runNum))
    ax.scatter(iterations,scores)
    m, b = np.polyfit(iterations,scores,1)
    x = np.array(iterations)
    plt.plot(x, m*x+b)
    plt.draw()
    plt.show()

for x in range(1,21):
    readFile(x)
    plotRunData(x)
    scores = []
    iterations = []


