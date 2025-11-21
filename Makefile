# Makefile for ROOT Minuit2 fit example

CXX      = g++
CXXFLAGS = -O3 -std=c++17
ROOTCFLAGS = $(shell root-config --cflags)
ROOTLIBS   = $(shell root-config --libs) -lMinuit2

TARGET  = fit
SOURCES = fit.cpp

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(ROOTCFLAGS) $^ $(ROOTLIBS) -o $@

clean:
	rm -f $(TARGET) *.o

