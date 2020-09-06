#ifndef SYNTATIC_ANALYSIS_H
#define SYNTATIC_ANALYSIS_H

#include "../lexical/LexicalAnalysis.h"

class IntExpr;
class ConstIntExpr;
class Variable;
class BoolExpr;

class Command;
class AssignCommand;
class BlocksCommand;
class IfCommand;
class OutputCommand;
class WhileCommand;

class SyntaticAnalysis {
	public:
		SyntaticAnalysis(LexicalAnalysis& lex);
		virtual ~SyntaticAnalysis();

		Command* start();

	private:
		LexicalAnalysis& m_lex;
		Lexeme m_current;

		void advance();
		void eat(enum TokenType type);
		void showError();

		Command* procProgram();
		BlocksCommand* procCmdList();
		Command* procCmd();
		AssignCommand* procAssign();
		OutputCommand* procOutput();
		IfCommand* procIf();
		WhileCommand* procWhile();
		BoolExpr* procBoolExpr();
		IntExpr* procIntExpr();
		IntExpr* procIntTerm();
		Variable* procVar();
		ConstIntExpr* procConst();

};

#endif
