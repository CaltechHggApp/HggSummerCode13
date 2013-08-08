# macro to create text that looks like this:
# chain->add("<path to file 1>");
# chain->add("<path to file 2>");
#
# I copy and paste it into an analysis script
# To run: python files.py

import os

outputFile = open('chain.txt', 'w')

inputFile = open('/home/amott/HggApp/Hgg_53X/Caltech/Graviton2PMGluGluToHToGG_M-125_8TeV-jhu-pythia6__Summer12_DR53X-PU_S10_START53_V7C-v1.list', 'r')

#directory = "/mnt/tier2/store/user/amott/Vecbos2012/MC/V00-5_3_X/GluGluToHToGG_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/"
#files = os.listdir(directory)
#for file in files:

for file in inputFile:
    line = '         chain->Add(directory + \"' + file[140:-1] + '\");\n'
    outputFile.write(line)

outputFile.close()
inputFile.close()
print "done"
