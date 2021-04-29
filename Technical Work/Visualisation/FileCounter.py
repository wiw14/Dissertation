import os

lineCount = 0
#Set starting directory.
directory = "C:/Users/wmw13/Documents/GitHub/Dissertation/Technical Work"
#Create list of directories not included in the count.
nonValidDir = {".idea","cmake-build-debug","Framework","venv","CMakeLists.txt","Data","main.cpp"}

#Loop through all the directories.
for filename in os.listdir(directory):
    check = 0
    for a in nonValidDir:
        if(filename == a):
            check = 1
            break
    #If check is false then the sub-directory is valid.
    #Loop through all the files for counting.
    if(check == 0):
        print("=== " + filename + " ===")
        for file in os.listdir(os.path.join(directory,filename)):
            lc = 0
            f = open(os.path.join(os.path.join(directory,filename),file),"r")
            for l in f:
                lc += 1
            f.close()
            print(file + " has : " + str(lc))
            lineCount += lc
        print("")
print("Total lines written : " + str(lineCount))

