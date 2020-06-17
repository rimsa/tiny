package interpreter.expr;

public abstract class BoolExpr {

    private int line;

    public BoolExpr(int line) {
        this.line = line;
    }

    public int getLine() {
        return line;
    }

    public abstract boolean expr();

}