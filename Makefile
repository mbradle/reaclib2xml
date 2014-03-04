LIBNUCNET_VERSION = 0.21
WNMATRIX_VERSION = 0.14

SOURCEFORGE_URL = http://sourceforge.net/projects/

LIBNUCNET_URL = $(SOURCEFORGE_URL)/libnucnet/files/libnucnet
WNMATRIX_URL = $(SOURCEFORGE_URL)/wnmatrix/files/wn_matrix

LIBNUCNETDIR = $(VENDORDIR)/libnucnet/$(LIBNUCNET_VERSION)/src
WNMATRIXDIR = $(VENDORDIR)/wn_matrix/$(WNMATRIX_VERSION)/src

GC=g++
CINCLUDE=`xml2-config --cflags` `gsl-config --cflags` \
            -I$(WNMATRIXDIR) \
            -I$(LIBNUCNETDIR)
CLIBS=`xml2-config --libs` `gsl-config --libs`
CLIBS_STATIC=`xml2-config --libs` `gsl-config --libs`
CFLAGS=-Wall
CC=$(GC) $(CFLAGS) $(CINCLUDE) $(CLIBS)

#===============================================================================
# OBJDIR is the temporary directory for codes compilation, this is where
# object files are created.
#===============================================================================

ifndef OBJDIR
OBJDIR = ./build/obj
endif

OBJ_DIR := $(shell mkdir -p $(OBJDIR))

VPATH= .:$(SRCDIR):$(RCDIR):$(WNMATRIXDIR):$(LIBNUCNETDIR)

#===============================================================================
# BINDIR is the temporary directory for code executables.
#===============================================================================

ifndef BINDIR
BINDIR = ./build/bin
endif

BIN_DIR := $(shell mkdir -p $(BINDIR))

#===============================================================================
# VENDORDIR is the directory for storing webnucleo codes.
#===============================================================================

ifndef VENDORDIR
VENDORDIR = ./vendor
endif

VENDOR_DIR := $(shell mkdir -p $(VENDORDIR))

#===============================================================================
# SRCDIR is the directory for the main codes.
#===============================================================================

SRCDIR = ./src

#===============================================================================
# RCDIR is the directory for the reaclib reader.
#===============================================================================

RCDIR = $(SRCDIR)/reaclibReader

#===============================================================================
# The make target.
#===============================================================================

target: all

#===============================================================================
# Libnucnet and WnMatrix shared objects
#===============================================================================

$(VENDORDIR)/wn_matrix.tar.gz:
	wget $(WNMATRIX_URL)/wn_matrix_$(WNMATRIX_VERSION).tar.gz -O $@

$(VENDORDIR)/libnucnet.tar.gz:
	wget $(LIBNUCNET_URL)/libnucnet_$(LIBNUCNET_VERSION).tar.gz -O $@

$(OBJDIR)/WnMatrix.o: $(VENDORDIR)/wn_matrix.tar.gz
	tar xz -C $(VENDORDIR) -f $<
	$(CC) -c $(WNMATRIXDIR)/WnMatrix.c -o $@

$(OBJDIR)/Libnucnet__Nuc.o: $(VENDORDIR)/libnucnet.tar.gz $(OBJDIR)/WnMatrix.o
	tar xz -C $(VENDORDIR) -f $<
	$(CC) -c $(LIBNUCNETDIR)/Libnucnet__Nuc.c -o $@

$(OBJDIR)/Libnucnet__Reac.o: $(VENDORDIR)/libnucnet.tar.gz $(OBJDIR)/WnMatrix.o
	tar xz -C $(VENDORDIR) -f $<
	$(CC) -c $(LIBNUCNETDIR)/Libnucnet__Reac.c -o $@

$(OBJDIR)/Libnucnet.o: $(VENDORDIR)/libnucnet.tar.gz $(OBJDIR)/WnMatrix.o
	tar xz -C $(VENDORDIR) -f $<
	$(CC) -c $(LIBNUCNETDIR)/Libnucnet.c -o $@

WN_OBJ = $(OBJDIR)/WnMatrix.o        \
	 $(OBJDIR)/Libnucnet__Nuc.o  \
	 $(OBJDIR)/Libnucnet__Reac.o \
	 $(OBJDIR)/Libnucnet.o       \

#===============================================================================
# ReaclibReader files.
#===============================================================================

RC_OBJ = $(OBJDIR)/NuclideSearch.o \
	 $(OBJDIR)/reaclibRate.o   \
	 $(OBJDIR)/reaclibReader.o \
	 $(OBJDIR)/reaclibSet.o    \
	 $(OBJDIR)/reaclibVer.o    \
	 $(OBJDIR)/tinystr.o       \
	 $(OBJDIR)/tinyxml.o       \
	 $(OBJDIR)/tinyxmlerror.o  \
	 $(OBJDIR)/tinyxmlparser.o \

$(RC_OBJ): $(OBJDIR)/%.o: $(RCDIR)/%.cpp
	$(CC) -c -o $@ $<

#===============================================================================
# General build commands
#===============================================================================

REAC2XML_EXEC = reaclib2xml   \
                partition2xml \

$(REAC2XML_EXEC): $(WN_OBJ) $(RC_OBJ)
	$(CC) -c -o $@ $<
	$(CC) -o $(BINDIR)/$@ $(SRCDIR)/$@.cpp $(CLIBS) $(WN_OBJ) $(RC_OBJ) 

.PHONY all : $(REAC2XML_EXEC)

#===============================================================================
# Clean up.
#===============================================================================

.PHONY: clean clean_all clean_dist

clean:
	rm -fr $(OBJDIR)

clean_dist:
	rm -fr $(VENDORDIR)
