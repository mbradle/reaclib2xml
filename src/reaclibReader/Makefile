# Makefile for ReaclibReader

CXX := g++
LD := g++

# tiny XML files
SRCS := tinyxmlparser.cpp tinyxmlerror.cpp tinystr.cpp tinyxml.cpp
# reaclib reader files
SRCS := ${SRCS} reaclibReader.cpp reaclibRate.cpp reaclibVer.cpp reaclibSet.cpp
SRCS := ${SRCS} NuclideSearch.cpp

# the objects needed to compile the reader
OBJS := $(addsuffix .o,$(basename ${SRCS}))

# base program
MAIN_SRCS := main.cpp
MAIN_OBJS := $(addsuffix .o, $(basename ${MAIN_SRCS}))
MAIN_PROG := reaclibToBDAT

# all obj
ALL_OBJS := $(OBJS) $(PROG) $(MAIN_OBJS)

# the support files
#SUPPORT := line2.dat 

all: ${SUPPORT} ${MAIN_PROG} 

objs: ${OBJS}
.PHONY: objs
	
${MAIN_PROG}: ${OBJS} $(MAIN_OBJS) 
	${LD} -o $@ ${OBJS} $(MAIN_OBJS)
	
%.o : %.cpp %.h
	${CXX} -c -Wall -g -DDEBUG $< -o $@

%.o : %.cpp	
	${CXX} -c -Wall -g -DDEBUG $< -o $@

#line2.dat : createLine2
#	./createLine2
	
createLine2 : fit_part.f
	f77 -o createLine2 -u -r8 fit_part.f
	
clean:
	rm -f $(ALL_OBJS) $(SUPPORT) createLine2 $(MAIN_PROG)

test: all
	nice $(MAIN_PROG) small.reaclib line2.dat nuclides.xml
	
test2: all
	nice $(MAIN_PROG) reaclib.txt line2.dat nuclides.xml > all.bdat

test4: all
	nice reaclibToBDAT current line2_4.dat nuclides.xml
test42: all
	nice reaclibToBDAT ths8 line2_4.dat nuclides.xml

	
rath: all
	nice $(MAIN_PROG) rath.txt line2.dat nuclides.xml > rath.bdat
