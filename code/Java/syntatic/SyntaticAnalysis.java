package syntatic;

import interpreter.command.AssignCommand;
import interpreter.command.BlocksCommand;
import interpreter.command.Command;
import interpreter.command.IfCommand;
import interpreter.command.OutputCommand;
import interpreter.command.WhileCommand;
import interpreter.expr.BinaryIntExpr;
import interpreter.expr.BoolExpr;
import interpreter.expr.ConstBoolExpr;
import interpreter.expr.ConstIntExpr;
import interpreter.expr.IntExpr;
import interpreter.expr.IntOp;
import interpreter.expr.NegIntExpr;
import interpreter.expr.NotBoolExpr;
import interpreter.expr.ReadIntExpr;
import interpreter.expr.RelOp;
import interpreter.expr.SingleBoolExpr;
import interpreter.expr.Variable;
import lexical.Lexeme;
import lexical.LexicalAnalysis;
import lexical.TokenType;

public class SyntaticAnalysis {

    private LexicalAnalysis lex;
    private Lexeme current;

    public SyntaticAnalysis(LexicalAnalysis lex) {
        this.lex = lex;
        this.current = lex.nextToken();
    }

    private void advance() {
        current = lex.nextToken();
    }

    private void eat(TokenType type) {
        // System.out.println("Expected (..., " + type + "), found (\"" +
        //     current.token + "\", " + current.type + ")");
        if (type == current.type) {
            advance();
        } else {
            showError();
        }
    }

    private void showError() {
        System.out.printf("%02d: ", lex.getLine());

        switch (current.type) {
            case INVALID_TOKEN:
                System.out.printf("Lexema inválido [%s]\n", current.token);
                break;
            case UNEXPECTED_EOF:
            case END_OF_FILE:
                System.out.printf("Fim de arquivo inesperado\n");
                break;
            default:
                System.out.printf("Lexema não esperado [%s]\n", current.token);
                break;
        }

        System.exit(1);
    }

    public BlocksCommand start() {
        BlocksCommand cmds = procProgram();
        eat(TokenType.END_OF_FILE);

        return cmds;
    }

    // <program>   ::= program <cmdlist>
    private BlocksCommand procProgram() {
        eat(TokenType.PROGRAM);
        return procCmdList();
    }

    // <cmdlist>   ::= <cmd> { <cmd> }
    private BlocksCommand procCmdList() {
        // There is no precise way to track the line number
        // for block commands, thus using -1.
        BlocksCommand cb = new BlocksCommand(-1);

        Command c = procCmd();
        cb.addCommand(c);
        while (current.type == TokenType.VAR ||
                current.type == TokenType.OUTPUT ||
                current.type == TokenType.IF ||
                current.type == TokenType.WHILE) {
            c = procCmd();
            cb.addCommand(c);
        }

        return cb;
    }

    // <cmd>       ::= (<assign> | <output> | <if> | <while>) ;
    private Command procCmd() {
        Command c = null;
        if (current.type == TokenType.VAR)
            c = procAssign();
        else if (current.type == TokenType.OUTPUT)
            c = procOutput();
        else if (current.type == TokenType.IF)
            c = procIf();
        else if (current.type == TokenType.WHILE)
            c = procWhile();
        else
            showError();

        eat(TokenType.SEMICOLON);

        return c;
    }

    // <assign>    ::= <var> = <intexpr>
    private AssignCommand procAssign() {
        Variable var = procVar();
        int line = lex.getLine();

        eat(TokenType.ASSIGN);
        IntExpr expr = procIntExpr();

        return new AssignCommand(line, var, expr);
    }

    // <output>    ::= output <intexpr>
    private OutputCommand procOutput() {
        eat(TokenType.OUTPUT);
        int line = lex.getLine();

        IntExpr expr = procIntExpr();

        return new OutputCommand(line, expr);
    }

    // <if>        ::= if <boolexpr> then <cmdlist> [ else <cmdlist> ] done
    private IfCommand procIf() {
        eat(TokenType.IF);
        int line = lex.getLine();

        BoolExpr cond = procBoolExpr();
        eat(TokenType.THEN);
        BlocksCommand thenCmds = procCmdList();

        BlocksCommand elseCmds = null;
        if (current.type == TokenType.ELSE) {
            advance();
            elseCmds = procCmdList();
        }
        eat(TokenType.DONE);

        return new IfCommand(line, cond, thenCmds, elseCmds);
    }

