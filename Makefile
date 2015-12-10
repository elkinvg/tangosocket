FLAGS = $(shell pkg-config --cflags tango)
LIBS = $(shell pkg-config --libs tango)
OBJDIR = ./obj
LIBDIR = ./lib
MKDIR = mkdir
TARGET = libtangosocket.a
TARGETLOC = $(LIBDIR)/$(TARGET)
CXX = g++
CXXFLAGS = $(FLAGS)

all: $(TARGETLOC)

$(TARGETLOC): $(OBJDIR)/tangosocket.o
	@if ! [ -d $(LIBDIR) ] ; then $(MKDIR) $(LIBDIR) ; fi
	ar cr $@ $(OBJDIR)/tangosocket.o 
	
$(OBJDIR)/tangosocket.o: tangosocket.cpp tangosocket.h
	@if ! [ -d $(OBJDIR) ] ; then $(MKDIR) $(OBJDIR) ; fi
	$(CXX) -c $(CXXFLAGS) -o $@ $<

clean: 
	@rm -f $(TARGETLOC) $(OBJDIR)/tangosocket.o
	
.PHONY: clean
