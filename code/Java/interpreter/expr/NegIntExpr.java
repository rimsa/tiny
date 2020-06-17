package interpreter.expr;

public class NegIntExpr extends IntExpr {

    private IntExpr expr;

    public NegIntExpr(int line, IntExpr expr) {
        super(line);
        this.expr = expr;
    }

    public int expr() {
        return -expr.expr();
    }

}