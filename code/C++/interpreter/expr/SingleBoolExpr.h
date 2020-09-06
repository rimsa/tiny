#ifndef SINGLE_BOOL_EXPR_H
#define SINGLE_BOOL_EXPR_H

#include "BoolExpr.h"

class IntExpr;

class SingleBoolExpr : public BoolExpr {
	public:
		enum Op {
			EQUAL,
			NOT_EQUAL,
			LOWER,
			GREATER,
			LOWER_EQUAL,
			GREATER_EQUAL
		};

		SingleBoolExpr(int line, IntExpr* left, enum Op op, IntExpr* right);
		virtual ~SingleBoolExpr();

		virtual bool expr();

	private:
		IntExpr* m_left;
		enum Op m_op;
		IntExpr* m_right;

};

#endif
