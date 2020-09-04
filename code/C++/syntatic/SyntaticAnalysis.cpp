#include <iostream>
#include <iomanip>
#include <cstdlib>
#include "SyntaticAnalysis.h"

SyntaticAnalysis::SyntaticAnalysis(LexicalAnalysis& lex) :
	m_lex(lex), m_current(lex.nextToken()) {
}

SyntaticAnalysis::~SyntaticAnalysis() {
}

void SyntaticAnalysis::start() {
	procProgram();
	eat(TT_END_OF_FILE);
}

void SyntaticAnalysis::advance() {
	m_current = m_lex.nextToken();
}

void SyntaticAnalysis::eat(enum TokenType type) {
	std::cout << "Expected (..., " << tt2str(type)
	          <<  "), found (\"" + m_current.token << "\", "
			  << tt2str(m_current.type) << ")" << std::endl;
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
void SyntaticAnalysis::procProgram() {
	eat(TT_PROGRAM);
	procCmdList();
}

// <cmdlist>   ::= <cmd> { <cmd> }
void SyntaticAnalysis::procCmdList() {
	procCmd();
	while (m_current.type == TT_VAR ||
	       m_current.type == TT_OUTPUT ||
		   m_current.type == TT_IF ||
		   m_current.type == TT_WHILE) {
		procCmd();
	}
}

// <cmd>       ::= (<assign> | <output> | <if> | <while>) ;
void SyntaticAnalysis::procCmd() {
	if (m_current.type == TT_VAR) {
		procAssign();
	} else if (m_current.type == TT_OUTPUT) {
		procOutput();
	} else if (m_current.type == TT_IF) {
		procIf();
	} else if (m_current.type == TT_WHILE) {
		procWhile();
	} else {
		showError();
	}

	eat(TT_SEMICOLON);
}

// <assign>    ::= <var> = <intexpr>
void SyntaticAnalysis::procAssign() {
	procVar();
	eat(TT_ASSIGN);
	procIntExpr();
}

// <output>    ::= output <intexpr>
void SyntaticAnalysis::procOutput() {
	eat(TT_OUTPUT);
	procIntExpr();
}

// <if>        ::= if <boolexpr> then <cmdlist> [ else <cmdlist> ] done
void SyntaticAnalysis::procIf() {
	eat(TT_IF);
	procBoolExpr();
	eat(TT_THEN);
	procCmdList();
	if (m_current.type == TT_ELSE) {
		advance();
		procCmdList();
	}
	eat(TT_DONE);
}

// <while>     ::= while <boolexpr> do <cmdlist> done
void SyntaticAnalysis::procWhile() {
	eat(TT_WHILE);
	procBoolExpr();
	eat(TT_DO);
	procCmdList();
	eat(TT_DONE);
}

// <boolexpr>  ::= false | true |
//                 not <boolexpr> |
//                 <intterm> (== | != | < | > | <= | >=) <intterm>
void SyntaticAnalysis::procBoolExpr() {
	if (m_current.type == TT_FALSE) {
		advance();
	} else if (m_current.type == TT_TRUE) {
		advance();
	} else if (m_current.type == TT_NOT) {
		advance();
		procBoolExpr();
	} else {
		procIntTerm();

		switch (m_current.type) {
			case TT_EQUAL:
			case TT_NOT_EQUAL:
			case TT_LOWER:
			case TT_GREATER:
			case TT_LOWER_EQUAL:
			case TT_GREATER_EQUAL:
				advance();
				break;
			default:
				showError();
		}

		procIntTerm();
	}
}

// <intexpr>   ::= [ + | - ] <intterm> [ (+ | - | * | / | %) <intterm> ]
void SyntaticAnalysis::procIntExpr() {
	if (m_current.type == TT_ADD) {
		advance();
	} else if (m_current.type == TT_SUB) {
		advance();
	}

	procIntTerm();

	if (m_current.type == TT_ADD ||
	    m_current.type == TT_SUB ||
		m_current.type == TT_MUL ||
	    m_current.type == TT_DIV ||
	    m_current.type == TT_MOD) {
		switch (m_current.type) {
			case TT_ADD:
			case TT_SUB:
			case TT_MUL:
			case TT_DIV:
			case TT_MOD:
			default:
				advance();
		}

		procIntTerm();
	}
}

// <intterm>   ::= <var> | <const> | read
void SyntaticAnalysis::procIntTerm() {
	if (m_current.type == TT_VAR) {
		procVar();
	} else if (m_current.type == TT_NUMBER) {
		procConst();
	} else {
		eat(TT_READ);
	}
}

// <var>       ::= id
void SyntaticAnalysis::procVar() {
	eat(TT_VAR);
}

// <const>     ::= number
void SyntaticAnalysis::procConst() {
	eat(TT_NUMBER);
}
