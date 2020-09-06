#ifndef MEMORY_H
#define MEMORY_H

#include <map>
#include <string>

class Memory {
	public:
		static int read(const std::string& name);
		static void write(const std::string& name, int value);

	private:
		static std::map<std::string, int> m_memory;

};

#endif
