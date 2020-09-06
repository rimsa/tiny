#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstdlib>

#include "SyntaticAnalysis.h"

#include "../interpreter/expr/IntExpr.h"
#include "../interpreter/expr/BinaryIntExpr.h"
#include "../interpreter/expr/ConstIntExpr.h"
#include "../interpreter/expr/NegIntExpr.h"
#include "../interpreter/expr/ReadIntExpr.h"
#include "../interpreter/expr/Variable.h"
#include "../interpreter/expr/BoolExpr.h"
#include "../interpreter/expr/ConstBoolExpr.h"
#include "../interpreter/expr/NotBoolExpr.h"
#include "../interpreter/expr/SingleBoolExpr.h"
#include "../interpreter/command/Command.h"
#include "../interpreter/command/AssignCommand.h"
#include "../interpreter/command/BlocksCommand.h"
#include "../interpreter/command/IfCommand.h"
#include "../interpreter/command/OutputCommand.h"
#include "../interpreter/command/WhileCommand.h"

SyntaticAnalysis::SyntaticAnalysis(LexicalAnalysis& lex) :
	m_lex(lex), m_current(lex.nextToken()) {
}

SyntaticAnalysis::~SyntaticAnalysis() {
}

Command* SyntaticAnalysis::start() {
	Command* cmd = procProgram();
	eat(TT_END_OF_FILE);
	return cmd;
}

void SyntaticAnalysis::advance() {
	m_current = m_lex.nextToken();
}

void SyntaticAnalysis::eat(enum TokenType type) {
	// std::cout << "Expected (..., " << tt2str(type)
	//           <<  "), found (\"" + m_current.token << "\", "
	// 		  << tt2str(m_current.type) << ")" << std::endl;
	if (type == m_current.type) {
		advance();
	} else {
		showError();
	}
}

void SyntaticAnalysis::showError() {
	std::cout << std::setw(2) << std::setfill('0') << m_lex.line() << ": ";

	switch (m_current.type) {
		case TT_INVALID_TOKEN:
			std::cout << "Lexema inválido [" << m_current.token << "]" << std::endl;
			break;
		case TT_UNEXPECTED_EOF:
		case TT_END_OF_FILE:
			std::cout << "Fim de arquivo inesperado" << std::endl;
			break;
		default:
			std::cout << "Lexema não esperado [" << m_current.token << "]" << std::endl;
			break;
	}

    exit(1);
}

// <program>   ::= program <cmdlist>
Command* SyntaticAnalysis::procProgram() {
	eat(TT_PROGRAM);
	Command* cmd = procCmdList();
	return cmd;
}

// <cmdlist>   ::= <cmd> { <cmd> }
BlocksCommand* SyntaticAnalysis::procCmdList() {
	int line = m_lex.line();
	BlocksCommand* cmds = new BlocksCommand(line);

	Command* cmd = procCmd();
	cmds->addCommand(cmd);

	while (m_current.type == TT_VAR ||
	       m_current.type == TT_OUTPUT ||
		   m_current.type == TT_IF ||
		   m_current.type == TT_WHILE) {
		cmd = procCmd();
		cmds->addCommand(cmd);
	}

	return cmds;
}

// <cmd>       ::= (<assign> | <output> | <if> | <while>) ;
Command* SyntaticAnalysis::procCmd() {
	Command* cmd = 0;
	if (m_current.type == TT_VAR) {
		cmd = procAssign();
	} else if (m_current.type == TT_OUTPUT) {
		cmd = procOutput();
	} else if (m_current.type == TT_IF) {
		cmd = procIf();
	} else if (m_current.type == TT_WHILE) {
		cmd = procWhile();
	} else {
		showError();
	}

	eat(TT_SEMICOLON);
	return cmd;
}

// <assign>    ::= <var> = <intexpr>
AssignCommand* SyntaticAnalysis::procAssign() {
	int line = m_lex.line();

	Variable* var = procVar();
	eat(TT_ASSIGN);
	IntExpr* expr = procIntExpr();

	AssignCommand* cmd = new AssignCommand(line, var, expr);
	return cmd;
}

// <output>    ::= output <intexpr>
OutputCommand* SyntaticAnalysis::procOutput() {
	eat(TT_OUTPUT);
	int line = m_lex.line();

	IntExpr* expr = procIntExpr();
	OutputCommand* cmd = new OutputCommand(line, expr);
	return cmd;
}

// <if>        ::= if <boolexpr> then <cmdlist> [ else <cmdlist> ] done
IfCommand* SyntaticAnalysis::procIf() {
	eat(TT_IF);
	int line = m_lex.line();

	BoolExpr* cond = procBoolExpr();
	eat(TT_THEN);
	Command* thenCmds = procCmdList();
	Command* elseCmds = 0;
	if (m_current.type == TT_ELSE) {
		advance();
		elseCmds = procCmdList();
	}
	eat(TT_DONE);

	IfCommand* cmd = new IfCommand(line, cond, thenCmds, elseCmds);
	return cmd;
}

