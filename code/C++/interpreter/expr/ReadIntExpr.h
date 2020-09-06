#ifndef READ_INT_EXPR_H
#define READ_INT_EXPR_H

#include "IntExpr.h"

class ReadIntExpr : public IntExpr {
	public:
		ReadIntExpr(int line);
		virtual ~ReadIntExpr();

		virtual int expr();

};

#endif