    // <while>     ::= while <boolexpr> do <cmdlist> done
    private WhileCommand procWhile() {
        eat(TokenType.WHILE);
        int line = lex.getLine();

        BoolExpr cond = procBoolExpr();
        eat(TokenType.DO);
        Command cmds = procCmdList();
        eat(TokenType.DONE);

        return new WhileCommand(line, cond, cmds);
    }

    // <boolexpr>  ::= false | true |
    //                 not <boolexpr> |
    //                 <intterm> (== | != | < | <= | > | >=) <intterm>
    private BoolExpr procBoolExpr() {
        if (current.type == TokenType.FALSE) {
            advance();
            return new ConstBoolExpr(lex.getLine(), false);
        } else if (current.type == TokenType.TRUE) {
            advance();
            return new ConstBoolExpr(lex.getLine(), true);
        } else if (current.type == TokenType.NOT) {
            advance();
            int line = lex.getLine();
            BoolExpr tmp = procBoolExpr();
            return new NotBoolExpr(line, tmp);
        } else {
            IntExpr left = procIntTerm();
            int line = lex.getLine();

            RelOp op  = null;
            switch (current.type) {
                case EQUAL:
                    op = RelOp.Equal;
                    break;
                case NOT_EQUAL:
                    op = RelOp.NotEqual;
                    break;
                case LOWER:
                    op = RelOp.LowerThan;
                    break;
                case LOWER_EQUAL:
                    op = RelOp.LowerEqual;
                    break;
                case GREATER:
                    op = RelOp.GreaterThan;
                    break;
                case GREATER_EQUAL:
                    op = RelOp.GreaterEqual;
                    break;
                default:
                    showError();
                    break;
            }

            advance();

            IntExpr right = procIntTerm();

            return new SingleBoolExpr(line, left, op, right);
        }
    }

    // <intexpr>   ::= [ + | - ] <intterm> [ (+ | - | * | / | %) <intterm> ]
    private IntExpr procIntExpr() {
        boolean isNegative = false;
        if (current.type == TokenType.ADD) {
            advance();
        } else if (current.type == TokenType.SUB) {
            advance();
            isNegative = true;
        }

        IntExpr left;
        if (isNegative) {
            int line = lex.getLine();
            IntExpr tmp = procIntTerm();
            left = new NegIntExpr(line, tmp);
        } else {
            left = procIntTerm();
        }

        if (current.type == TokenType.ADD |
                current.type == TokenType.SUB |
                current.type == TokenType.MUL |
                current.type == TokenType.DIV |
                current.type == TokenType.MOD) {
            int line = lex.getLine();

            IntOp op;
            switch (current.type) {
                case ADD:
                    op = IntOp.Add;
                    break;
                case SUB:
                    op = IntOp.Sub;
                    break;
                case MUL:
                    op = IntOp.Mul;
                    break;
                case DIV:
                    op = IntOp.Div;
                    break;
                case MOD:
                default:
                    op = IntOp.Mod;
                    break;
            }

            advance();

            IntExpr right = procIntTerm();

            return new BinaryIntExpr(line, left, op, right);
        } else {
            return left;
        }
    }

    // <intterm>   ::= <var> | <const> | read
    private IntExpr procIntTerm() {
        if (current.type == TokenType.VAR) {
            return procVar();
        } else if (current.type == TokenType.NUMBER) {
            return procConst();
        } else {
            eat(TokenType.READ);
            int line = lex.getLine();
            return new ReadIntExpr(line);
        }
    }

    // <var>       ::= id
    private Variable procVar() {
        String name = current.token;
        eat(TokenType.VAR);

        return Variable.instance(name);
    }

    // <const>     ::= number
    private ConstIntExpr procConst() {
        String tmp = current.token;
        eat(TokenType.NUMBER);
        int line = lex.getLine();

        int number;
        try {
            number = Integer.parseInt(tmp);
        } catch (Exception e) {
            number = 0;
        }

        return new ConstIntExpr(line, number);
    }

}