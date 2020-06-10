package lexical;

import java.util.Map;
import java.util.HashMap;

public class SymbolTable {

	private Map<String, TokenType> st;

	public SymbolTable() {
		st = new HashMap<String, TokenType>();

		// Symbols
		st.put(";", TokenType.SEMICOLON);
		st.put("=", TokenType.ASSIGN);

		// Logic operators
		st.put("==", TokenType.EQUAL);
		st.put("!=", TokenType.NOT_EQUAL);
		st.put("<", TokenType.LOWER);
		st.put("<=", TokenType.LOWER_EQUAL);
		st.put(">", TokenType.GREATER);
		st.put(">=", TokenType.GREATER_EQUAL);

		// Arithmetic operators
		st.put("+", TokenType.ADD);
		st.put("-", TokenType.SUB);
		st.put("*", TokenType.MUL);
		st.put("/", TokenType.DIV);
		st.put("%", TokenType.MOD);

		// Keywords
		st.put("program", TokenType.PROGRAM);
		st.put("while", TokenType.WHILE);
		st.put("do", TokenType.DO);
		st.put("done", TokenType.DONE);
		st.put("if", TokenType.IF);
		st.put("then", TokenType.THEN);
		st.put("else", TokenType.ELSE);
		st.put("output", TokenType.OUTPUT);
		st.put("true", TokenType.TRUE);
		st.put("false", TokenType.FALSE);
		st.put("read", TokenType.READ);
		st.put("not", TokenType.NOT);
	}

	public boolean contains(String token) {
		return st.containsKey(token);
	}

	public TokenType find(String token) {
		return this.contains(token) ?
					st.get(token) : TokenType.VAR;
	}
}
