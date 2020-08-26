#include "SymbolTable.h"

SymbolTable::SymbolTable() {
	// TODO: Implement me!
}

SymbolTable::~SymbolTable() {
}

bool SymbolTable::contains(const std::string& token) const {
	return m_symbols.find(token) != m_symbols.end();
}

enum TokenType SymbolTable::find(const std::string& token) {
	return this->contains(token) ?
				m_symbols[token] : TT_INVALID_TOKEN;
}
