#include <iostream>
#include "OutputCommand.h"
#include "../expr/IntExpr.h"

OutputCommand::OutputCommand(int line, IntExpr* expr)
	: Command(line), m_expr(expr) {
}

OutputCommand::~OutputCommand() {
	delete m_expr;
}

void OutputCommand::execute() {
	int v = m_expr->expr();
	std::cout << v << std::endl;
}
