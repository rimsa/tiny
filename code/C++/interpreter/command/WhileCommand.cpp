#include "WhileCommand.h"
#include "../expr/BoolExpr.h"

WhileCommand::WhileCommand(int line, BoolExpr* cond, Command* cmds)
	: Command(line), m_cond(cond), m_cmds(cmds) {
}

WhileCommand::~WhileCommand() {
	delete m_cond;
	delete m_cmds;
}

void WhileCommand::execute() {
	while (m_cond->expr())
		m_cmds->execute();
}
