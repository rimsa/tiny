CXX=g++
CXXFLAGS=-g -ggdb -O2 -Wall

TARGET=tinyi
OBJS=tinyi.o \
		lexical/SymbolTable.o \
		lexical/LexicalAnalysis.o \
		syntatic/SyntaticAnalysis.o \
		interpreter/util/Memory.o \
		interpreter/expr/BinaryIntExpr.o \
		interpreter/expr/ConstIntExpr.o \
		interpreter/expr/NegIntExpr.o \
		interpreter/expr/ReadIntExpr.o \
		interpreter/expr/Variable.o \
		interpreter/expr/ConstBoolExpr.o \
		interpreter/expr/NotBoolExpr.o \
		interpreter/expr/SingleBoolExpr.o \
		interpreter/command/AssignCommand.o \
		interpreter/command/BlocksCommand.o \
		interpreter/command/IfCommand.o \
		interpreter/command/OutputCommand.o \
		interpreter/command/WhileCommand.o

all: $(TARGET)

clean:
	rm -rf $(TARGET) $(OBJS)

install:
	cp $(TARGET) /usr/local/bin

tinyi.o: lexical/LexicalAnalysis.h syntatic/SyntaticAnalysis.h interpreter/command/Command.h

lexical/SymbolTable.o: lexical/TokenType.h

lexical/LexicalAnalysis.o: lexical/LexicalAnalysis.h lexical/Lexeme.h lexical/SymbolTable.h

syntatic/SyntaticAnalysis.o: syntatic/SyntaticAnalysis.h interpreter/expr/*.h interpreter/command/*.h

interpreter/util/Memory.o: interpreter/util/Memory.h

interpreter/expr/BinaryIntExpr.o: interpreter/expr/BinaryIntExpr.h interpreter/expr/IntExpr.h

interpreter/expr/ConstIntExpr.o: interpreter/expr/ConstIntExpr.h interpreter/expr/IntExpr.h

interpreter/expr/NegIntExpr.o: interpreter/expr/NegIntExpr.h interpreter/expr/IntExpr.h

interpreter/expr/ReadIntExpr.o: interpreter/expr/ReadIntExpr.h interpreter/expr/IntExpr.h

interpreter/expr/Variable.o: interpreter/expr/Variable.h interpreter/expr/IntExpr.h interpreter/util/Memory.h

interpreter/expr/ConstBoolExpr.o: interpreter/expr/ConstBoolExpr.h interpreter/expr/BoolExpr.h

interpreter/expr/NotBoolExpr.o: interpreter/expr/NotBoolExpr.h interpreter/expr/BoolExpr.h

interpreter/expr/SingleBoolExpr.o: interpreter/expr/SingleBoolExpr.h interpreter/expr/BoolExpr.h interpreter/expr/IntExpr.h

interpreter/command/AssignCommand.o: interpreter/command/AssignCommand.h interpreter/command/Command.h interpreter/expr/Variable.h

interpreter/command/BlocksCommand.o: interpreter/command/BlocksCommand.h interpreter/command/Command.h

interpreter/command/IfCommand.o: interpreter/command/IfCommand.h interpreter/command/Command.h interpreter/expr/BoolExpr.h

interpreter/command/OutputCommand.o: interpreter/command/OutputCommand.h interpreter/command/Command.h interpreter/expr/IntExpr.h

interpreter/command/WhileCommand.o: interpreter/command/WhileCommand.h interpreter/command/Command.h interpreter/expr/BoolExpr.h

$(TARGET):	$(OBJS)
			$(CXX) -o $(TARGET) $(OBJS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<
