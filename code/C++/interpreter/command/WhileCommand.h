#ifndef WHILE_COMMAND_H
#define WHILE_COMMAND_H

#include "Command.h"

class BoolExpr;

class WhileCommand : public Command {
	public:
		WhileCommand(int line, BoolExpr* cond, Command* cmds);
		virtual ~WhileCommand();

		virtual void execute();

	private:
		BoolExpr* m_cond;
		Command* m_cmds;

};

#endif
