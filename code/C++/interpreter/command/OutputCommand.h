#ifndef OUTPUT_COMMAND_H
#define OUTPUT_COMMAND_H

#include "Command.h"

class IntExpr;

class OutputCommand : public Command {
	public:
		OutputCommand(int line, IntExpr* expr);
		virtual ~OutputCommand();

		virtual void execute();

	private:
		IntExpr* m_expr;
};

#endif
