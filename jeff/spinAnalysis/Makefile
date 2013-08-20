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

all: runMC runToys runPlots

clean:
	rm -f $(OUTLIB)*.o
	rm -f runMC
	rm -f runToys
	rm -f runPlots


runMC:          $(EXESRCDIR)runMC.C \
                $(OUTLIB)AnalyzeMC.o \
		$(HGGAPPINC)ReadConfig.hh \
		$(OUTLIB)spinLibrary.o
	$(CXX) $(CXXFLAGS) $(ROOFITLIBS) -I$(HGGAPPINC) -I$(INCLUDEDIR) -o runMC $(OUTLIB)/*.o $(GLIBS) $<


runToys:	$(EXESRCDIR)runToys.C \
		$(OUTLIB)AnalyzeToy.o \
		$(OUTLIB)MakeToys.o \
		$(HGGAPPINC)ReadConfig.hh \
		$(OUTLIB)MakeSpinSPlot.o
	$(CXX) $(CXXFLAGS) $(ROOFITLIBS) -I$(HGGAPPINC) -I$(INCLUDEDIR) -o runToys $(OUTLIB)/*.o $(GLIBS) $<


runPlots:	$(EXESRCDIR)runPlots.C \
		$(OUTLIB)MakePlots.o \
		$(HGGAPPINC)ReadConfig.hh
	$(CXX) $(CXXFLAGS) $(ROOFITLIBS) -I$(HGGAPPINC) -I$(INCLUDEDIR) -o runPlots $(OUTLIB)/*.o $(GLIBS) $<





$(OUTLIB)AnalyzeMC.o:	$(SRCDIR)AnalyzeMC.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)AnalyzeMC.o $<

$(OUTLIB)spinLibrary.o: $(SRCDIR)spinLibrary.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)spinLibrary.o $<

$(OUTLIB)AnalyzeToy.o: $(SRCDIR)AnalyzeToy.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)AnalyzeToy.o $<

$(OUTLIB)MakeToys.o: $(SRCDIR)MakeToys.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)MakeToys.o $<

$(OUTLIB)MakeSpinSPlot.o: $(SRCDIR)MakeSpinSPlot.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)MakeSpinSPlot.o $<

$(OUTLIB)MakePlots.o: $(SRCDIR)MakePlots.C
	$(CXX) $(CXXFLAGS) -c -I$(INCLUDEDIR) -o $(OUTLIB)MakePlots.o $<