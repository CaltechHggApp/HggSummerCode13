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

HGGAPPINC        = ./include/
SRCDIR           = ./src/
EXESRCDIR        = ./exesrc/
CXX	         += -I$(INCLUDEDIR) -I$(INCLUDEDIRCOMMON) -I. -I$(INCLUDEDIRROOFIT)
OUTLIB   	= ./lib/

all: runMC makePlots

clean:
	rm -f $(OUTLIB)*.o
	rm -f runMC
	rm -f makePlots



runMC:          $(EXESRCDIR)runMC.cc \
                $(OUTLIB)analyzerSpin.o \
		$(HGGAPPINC)ReadConfig.hh \
		$(OUTLIB)librarySpin.o
	$(CXX) $(CXXFLAGS) $(ROOFITLIBS) -I$(HGGAPPINC) -I$(INCLUDEDIR) -o runMC $(OUTLIB)/*.o $(GLIBS) $<


makePlots:	$(EXESRCDIR)makePlots.cc \
		$(OUTLIB)class2.o \
		$(OUTLIB)plotter.o \
		$(HGGAPPINC)ReadConfig.hh \
		$(OUTLIB)MakeSpinSPlot.o
	$(CXX) $(CXXFLAGS) $(ROOFITLIBS) -I$(HGGAPPINC) -I$(INCLUDEDIR) -o makePlots $(OUTLIB)/*.o $(GLIBS) $<


$(OUTLIB)analyzerSpin.o:	$(SRCDIR)analyzerSpin.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)analyzerSpin.o $<


$(OUTLIB)librarySpin.o: $(SRCDIR)librarySpin.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)librarySpin.o $<


$(OUTLIB)class2.o: $(SRCDIR)class2.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)class2.o $<

$(OUTLIB)plotter.o: $(SRCDIR)plotter.cc
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)plotter.o $<


$(OUTLIB)MakeSpinSPlot.o: $(SRCDIR)MakeSpinSPlot.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)MakeSpinSPlot.o $<

