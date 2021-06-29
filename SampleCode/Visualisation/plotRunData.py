import numpy as np
import matplotlib.pyplot as plt
import os

#Creates relevant lists.
iterations = []
scores = []

#Reads the individual run data from the file for visualisation.
def readFile(runNum):
    directory = os.path.realpath('..')
    file = open(os.path.join(directory,"Data","RunData"+str(runNum)+".txt"),"r")
    for line in file:
        iteration, score = line.split(" ")
        iterations.append(int(iteration))
        scores.append(float(score))
    file.close()

#Plots the run data and creates regression line to show general trend.
#Creates a single graph representing one run.
def plotRunData(runNum):
    minVal = min(scores)
    minIter =[]
    minScores = []
    for s in range(0,len(scores)):
        if(scores[s] == minVal):
            minIter.append(iterations[s])
            minScores.append(scores[s])
    fig,ax = plt.subplots()
    plt.title("Run "+str(runNum))
    ax.scatter(iterations,scores)
    ax.scatter(minIter,minScores)
    print("MIN SCORE: "+str(minVal))
    m, b = np.polyfit(iterations,scores,1)
    x = np.array(iterations)
    plt.plot(x, m*x+b,'k')
    plt.draw()
    plt.show()

#Creates 20 sub-plots representing all the runs.
def plotAllRunData():
    global scores, iterations
    fig = plt.figure()
    for x in range(1,21):
        readFile(x)
        ax = fig.add_subplot(4, 5, x)
        minVal = min(scores)
        minIter =[]
        minScores = []
        for s in range(0,len(scores)):
            if(scores[s] == minVal):
                minIter.append(iterations[s])
                minScores.append(scores[s])

        ax.scatter(iterations,scores)
        ax.scatter(minIter,minScores)
        m, b = np.polyfit(iterations,scores,1)
        x = np.array(iterations)
        plt.plot(x, m*x+b,'k')
        plt.draw()
        scores = []
        iterations = []
    plt.show()

#Main body of the program.
plotAllRunData()

