# ----------------------------------------------------------------------
EXE            :=kpi_getrank
SRCS           :=kpi_getrank.cpp getrank.knucpp
# Useful KNUCC Flags:
# -v                   Run in verbose mode to display individual steps
# -k                   Keep intermediate files for examination/debugging
# --show-disassembly   Show disassembly output during compilation
# --gen-disassembly    Generate '.os' disassembly files during compilation
# --gen-assembly       Generate .s assembly language files, which can
#                      later be processed by an assembler.
KNUCC_FLAGS    :=
# ----------------------------------------------------------------------
OBJS           :=$(addsuffix .o,$(basename $(SRCS)))
TEMPFILES      :=$(addsuffix .*, $(wildcard *.knuc))
KNUPATH_INSTALL:=/opt/knupath
KNUCC          :=$(KNUPATH_INSTALL)/bin/knucc
CPPFLAGS       :=-I$(KNUPATH_INSTALL)/include \
                 -I$(KNUPATH_INSTALL)/include/kpi
CXXFLAGS       :=-std=c++11
LDFLAGS        :=-L$(KNUPATH_INSTALL)/lib -Wl,-rpath,$(KNUPATH_INSTALL)/lib
LDLIBS         :=-lkpirt -lism -llfi

.PHONY: all clean

all: $(EXE)

clean:
	rm -rf $(EXE) $(OBJS) $(TEMPFILES)

%.o : %.knuc
	$(KNUCC) $(KNUCC_FLAGS) -c -o $@ $< 

%.o : %.knucpp
	$(KNUCC) $(KNUCC_FLAGS) -c -o $@ $< 

%.o : %.knucxx
	$(KNUCC) $(KNUCC_FLAGS) -c -o $@ $<

$(EXE) : $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS) 
