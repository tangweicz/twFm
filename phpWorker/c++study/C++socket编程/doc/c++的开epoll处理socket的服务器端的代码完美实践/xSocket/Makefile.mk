.PHONY:all

CPPFILES = $(wildcard *.cpp)

MIDFILES = $(patsubst %.cpp, %.d, $(CPPFILES))

OBJFILES = $(patsubst %.cpp, %.o, $(CPPFILES))

LINKPATH = $(join ../, linkpath)

MIDLINKPATH = $(join ..\/, linkpath)

ifeq ("$(wildcard $(LINKPATH))", "")
$(shell mkdir $(LINKPATH))
endif

LINKOBJFILES = $(join $(LINKPATH)/, $(OBJFILES))

OTHEROBJS = $(wildcard $(join $(LINKPATH)/, *.o))

LINKOBJFILES += $(OTHEROBJS)

all:$(MIDFILES) $(LINKOBJFILES) $(BIN)

%.d:%.cpp
	g++ -MM $< -std=c++11 -I../include | sed 's/\(.*\)\.o/$(MIDLINKPATH)\/\1.o/g'>> $@

ifneq ("$(wildcard *.d)", "")
include $(MIDFILES)
endif

$(join $(LINKPATH)/, %.o):%.cpp
	g++ $< -c -o $@ -std=c++11 -I../include

$(BIN):$(LINKOBJFILES)
	g++ $^ -o $@ -std=c++11
