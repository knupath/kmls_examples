# ----------------------------------------------------------------------
# KPI Makefile Template
# ----------------------------------------------------------------------
# Runtime Files
EXE            := example
# Grab all files (Default)
SRCS       		 :=$(wildcard *.cpp) $(wildcard *.knucpp)
# Specify source files
# SRCS           := main.cpp kernel.knucpp
#
# EXE = Generated executables name
# SRCS = Source files
# ----------------------------------------------------------------------

OBJS           :=$(addsuffix .o,$(basename $(SRCS)))
TEMPFILES      :=$(addsuffix .*, $(wildcard *.knuc))

KNUPATH_INSTALL?=$(KNU_PATH)
KNUCC          :=$(KNUPATH_INSTALL)/bin/knucc
CPPFLAGS       :=-I$(KNUPATH_INSTALL)/include \
                 -I$(KNUPATH_INSTALL)/include/kpi
CXXFLAGS       :=-std=c++11
LDFLAGS        :=-L$(KNUPATH_INSTALL)/lib -Wl,-rpath,$(KNUPATH_INSTALL)/lib
LDLIBS         :=-lkpirt -lism -llfi

.PHONY: all run clean

all: $(EXE)

run: all
	./$(EXE)

clean:
	rm -rf $(EXE) $(OBJS) $(TEMPFILES)

%.o : %.knuc
	$(KNUCC) -c -o $@ $<

%.o : %.knucpp
	$(KNUCC) -c -o $@ $<

%.o : %.knucxx
	$(KNUCC) -c -o $@ $<

$(EXE) : $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)
