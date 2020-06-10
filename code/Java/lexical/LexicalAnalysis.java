package lexical;

import java.io.FileInputStream;
import java.io.PushbackInputStream;

public class LexicalAnalysis implements AutoCloseable {

	private int line;
	private SymbolTable st;
	private PushbackInputStream input;

	public LexicalAnalysis(String filename) {
		try {
			input = new PushbackInputStream(new FileInputStream(filename));
		} catch (Exception e) {
			throw new LexicalException("Unable to open file: " + filename);
		}

		st = new SymbolTable();
		line = 1;
	}

	public void close() {
		try {
			input.close();
		} catch (Exception e) {
			throw new LexicalException("Unable to close file");
		}
	}

	public int getLine() {
		return this.line;
	}

	public Lexeme nextToken() {
		Lexeme lex = new Lexeme("", TokenType.END_OF_FILE);

		int state = 1;
		while (state != 7 && state != 8) {
			int c = getc();
			switch (state) {
				case 1:
					if (c == ' ' || c == '\r' || c == '\t') {
						state = 1;
					} else if (c == '\n') {
						line++;
						state = 1;
					} else if (c == '#') {
						state = 2;
					} else if (c == '=' || c == '<' || c == '>') {
						lex.token += (char) c;
						state = 3;
					} else if (c == '!') {
						lex.token += (char) c;
						state = 4;
					} else if (c == ';' || c == '+' || c == '-' ||
							   c == '*' || c == '/' || c == '%') {
						lex.token += (char) c;
						state = 7;
					} else if (Character.isLetter(c) || c == '_') {
						lex.token += (char) c;
						state = 5;
					} else if (Character.isDigit(c)) {
						lex.token += (char) c;
						state = 6;
					} else if (c == -1) {
						lex.type = TokenType.END_OF_FILE;
						state = 8;
					} else {
						lex.token += (char) c;
						lex.type = TokenType.INVALID_TOKEN;
						state = 8;
					}

					break;
				case 2:
					if (c == '\n') {
						line++;
						state = 1;
					} else if (c == -1) {
						lex.type = TokenType.END_OF_FILE;
						state = 8;
					} else {
						state = 2;
					}

					break;
				case 3:
					if (c == '=') {
						lex.token += (char) c;
						state = 7;
					} else {
						ungetc(c);
						state = 7;
					}

					break;
				case 4:
					if (c == '=') {
						lex.token += (char) c;
						state = 7;
					} else if (c == -1) {
						lex.type = TokenType.UNEXPECTED_EOF;
						state = 8;
					} else {
						lex.type = TokenType.INVALID_TOKEN;
						state = 8;
					}

					break;
				case 5:
					if (Character.isLetter(c) ||
							Character.isDigit(c) ||
							c == '_') {
						lex.token += (char) c;
						state = 5;
					} else {
						ungetc(c);
						state = 7;
					}

					break;
				case 6:
					if (Character.isDigit(c)) {
						lex.token += (char) c;
						state = 6;
					} else {
						ungetc(c);
						lex.type = TokenType.NUMBER;
						state = 8;
					}

					break;
				default:
					throw new LexicalException("Unreachable");
			}
		}

		if (state == 7)
			lex.type = st.find(lex.token);

		return lex;
	}

	private int getc() {
		try {
			return input.read();
		} catch (Exception e) {
			throw new LexicalException("Unable to read file");
		}
	}

	private void ungetc(int c) {
		if (c != -1) {
			try {
				input.unread(c);
			} catch (Exception e) {
				throw new LexicalException("Unable to ungetc");
			}
		}
	}
}
