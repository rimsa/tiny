package interpreter.expr;

public class ConstBoolExpr extends BoolExpr {

    private boolean value;

    public ConstBoolExpr(int line, boolean value) {
        super(line);
        this.value = value;
    }

    public boolean expr() {
        return value;
    }
    
}