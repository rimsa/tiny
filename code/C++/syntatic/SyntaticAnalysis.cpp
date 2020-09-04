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
	// TODO: Implement me!
}

// <cmd>       ::= (<assign> | <output> | <if> | <while>) ;
void SyntaticAnalysis::procCmd() {
	// TODO: Implement me!
}

// <assign>    ::= <var> = <intexpr>
void SyntaticAnalysis::procAssign() {
	// TODO: Implement me!
}

// <output>    ::= output <intexpr>
void SyntaticAnalysis::procOutput() {
	// TODO: Implement me!
}

// <if>        ::= if <boolexpr> then <cmdlist> [ else <cmdlist> ] done
void SyntaticAnalysis::procIf() {
	// TODO: Implement me!
}

// <while>     ::= while <boolexpr> do <cmdlist> done
void SyntaticAnalysis::procWhile() {
	// TODO: Implement me!
}

// <boolexpr>  ::= false | true |
//                 not <boolexpr> |
//                 <intterm> (== | != | < | > | <= | >=) <intterm>
void SyntaticAnalysis::procBoolExpr() {
	// TODO: Implement me!
}

// <intexpr>   ::= [ + | - ] <intterm> [ (+ | - | * | / | %) <intterm> ]
void SyntaticAnalysis::procIntExpr() {
	// TODO: Implement me!
}

// <intterm>   ::= <var> | <const> | read
void SyntaticAnalysis::procIntTerm() {
	// TODO: Implement me!
}

// <var>       ::= id
void SyntaticAnalysis::procVar() {
	// TODO: Implement me!
}

// <const>     ::= number
void SyntaticAnalysis::procConst() {
	// TODO: Implement me!
}
