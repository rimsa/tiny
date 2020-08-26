#include "SymbolTable.h"

SymbolTable::SymbolTable() {
	// Symbols
	m_symbols[";"] = TT_SEMICOLON;
	m_symbols["="] = TT_ASSIGN;

	// Logic operators
	m_symbols["=="] = TT_EQUAL;
	m_symbols["!="] = TT_NOT_EQUAL;
	m_symbols["<"] = TT_LOWER;
	m_symbols["<="] = TT_LOWER_EQUAL;
	m_symbols[">"] = TT_GREATER;
	m_symbols[">="] = TT_GREATER_EQUAL;

	// Arithmetic operators
	m_symbols["+"] = TT_ADD;
	m_symbols["-"] = TT_SUB;
	m_symbols["*"] = TT_MUL;
	m_symbols["/"] = TT_DIV;
	m_symbols["%"] = TT_MOD;

	// Keywords
	m_symbols["program"] = TT_PROGRAM;
	m_symbols["while"] = TT_WHILE;
	m_symbols["do"] = TT_DO;
	m_symbols["done"] = TT_DONE;
	m_symbols["if"] = TT_IF;
	m_symbols["then"] = TT_THEN;
	m_symbols["else"] = TT_ELSE;
	m_symbols["output"] = TT_OUTPUT;
	m_symbols["true"] = TT_TRUE;
	m_symbols["false"] = TT_FALSE;
	m_symbols["read"] = TT_READ;
	m_symbols["not"] = TT_NOT;
}

SymbolTable::~SymbolTable() {
}

bool SymbolTable::contains(const std::string& token) const {
	return m_symbols.find(token) != m_symbols.end();
}

enum TokenType SymbolTable::find(const std::string& token) {
	return this->contains(token) ?
				m_symbols[token] : TT_VAR;
}
