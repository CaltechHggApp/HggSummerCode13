all: runSpin

runSpin: mainSpin.o
	g++ -o runSpin `root-config --libs` mainSpin.o

mainSpin.o: mainSpin.cc librarySpin.h analyzerSpin.cc analyzerSpin.h
	g++ -c -I`root-config --incdir` `root-config --libs` mainSpin.cc

clean:
	rm mainSpin.o