#include "NegIntExpr.h"

NegIntExpr::NegIntExpr(int line, IntExpr* expr)
	: IntExpr(line), m_expr(expr) {
}

NegIntExpr::~NegIntExpr() {
	delete m_expr;
}

int NegIntExpr::expr() {
	return -m_expr->expr();
}
