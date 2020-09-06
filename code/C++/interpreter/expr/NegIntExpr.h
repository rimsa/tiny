#ifndef NEG_INT_EXPR_H
#define NEG_INT_EXPR_H

#include "IntExpr.h"

class NegIntExpr : public IntExpr {
	public:
		NegIntExpr(int line, IntExpr* expr);
		virtual ~NegIntExpr();

		virtual int expr();

	private:
		IntExpr* m_expr;

};

#endif
