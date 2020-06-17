package syntatic;

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
        System.out.println("Expected (..., " + type + "), found (\"" +
            current.token + "\", " + current.type + ")");
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

    public void start() {
        procProgram();
        eat(TokenType.END_OF_FILE);
    }

    // <program>   ::= program <cmdlist>
    private void procProgram() {
        eat(TokenType.PROGRAM);
        procCmdList();
    }

    // <cmdlist>   ::= <cmd> { <cmd> }
    private void procCmdList() {
        procCmd();
        while (current.type == TokenType.VAR ||
                current.type == TokenType.OUTPUT ||
                current.type == TokenType.IF ||
                current.type == TokenType.WHILE) {
            procCmd();
        }
    }

    // <cmd>       ::= (<assign> | <output> | <if> | <while>) ;
    private void procCmd() {
        if (current.type == TokenType.VAR)
            procAssign();
        else if (current.type == TokenType.OUTPUT)
            procOutput();
        else if (current.type == TokenType.IF)
            procIf();
        else if (current.type == TokenType.WHILE)
            procWhile();
        else
            showError();

        eat(TokenType.SEMICOLON);
    }

    // <assign>    ::= <var> = <intexpr>
    private void procAssign() {
        procVar();
        eat(TokenType.ASSIGN);
        procIntExpr();
    }

    // <output>    ::= output <intexpr>
    private void procOutput() {
        eat(TokenType.OUTPUT);
        procIntExpr();
    }

    // <if>        ::= if <boolexpr> then <cmdlist> [ else <cmdlist> ] done
    private void procIf() {
        eat(TokenType.IF);
        procBoolExpr();
        eat(TokenType.THEN);
        procCmdList();
        if (current.type == TokenType.ELSE) {
            advance();
            procCmdList();
        }
        eat(TokenType.DONE);
    }

    // <while>     ::= while <boolexpr> do <cmdlist> done
    private void procWhile() {
        eat(TokenType.WHILE);
        procBoolExpr();
        eat(TokenType.DO);
        procCmdList();
        eat(TokenType.DONE);
    }

    // <boolexpr>  ::= false | true |
    //                 not <boolexpr> |
    //                 <intterm> (== | != | < | > | <= | >=) <intterm>
    private void procBoolExpr() {
        if (current.type == TokenType.FALSE) {
            advance();
        } else if (current.type == TokenType.TRUE) {
            advance();
        } else if (current.type == TokenType.NOT) {
            advance();
            procBoolExpr();
        } else {
            procIntTerm();

            switch (current.type) {
                case EQUAL:
                case NOT_EQUAL:
                case LOWER:
                case GREATER:
                case LOWER_EQUAL:
                case GREATER_EQUAL:
                    advance();
                    break;
                default:
                    showError();
                    break;
            }

            procIntTerm();
        }
    }

    // <intexpr>   ::= [ + | - ] <intterm> [ (+ | - | * | / | %) <intterm> ]
    private void procIntExpr() {
        if (current.type == TokenType.ADD) {
            advance();
        } else if (current.type == TokenType.SUB) {
            advance();
        }

        procIntTerm();

        if (current.type == TokenType.ADD |
                current.type == TokenType.SUB |
                current.type == TokenType.MUL |
                current.type == TokenType.DIV |
                current.type == TokenType.MOD) {
            switch (current.type) {
                case ADD:
                case SUB:
                case MUL:
                case DIV:
                case MOD:
                    advance();
                    break;
                default:
                    // unrecheable
                    break;
            }

            procIntTerm();
        }
    }

    // <intterm>   ::= <var> | <const> | read
    private void procIntTerm() {
        if (current.type == TokenType.VAR) {
            procVar();
        } else if (current.type == TokenType.NUMBER) {
            procConst();
        } else {
            eat(TokenType.READ);
        }
    }

    // <var>       ::= id
    private void procVar() {
        eat(TokenType.VAR);
    }

    // <const>     ::= number
    private void procConst() {
        eat(TokenType.NUMBER);
    }

}