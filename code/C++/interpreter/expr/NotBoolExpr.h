#ifndef NOT_BOOL_EXPR_H
#define NOT_BOOL_EXPR_H

#include "BoolExpr.h"

class NotBoolExpr : public BoolExpr {
	public:
		NotBoolExpr(int line, BoolExpr* expr);
		virtual ~NotBoolExpr();

		virtual bool expr();

	private:
		BoolExpr* m_expr;

};

#endif