// <while>     ::= while <boolexpr> do <cmdlist> done
WhileCommand* SyntaticAnalysis::procWhile() {
	eat(TT_WHILE);
	int line = m_lex.line();

	BoolExpr* expr = procBoolExpr();
	eat(TT_DO);
	Command* cmds = procCmdList();
	eat(TT_DONE);

	WhileCommand* cmd = new WhileCommand(line, expr, cmds);
	return cmd;
}

// <boolexpr>  ::= false | true |
//                 not <boolexpr> |
//                 <intterm> (== | != | < | > | <= | >=) <intterm>
BoolExpr* SyntaticAnalysis::procBoolExpr() {
	if (m_current.type == TT_FALSE) {
		advance();
		return new ConstBoolExpr(m_lex.line(), false);
	} else if (m_current.type == TT_TRUE) {
		advance();
		return new ConstBoolExpr(m_lex.line(), false);
	} else if (m_current.type == TT_NOT) {
		advance();
		int line = m_lex.line();
		BoolExpr* expr = procBoolExpr();
		return new NotBoolExpr(line, expr);
	} else {
		int line = m_lex.line();
		IntExpr* left = procIntTerm();

		enum SingleBoolExpr::Op op = SingleBoolExpr::EQUAL;
		switch (m_current.type) {
			case TT_EQUAL:
				op = SingleBoolExpr::EQUAL;
				advance();
				break;
			case TT_NOT_EQUAL:
				op = SingleBoolExpr::NOT_EQUAL;
				advance();
				break;
			case TT_LOWER:
				op = SingleBoolExpr::LOWER;
				advance();
				break;
			case TT_GREATER:
				op = SingleBoolExpr::GREATER;
				advance();
				break;
			case TT_LOWER_EQUAL:
				op = SingleBoolExpr::LOWER_EQUAL;
				advance();
				break;
			case TT_GREATER_EQUAL:
				op = SingleBoolExpr::GREATER_EQUAL;
				advance();
				break;
			default:
				showError();
		}

		IntExpr* right = procIntTerm();

		BoolExpr* expr = new SingleBoolExpr(line, left, op, right);
		return expr;
	}
}

// <intexpr>   ::= [ + | - ] <intterm> [ (+ | - | * | / | %) <intterm> ]
IntExpr* SyntaticAnalysis::procIntExpr() {
	bool is_negative = false;
	if (m_current.type == TT_ADD) {
		advance();
	} else if (m_current.type == TT_SUB) {
		advance();
		is_negative = true;
	}

	IntExpr* left;
	if (is_negative) {
		int line = m_lex.line();
		IntExpr* tmp = procIntTerm();
		left = new NegIntExpr(line, tmp);
	} else {
		left = procIntTerm();
	}

	if (m_current.type == TT_ADD ||
	    m_current.type == TT_SUB ||
		m_current.type == TT_MUL ||
	    m_current.type == TT_DIV ||
	    m_current.type == TT_MOD) {
		int line = m_lex.line();

		enum BinaryIntExpr::Op op;
		switch (m_current.type) {
			case TT_ADD:
				op = BinaryIntExpr::ADD;
				advance();
				break;
			case TT_SUB:
				op = BinaryIntExpr::SUB;
				advance();
				break;
			case TT_MUL:
				op = BinaryIntExpr::MUL;
				advance();
				break;
			case TT_DIV:
				op = BinaryIntExpr::DIV;
				advance();
				break;
			case TT_MOD:
			default:
				op = BinaryIntExpr::MOD;
				advance();
				break;
		}

		IntExpr* right = procIntTerm();

		left = new BinaryIntExpr(line, left, op, right);
	}

	return left;
}

// <intterm>   ::= <var> | <const> | read
IntExpr* SyntaticAnalysis::procIntTerm() {
	if (m_current.type == TT_VAR) {
		return procVar();
	} else if (m_current.type == TT_NUMBER) {
		return procConst();
	} else {
		eat(TT_READ);
		int line = m_lex.line();
		ReadIntExpr* expr = new ReadIntExpr(line);
		return expr;
	}
}

// <var>       ::= id
Variable* SyntaticAnalysis::procVar() {
	std::string tmp = m_current.token;

	eat(TT_VAR);
	int line = m_lex.line();

	Variable* var = new Variable(line, tmp);
	return var;
}

// <const>     ::= number
ConstIntExpr* SyntaticAnalysis::procConst() {
	std::string tmp = m_current.token;

	eat(TT_NUMBER);
	int line = m_lex.line();

	int value = atoi(tmp.c_str());
	ConstIntExpr* expr = new ConstIntExpr(line, value);
	return expr;
}
