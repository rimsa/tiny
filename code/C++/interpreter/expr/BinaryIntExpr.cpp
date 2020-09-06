#include "BinaryIntExpr.h"

BinaryIntExpr::BinaryIntExpr(int line, IntExpr* left, enum BinaryIntExpr::Op op, IntExpr* right)
	: IntExpr(line), m_left(left), m_op(op), m_right(right) {
}

BinaryIntExpr::~BinaryIntExpr() {
	delete m_left;
	delete m_right;
}

int BinaryIntExpr::expr() {
	int v1 = m_left->expr();
	int v2 = m_right->expr();

	switch (m_op) {
		case BinaryIntExpr::ADD:
			return v1 + v2;
		case BinaryIntExpr::SUB:
			return v1 - v2;
		case BinaryIntExpr::MUL:
			return v1 * v2;
		case BinaryIntExpr::DIV:
			return v1 / v2;
		case BinaryIntExpr::MOD:
		default:
			return v1 % v2;
	}
}
