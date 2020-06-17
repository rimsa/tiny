package interpreter.expr;

public class BinaryIntExpr extends IntExpr {

    private IntExpr left;
    private IntOp op;
    private IntExpr right;

    public BinaryIntExpr(int line, IntExpr left, IntOp op, IntExpr right) {
        super(line);
        this.left = left;
        this.op = op;
        this.right = right;
    }

    public int expr() {
        int v1 = left.expr();
        int v2 = right.expr();

        switch (op) {
            case Add:
                return v1 + v2;
            case Sub:
                return v1 - v2;
            case Mul:
                return v1 * v2;
            case Div:
                return v1 / v2;
            case Mod:
            default:
                return v1 % v2;
        }
    }
    
}