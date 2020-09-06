#include "BlocksCommand.h"

BlocksCommand::BlocksCommand(int line)
	: Command(line) {
}

BlocksCommand::~BlocksCommand() {
	for (std::list<Command*>::iterator it = m_cmds.begin(),
	     ed = m_cmds.end(); it != ed; it++) {
		Command* cmd = *it;
		delete cmd;
	}
}

void BlocksCommand::addCommand(Command* cmd) {
	m_cmds.push_back(cmd);
}

void BlocksCommand::execute() {
	for (std::list<Command*>::iterator it = m_cmds.begin(),
	     ed = m_cmds.end(); it != ed; it++) {
		Command* cmd = *it;
		cmd->execute();
	}
}
