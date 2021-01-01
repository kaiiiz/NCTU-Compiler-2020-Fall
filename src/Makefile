CC = g++
LEX = flex
YACC = bison
CFLAGS = -Wall -std=gnu++11 -g
LIBS = -lfl -ly
INCLUDE = -Iinclude -I.

SCANNER = scanner
PARSER = parser

ASTDIR = lib/AST/
AST := $(shell find $(ASTDIR) -name '*.cpp')

VISITORDIR = lib/visitor/
VISITOR := $(shell find $(VISITORDIR) -name '*.cpp')

DRIVERDIR = lib/driver/
DRIVER := $(shell find $(DRIVERDIR) -name '*.cpp')

TYPEDIR = lib/type/
TYPE := $(shell find $(TYPEDIR) -name '*.cpp')

SEMANTICDIR = lib/sema/
SEMANTIC := $(shell find $(SEMANTICDIR) -name '*.cpp')

SRC := $(AST) \
	   $(VISITOR) \
	   $(DRIVER) \
	   $(TYPE) \
       $(SEMANTIC)

EXEC = $(PARSER)
OBJS = $(PARSER:=.cpp) \
       $(SCANNER:=.cpp) \
       $(SRC)

# Substitution reference
DEPS := $(OBJS:%.cpp=%.d)
OBJS := $(OBJS:%.cpp=%.o)

all: $(EXEC)

# Static pattern rule
$(SCANNER).cpp: %.cpp: %.ll
	$(LEX) -o $@ $<

$(PARSER).cpp: %.cpp: %.yy
	$(YACC) -o $@ --defines=parser.hh -v $<

%.o: %.cpp
	$(CC) -o $@ $(CFLAGS) $(INCLUDE) -c -MMD $<

$(EXEC): $(OBJS)
	$(CC) -o $@ $^ $(LIBS) $(INCLUDE)

clean:
	$(RM) $(DEPS) $(SCANNER:=.cpp) $(PARSER:=.cpp) $(PARSER:=.hh) $(OBJS) $(EXEC)

test:
	./unittests/test.sh

-include $(DEPS)
