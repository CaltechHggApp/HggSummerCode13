# macro to create text that looks like this:
# chain->add("<path to file 1>");
# chain->add("<path to file 2>");
#
# I usually copy and paste it into an analysis script
# To run: python files.py

import os

outputFile = open('chain.txt', 'w')

inputFile = open('/home/amott/tmp_data/GluGluToHToGG_M-125_14TeV-powheg-pythia6_Summer12-PU50_POSTLS161_V12-v1.list', 'r')
#directory = "/mnt/tier2/store/user/amott/Vecbos2012/MC/V00-5_3_X/GluGluToHToGG_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/"

#files = os.listdir(directory)

for file in inputFile:
    line = 'chain->Add(\"' + file[:-1] + '\");\n'
    outputFile.write(line)

outputFile.close()
inputFile.close()
print "done"
