#ifndef CONST_INT_EXPR_H
#define CONST_INT_EXPR_H

#include "IntExpr.h"

class ConstIntExpr : public IntExpr {
	public:
		ConstIntExpr(int line, int value);
		virtual ~ConstIntExpr();

		virtual int expr();

	private:
		int m_value;

};

#endif
