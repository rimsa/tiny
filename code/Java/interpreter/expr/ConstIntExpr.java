package interpreter.expr;

public class ConstIntExpr extends IntExpr {

    private int value;

    public ConstIntExpr(int line, int value) {
        super(line);
        this.value = value;
    }

    public int expr() {
        return value;
    }

}