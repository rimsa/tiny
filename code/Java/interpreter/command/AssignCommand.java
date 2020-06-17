package interpreter.command;

import interpreter.expr.IntExpr;
import interpreter.expr.Variable;

public class AssignCommand extends Command {

    private Variable var;
    private IntExpr expr;

    public AssignCommand(int line, Variable var, IntExpr expr) {
        super(line);
        this.var = var;
        this.expr = expr;
    }

    public void execute() {
        int value = expr.expr();
        var.setValue(value);
    }

}