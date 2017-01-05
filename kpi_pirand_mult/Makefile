# ----------------------------------------------------------------------
EXE            :=example
EXE_SRCS       :=$(wildcard *.cpp) $(wildcard *.knucpp)
SHLIB          :=
SHLIB_SRCS     :=
# Useful KNUCC Flags:
# -v                   Run in verbose mode to display individual steps
# -k                   Keep intermediate files for examination/debugging
# --show-disassembly   Show disassembly output during compilation
# --gen-disassembly    Generate '.os' disassembly files during compilation
# --gen-assembly       Generate .s assembly language files, which can
#                      later be processed by an assembler.
KNUCC_FLAGS    :=
# ----------------------------------------------------------------------
EXE_OBJS       :=$(addsuffix .o,$(basename $(EXE_SRCS)))
SHLIB_OBJS     :=$(addsuffix .o,$(basename $(SHLIB_SRCS)))
TEMPFILES      :=$(addsuffix .*, $(wildcard *.knuc))
KNUPATH_INSTALL:=/opt/knupath
KNUCC          :=$(KNUPATH_INSTALL)/bin/knucc
CPPFLAGS       :=-I$(KNUPATH_INSTALL)/include \
                 -I$(KNUPATH_INSTALL)/include/kpi

# Compile shared library sources with the position independent code
# flag
CXXFLAGS       :=-std=c++11 -fPIC 
LDFLAGS        :=-L$(KNUPATH_INSTALL)/lib -Wl,-rpath,$(KNUPATH_INSTALL)/lib -Wl,-rpath,$(shell pwd)
LDLIBS         :=-lprotobuf -lpthread -lkpirt

.PHONY: all clean

all: $(EXE) $(SHLIB)

run:
	./$(EXE)

clean:
	rm -rf $(EXE) $(SHLIB) $(EXE_OBJS) $(SHLIB_OBJS) $(TEMPFILES)

%.o : %.knuc
	$(KNUCC) $(KNUCC_FLAGS) -c -o $@ $< 

%.o : %.knucpp
	$(KNUCC) $(KNUCC_FLAGS) -c -o $@ $< 

%.o : %.knucxx
	$(KNUCC) $(KNUCC_FLAGS) -c -o $@ $<

$(SHLIB) : $(SHLIB_OBJS)
	$(CXX) $(LDFLAGS) -shared -o $@ $^ $(LDLIBS) 

$(EXE) : $(EXE_OBJS) $(SHLIB)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS) 
