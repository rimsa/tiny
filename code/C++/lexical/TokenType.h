#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

#include <string>

enum TokenType {
	// Specials
	TT_UNEXPECTED_EOF = -2,
	TT_INVALID_TOKEN  = -1,
	TT_END_OF_FILE    =  0,

	// Symbols
	TT_SEMICOLON,     // ;
	TT_ASSIGN,        // =

	// Logic operators
	TT_EQUAL,         // ==
	TT_NOT_EQUAL,     // !=
	TT_LOWER,         // <
	TT_LOWER_EQUAL,   // <=
	TT_GREATER,       // >
	TT_GREATER_EQUAL, // >=

	// Arithmetic operators
	TT_ADD,           // +
	TT_SUB,           // -
	TT_MUL,           // *
	TT_DIV,           // /
	TT_MOD,           // %

	// Keywords
	TT_PROGRAM,       // program
	TT_WHILE,         // while
	TT_DO,            // do
	TT_DONE,          // done
	TT_IF,            // if
	TT_THEN,          // then
	TT_ELSE,          // else
	TT_OUTPUT,        // output
	TT_TRUE,          // true
	TT_FALSE,         // false
	TT_READ,          // read
	TT_NOT,           // not

	// Others
	TT_NUMBER,        // number
	TT_VAR            // variable

};

inline std::string tt2str(enum TokenType type) {
	switch (type) {
		// Specials
		case TT_UNEXPECTED_EOF:
			return "UNEXPECTED_EOF";
		case TT_INVALID_TOKEN:
			return "INVALID_TOKEN";
		case TT_END_OF_FILE:
			return "END_OF_FILE";

		// Symbols
		case TT_SEMICOLON:
			return "SEMICOLON";
		case TT_ASSIGN:
			return "ASSIGN";

		// Logic operators
		case TT_EQUAL:
			return "EQUAL";
		case TT_NOT_EQUAL:
			return "NOT_EQUAL";
		case TT_LOWER:
			return "LOWER";
		case TT_LOWER_EQUAL:
			return "LOWER_EQUAL";
		case TT_GREATER:
			return "GREATER";
		case TT_GREATER_EQUAL:
			return "GREATER_EQUAL";

		// Arithmetic operators
		case TT_ADD:
			return "ADD";
		case TT_SUB:
			return "SUB";
		case TT_MUL:
			return "MUL";
		case TT_DIV:
			return "DIV";
		case TT_MOD:
			return "MOD";

		// Keywords
		case TT_PROGRAM:
			return "PROGRAM";
		case TT_WHILE:
			return "WHILE";
		case TT_DO:
			return "DO";
		case TT_DONE:
			return "DONE";
		case TT_IF:
			return "IF";
		case TT_THEN:
			return "THEN";
		case TT_ELSE:
			return "ELSE";
		case TT_OUTPUT:
			return "OUTPUT";
		case TT_TRUE:
			return "TRUE";
		case TT_FALSE:
			return "FALSE";
		case TT_READ:
			return "READ";
		case TT_NOT:
			return "NOT";

		// Others
		case TT_NUMBER:
			return "NUMBER";
		case TT_VAR:
			return "VAR";

		default:
			throw std::string("invalid token type");
	}
}

#endif
