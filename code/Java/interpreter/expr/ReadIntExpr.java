package interpreter.expr;

import java.util.Scanner;

public class ReadIntExpr extends IntExpr {

    private static Scanner in;
    
    static {
        in = new Scanner(System.in);
    }

    public ReadIntExpr(int line) {
        super(line);
    }

    public int expr() {
        try {
            String str = in.nextLine();
            int n = Integer.parseInt(str);
            return n;
        } catch (Exception e) {
            return 0;
        }
    }

}