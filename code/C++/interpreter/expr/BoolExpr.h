#ifndef BOOL_EXPR_H
#define BOOL_EXPR_H

class BoolExpr {
	public:
		virtual ~BoolExpr() {}

		int line() const { return m_line; }
		virtual bool expr() = 0;

	protected:
		BoolExpr(int line) : m_line(line) {}

	private:
		int m_line;

};

#endif
