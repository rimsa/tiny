#ifndef VARIABLE_H
#define VARIABLE_H

#include <map>
#include <string>
#include "IntExpr.h"

class Variable : public IntExpr {
	public:
		Variable(int line, const std::string& name);
		virtual ~Variable();

		const std::string& name() const { return m_name; }
		int value();
		void setValue(int value);

		virtual int expr();

	private:
		const std::string m_name;

};

#endif
