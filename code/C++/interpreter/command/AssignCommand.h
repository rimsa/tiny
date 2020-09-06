#ifndef ASSIGN_COMMAND_H
#define ASSIGN_COMMAND_H

#include "Command.h"

class Variable;
class IntExpr;

class AssignCommand : public Command {
	public:
		AssignCommand(int line, Variable* var, IntExpr* expr);
		virtual ~AssignCommand();

		virtual void execute();

	private:
		Variable* m_var;
		IntExpr* m_expr;

};

#endif

