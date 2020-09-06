#ifndef CONST_BOOL_EXPR_H
#define CONST_BOOL_EXPR_H

#include "BoolExpr.h"

class ConstBoolExpr : public BoolExpr {
	public:
		ConstBoolExpr(int line, bool value);
		virtual ~ConstBoolExpr();

		virtual bool expr();

	private:
		bool m_value;
};

#endif
