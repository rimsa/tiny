package interpreter.command;

import interpreter.expr.IntExpr;

public class OutputCommand extends Command {

    private IntExpr expr;

    public OutputCommand(int line, IntExpr expr) {
        super(line);
        this.expr = expr;
    }

    public void execute() {
        System.out.println(expr.expr());
    }

}