package lexical;

import java.util.Map;
import java.util.HashMap;

public class SymbolTable {

	private Map<String, TokenType> st;

	public SymbolTable() {
		st = new HashMap<String, TokenType>();

		// TODO: Implement me!
	}

	public boolean contains(String token) {
		return st.containsKey(token);
	}

	public TokenType find(String token) {
		return this.contains(token) ?
			st.get(token) : TokenType.INVALID_TOKEN;
	}
}
