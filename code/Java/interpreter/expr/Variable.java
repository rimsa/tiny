package interpreter.expr;

import java.util.HashMap;
import java.util.Map;

public class Variable extends IntExpr {
    
    public static Map<String, Variable> variables;

    static {
        variables = new HashMap<String, Variable>();
    }

    private String name;
    private int value;

    private Variable(String name) {
        super(-1);

        this.name = name;
        this.value = 0;
    }

    public String getName() {
        return name;
    }

    public int expr() {
        return value;
    }

    public void setValue(int value) {
        this.value = value;
    }

    public static Variable instance(String name) {
        Variable v = variables.get(name);
        if (v == null) {
            v = new Variable(name);
            variables.put(name, v);
        }

        return v;
    }

}