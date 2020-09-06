#include "AssignCommand.h"
#include "../expr/Variable.h"

AssignCommand::AssignCommand(int line, Variable* var, IntExpr* expr)
	: Command(line), m_var(var), m_expr(expr) {
}

AssignCommand::~AssignCommand() {
	delete m_var;
	delete m_expr;
}

void AssignCommand::execute() {
	int value = m_expr->expr();
	m_var->setValue(value);
}
