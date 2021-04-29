import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches
import pandas as pd

#======================================================================#
# Visualises the benchmark datasets with no route.
#======================================================================#

#Reads the benchmark datasets for plotting.
def readFile(file):
    file = open("C:/Users/wmw13/Documents/GitHub/Dissertation/evrp-benchmark-set/"+file, "r")
    customers = pd.DataFrame(columns={"x","y"})
    depot = pd.DataFrame(columns={"x","y"})
    charging = pd.DataFrame(columns={"x","y"})

    read = 0
    customerCounter = 0
    for line in file:
        if(line.startswith("DIMENSION")):
            sLine = line.split(" ")
            customerCounter = int(sLine[1])
        elif(line.rstrip() == "NODE_COORD_SECTION"):
            read = 1
        elif(line.rstrip() == "DEMAND_SECTION"):
            break
        elif(read == 1):
            sLine = line.split(" ")
            if(int(sLine[0]) == 1):
                depot = depot.append({"x":int(sLine[1]),"y":int(sLine[2])},ignore_index=True)
            elif(customerCounter > 1):
                customers = customers.append({"x":int(sLine[1]),"y":int(sLine[2])},ignore_index=True)
                customerCounter -=1
            else:
                charging = charging.append({"x":int(sLine[1]),"y":int(sLine[2])},ignore_index=True)

    file.close()
    print(depot)
    print(customers)
    print(charging)

    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    ax.plot(charging['x'], charging['y'], "rs")
    ax.plot(depot['x'], depot['y'], "ys")
    ax.plot(customers['x'], customers['y'], "b.")
    plt.draw()
    plt.show()

#Displays a command line menu for benchmark selection.
def displayMenu(files):
    print("Select a benchmark to visualise:")
    count = 1
    for f in files:
        print(str(count)+". "+f)
        count +=1
    print("Q. Quit")

#List of all available benchmarks.
files = ["E-n22-k4.evrp","E-n23-k3.evrp","E-n30-k3.evrp","E-n33-k4.evrp","E-n51-k5.evrp","E-n76-k7.evrp","E-n101-k8.evrp","X-n143-k7.evrp","X-n214-k11.evrp","X-n351-k40.evrp","X-n459-k26.evrp","X-n573-k30.evrp","X-n685-k75.evrp","X-n749-k98.evrp","X-n819-k171.evrp","X-n916-k207.evrp","X-n1001-k43.evrp"]

#Main body of the program.
while True:
    displayMenu(files)
    userInput = input("-:")
    if(userInput.upper() == 'Q'):
        break
    readFile(files[int(userInput)-1])
