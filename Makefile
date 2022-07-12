CXXFLAGS=-fdiagnostics-color -O2 -g $(shell geant4-config --cflags)
LDLIBS=-Wl,--copy-dt-needed-entries -fdiagnostics-color -lm $(shell geant4-config --libs)
PWD=$(shell pwd)

PREFIX?=$(HOME)/local
INSTALL_BIN=$(PREFIX)/build
INSTALL=install

all: sim 

Makefile.dep:
	-$(CXX) $(CXXFLAGS) -MM *.cc > Makefile.dep

-include Makefile.dep

sim: action.o construction.o detector.o event.o generator.o physics.o run.o sim.o stepping.o tracking.o

geant4:
	test ! -f geant4-v11.0.2.tar.gz && \
	curl -O -L http://cern.ch/geant4-data/releases/geant4-v11.0.2.tar.gz && \
	tar -xzvf geant4-v11.0.2.tar.gz || true
	cd geant4-v11.0.2 && \
	mkdir -p geant4-build && \
	mkdir -p geant4-install && \
	cd geant4-build && \
	cmake -DCMAKE_INSTALL_PREFIX=../geant4-install .. && \
	make -j4 && \
	make install

root:
	test ! -f root_v6.26.04.source.tar.gz && \
	curl -O -L https://root.cern/download/root_v6.26.04.source.tar.gz && \
	tar -xzvf root_v6.26.04.source.tar.gz || true
	cd root-6.26.04 && \
	mkdir -p root-build && \
	mkdir -p root-install && \
	cd root-build && \
	cmake -DCMAKE_INSTALL_PREFIX=../root-install .. && \
	make -j4 && \
	make install


install-deps: geant4 root
	@echo "Make sure to add the line source $(PWD)/geant4-v11.0.2/geant4-install/bin/geant4.sh to your ~/.bashrc file!"
	@echo "Make sure to add the line source $(PWD)/root-6.26.04/root-install/bin/thisroot.sh to your ~/.bashrc file!"

install:
	@mkdir -p $(INSTALL_BIN)
	$(INSTALL) sim $(INSTALL_BIN)
	$(INSTALL) zdab-cat $(INSTALL_BIN)

test: 
	@echo $(LDLIBS)

clean:
	rm -f *.o 

.PHONY: all clean geant4 root install-deps install test
