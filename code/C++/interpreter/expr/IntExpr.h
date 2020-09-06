#ifndef INT_EXPR_H
#define INT_EXPR_H

class IntExpr {
	public:
		virtual ~IntExpr() {}
		int line() const { return m_line; }

		virtual int expr() = 0;

	protected:
		IntExpr(int line) : m_line(line) {}

	private:
		int m_line;
};

#endif
