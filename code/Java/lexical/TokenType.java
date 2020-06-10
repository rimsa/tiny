package lexical;

public enum TokenType {
	// Specials
	UNEXPECTED_EOF,
	INVALID_TOKEN,
	END_OF_FILE,

	// Symbols
	SEMICOLON,     // ;
	ASSIGN,        // =

	// Logic operators
	EQUAL,         // ==
	NOT_EQUAL,     // !=
	LOWER,         // <
	LOWER_EQUAL,   // <=
	GREATER,       // >
	GREATER_EQUAL, // >=

	// Arithmetic operators
	ADD,           // +
	SUB,           // -
	MUL,           // *
	DIV,           // /
	MOD,           // %

	// Keywords
	PROGRAM,       // program
	WHILE,         // while
	DO,            // do
	DONE,          // done
	IF,            // if
	THEN,          // then
	ELSE,          // else
	OUTPUT,        // output
	TRUE,          // true
	FALSE,         // false
	READ,          // read
	NOT,           // not

	// Others
	NUMBER,        // number
	VAR            // variable

}
