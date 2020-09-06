#include "ConstIntExpr.h"

ConstIntExpr::ConstIntExpr(int line, int value)
	: IntExpr(line), m_value(value) {
}

ConstIntExpr::~ConstIntExpr() {
}

int ConstIntExpr::expr() {
	return m_value;
}
