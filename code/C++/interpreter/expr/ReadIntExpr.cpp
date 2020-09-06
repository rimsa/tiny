#include <iostream>
#include "ReadIntExpr.h"

ReadIntExpr::ReadIntExpr(int line)
	: IntExpr(line) {
}

ReadIntExpr::~ReadIntExpr() {
}

int ReadIntExpr::expr() {
	int value;
	std::cin >> value;
	return value;
}
