SHELL=/usr/bin/env bash

-include Makefile.inc

CXX_COMMON:=-I$(PREFIX_INCLUDE) $(CXX_COMMON) $(GZIP_LIB)
CXX_COMMON+= -L$(PREFIX_LIB) -Wl,-rpath,$(PREFIX_LIB) -lpythia8 -ldl
PYTHIA=$(PREFIX_LIB)/libpythia8$(LIB_SUFFIX)
FILENAME=gen_events

.SECONDEXPANSION:
.PHONY: all clean

all:
	$(info Usage: make mainXX)

$(PYTHIA):
	$(error Error: PYTHIA must be built, please run "make"\
                in the top PYTHIA directory)

$(FILENAME).so: $(FILENAME)Dct.cc
	$(CXX) $< -o $@ -w $(CXX_SHARED) $(CXX_COMMON)\
	 $(ROOT_LIB) `$(ROOT_CONFIG) --cflags`
	
$(FILENAME)Dct.cc: $(FILENAME).h $(FILENAME)LinkDef.h
ifeq ($(ROOT_USE),true)
	$(ROOT_BIN)rootcint -f $@ -I$(PREFIX_INCLUDE) $^
else
	$(error Error: $@ requires ROOT)
endif

$(FILENAME): $(PYTHIA) $$@.cc
ifeq ($(ROOT_USE),true)
	$(CXX) $@.cc -o $@ -w $(CXX_COMMON)  \
	 `$(ROOT_CONFIG) --cflags --glibs`
else
	$(error Error: $@ requires ROOT)
endif

clean:
	@rm -f main[0-9][0-9]; rm -f out[0-9][0-9];\
	rm -f main[0-9][0-9][0-9]; rm -f out[0-9][0-9][0-9];\
	rm -f mymain[0-9][0-9]; rm -f myout[0-9][0-9];\
	rm -f test[0-9][0-9][0-9]; rm -f *.dat;\
	rm -f weakbosons.lhe; rm -f hist.root;\
	rm -f *~; rm -f \#*; rm -f core*; rm -f *Dct.*; rm -f *.so;\
	rm -f *.log; rm -f *plot.py; rm -f *.pcm;
