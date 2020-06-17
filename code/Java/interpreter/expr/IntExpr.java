package interpreter.expr;

public abstract class IntExpr {

    private int line;

    public IntExpr(int line) {
        this.line = line;
    }

    public int getLine() {
        return line;
    }

    public abstract int expr();

}