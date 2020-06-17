package interpreter.expr;

public class SingleBoolExpr extends BoolExpr {

    private IntExpr left;
    private RelOp op;
    private IntExpr right;

    public SingleBoolExpr(int line, IntExpr left, RelOp op, IntExpr right) {
        super(line);
        this.left = left;
        this.op = op;
        this.right = right;
    }

    public boolean expr() {
        int v1 = left.expr();
        int v2 = right.expr();

        switch (op) {
            case Equal:
                return v1 == v2;
            case NotEqual:
                return v1 != v2;
            case LowerThan:
                return v1 < v2;
            case LowerEqual:
                return v1 <= v2;
            case GreaterThan:
                return v1 > v2;
            case GreaterEqual:
            default:
                return v1 >= v2;
        }
    }
    
}