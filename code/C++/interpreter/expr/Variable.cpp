#include "Variable.h"
#include "../util/Memory.h"

Variable::Variable(int line, const std::string& name)
	: IntExpr(line), m_name(name) {
}

Variable::~Variable() {
}

int Variable::value() {
	return Memory::read(m_name);
}

void Variable::setValue(int value) {
	Memory::write(m_name, value);
}

int Variable::expr() {
	return this->value();
}
