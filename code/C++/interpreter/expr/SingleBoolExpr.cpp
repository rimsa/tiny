#include "IntExpr.h"
#include "SingleBoolExpr.h"

SingleBoolExpr::SingleBoolExpr(int line, IntExpr* left, enum SingleBoolExpr::Op op, IntExpr* right)
	: BoolExpr(line), m_left(left), m_op(op), m_right(right) {
}

SingleBoolExpr::~SingleBoolExpr() {
	delete m_left;
	delete m_right;
}

bool SingleBoolExpr::expr() {
	int v1 = m_left->expr();
	int v2 = m_right->expr();

	switch (m_op) {
		case SingleBoolExpr::EQUAL:
			return v1 == v2;
		case SingleBoolExpr::NOT_EQUAL:
			return v1 != v2;
		case SingleBoolExpr::LOWER:
			return v1 < v2;
		case SingleBoolExpr::LOWER_EQUAL:
			return v1 <= v2;
		case SingleBoolExpr::GREATER:
			return v1 > v2;
		case SingleBoolExpr::GREATER_EQUAL:
		default:
			return v1 >= v2;
	}
}
