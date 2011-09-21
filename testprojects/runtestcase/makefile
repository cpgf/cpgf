CXXDIR=/cxxtest
CXXTESTGEN=perl $(CXXDIR)/cxxtestgen.pl

TESTCASE_DIR=../../include/cpgf/testcase
TESTSOURCE_DIR=testsource
CL=cl.exe -EHsc -W3 -WX -I. -I$(CXXDIR) -I../../include
BIN_DIR=bin
BIN_TARTGET=testmain.exe

all: $(TESTSOURCE_DIR)/TestContainer.cpp $(TESTSOURCE_DIR)/TestCallback.cpp $(TESTSOURCE_DIR)/TestEvent.cpp

$(TESTSOURCE_DIR)/%.cpp: $(TESTCASE_DIR)/%.h
	$(CXXTESTGEN) -part -o $@ $<

