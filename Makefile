CXX ?= g++

CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic
LDFLAGS = -I$(CURDIR)/stdb

SRCS =	$(CURDIR)/main.cpp \
	$(CURDIR)/stdb/stdb.cpp

OBJS = $(addsuffix .obj, $(basename $(SRCS)))

EXE = stdbtool

$(EXE) : $(OBJS)
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -o '$@' $^

%.obj : %.cpp
	@echo "[ CXX     ] " $@
	$(CXX) $(LDFLAGS) $(CXXFLAGS) -c -o '$@' $^


clean :
	rm -rf $(OBJS)
