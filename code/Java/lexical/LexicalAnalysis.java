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

		// TODO: Implement me!

		return lex;
	}

}
