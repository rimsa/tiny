#include "ConstBoolExpr.h"

ConstBoolExpr::ConstBoolExpr(int line, bool value)
	: BoolExpr(line), m_value(value) {
}

ConstBoolExpr::~ConstBoolExpr() {
}

bool ConstBoolExpr::expr() {
	return m_value;
}
