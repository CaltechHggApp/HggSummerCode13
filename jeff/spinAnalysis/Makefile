ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs) -lMLP -lXMLIO -lTMVA

ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs) -lTMVA -lMLP -lXMLIO

ROOFITLIBS    = -L$(ROOFITSYS)/lib -lRooFitCore -lRooFit -lRooStats -lMinuit -lFoam

CXX           = g++ -m64

#CXXFLAGS      = -Wall
CXXFLAGS      =  

CXXFLAGS      += $(ROOTCFLAGS)
CXXFLAGS      += $(FASTJETFLAGS)

NGLIBS         = $(ROOTGLIBS) 

NGLIBS        += -lMinuit -lMLP -lTreePlayer
GLIBS          = $(filter-out -lNew, $(NGLIBS))

INCLUDEDIR       = ./include/
INCLUDEDIRCOMMON = ./
INCLUDEDIRROOFIT = $(ROOFITSYS)/include/

CXX	         += -I$(INCLUDEDIR) -I$(INCLUDEDIRCOMMON) -I. -I$(INCLUDEDIRROOFIT)


run: src/mainSpin.cc src/* include/* config.cfg 
	$(CXX) $(CXXFLAGS) $(ROOFITLIBS) -o run  $(GLIBS) src/mainSpin.cc	


