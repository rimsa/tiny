package lexical;

public class Lexeme {

	// public variables for easy access.
	public String token;
	public TokenType type;

	public Lexeme(String token, TokenType type) {
		this.token = token;
		this.type = type;
	}

}
