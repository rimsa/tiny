#ifndef BLOCKS_COMMAND_H
#define BLOCKS_COMMAND_H

#include <list>
#include "Command.h"

class BlocksCommand : public Command {
	public:
		BlocksCommand(int line);
		virtual ~BlocksCommand();

		void addCommand(Command* cmd);
		virtual void execute();

	private:
		std::list<Command*> m_cmds;
};

#endif
