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
        // TODO: Implement me!
    }

    // <cmdlist>   ::= <cmd> { <cmd> }
    private void procCmdList() {
        // TODO: Implement me!
    }

    // <cmd>       ::= (<assign> | <output> | <if> | <while>) ;
    private void procCmd() {
        // TODO: Implement me!
    }

    // <assign>    ::= <var> = <intexpr>
    private void procAssign() {
        // TODO: Implement me!
    }

    // <output>    ::= output <intexpr>
    private void procOutput() {
        // TODO: Implement me!
    }

    // <if>        ::= if <boolexpr> then <cmdlist> [ else <cmdlist> ] done
    private void procIf() {
        // TODO: Implement me!
    }

    // <while>     ::= while <boolexpr> do <cmdlist> done
    private void procWhile() {
        // TODO: Implement me!
    }

    // <boolexpr>  ::= false | true |
    //                 not <boolexpr> |
    //                 <intterm> (== | != | < | > | <= | >=) <intterm>
    private void procBoolExpr() {
        // TODO: Implement me!
    }

    // <intexpr>   ::= [ + | - ] <intterm> [ (+ | - | * | / | %) <intterm> ]
    private void procIntExpr() {
        // TODO: Implement me!
    }

    // <intterm>   ::= <var> | <const> | read
    private void procIntTerm() {
        // TODO: Implement me!
    }

    // <var>       ::= id
    private void procVar() {
        // TODO: Implement me!
    }

    // <const>     ::= number
    private void procConst() {
        // TODO: Implement me!
    }

}