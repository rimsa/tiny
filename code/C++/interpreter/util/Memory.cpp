#include "Memory.h"

std::map<std::string, int> Memory::m_memory;

int Memory::read(const std::string& name) {
	return Memory::m_memory[name];
}

void Memory::write(const std::string& name, int value) {
	Memory::m_memory[name] = value;
}
