#ifndef BINARY_INT_EXPR_H
#define BINARY_INT_EXPR_H

#include "IntExpr.h"

class BinaryIntExpr : public IntExpr {
	public:
		enum Op {
			ADD,
			SUB,
			MUL,
			DIV,
			MOD
		};

		BinaryIntExpr(int line, IntExpr* left, enum Op op, IntExpr* right);
		virtual ~BinaryIntExpr();

		virtual int expr();

	private:
		IntExpr* m_left;
		enum Op m_op;
		IntExpr* m_right;

};

#endif
