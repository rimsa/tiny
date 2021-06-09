#include <iostream>

#include <string>
#include <cctype>
#include "LexicalAnalysis.h"

LexicalAnalysis::LexicalAnalysis(const char* filename) : m_line(1) {
	m_input = fopen(filename, "r");
	if (!m_input)
		throw std::string("Unable to open file");
}

LexicalAnalysis::~LexicalAnalysis() {
	if (m_input != nullptr)
		fclose(m_input);
}

Lexeme LexicalAnalysis::nextToken() {
	int state;
	Lexeme lex;

	state = 1;
	while (state != 7 && state != 8) {
		int c = fgetc(m_input);

		// std::cout << "[" << state << ", " << c
		//           << " ('" << (char) c << "')]" << std::endl;

		switch (state) {
			case 1:
				if (c == ' ' || c == '\t' || c == '\r') {
					state = 1;
				} else if (c == '\n') {
					m_line++;
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
				} else if (c == '_' || isalpha(c)) {
					lex.token += (char) c;
					state = 5;
				} else if (isdigit(c)) {
					lex.token += (char) c;
					state = 6;
				} else {
					if (c == -1) {
						lex.type = TT_END_OF_FILE;
						state = 8;
					} else {
						lex.token += (char) c;
						lex.type = TT_INVALID_TOKEN;
						state = 8;
					}
				}

				break;
			case 2:
				if (c == '\n') {
					m_line++;
					state = 1;
				} else if (c == -1) {
					lex.type = TT_END_OF_FILE;
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
					if (c != -1)
						ungetc(c, m_input);

					state = 7;
				}

				break;
			case 4:
				if (c == '=') {
					lex.token += (char) c;
					state = 7;
				} else {
					if (c == -1) {
						lex.type = TT_UNEXPECTED_EOF;
						state = 8;
					} else {
						lex.type = TT_INVALID_TOKEN;
						state = 8;
					}
				}

				break;
			case 5:
				if (c == '_' || isalpha(c) || isdigit(c)) {
					lex.token += (char) c;
					state = 5;
				} else {
					if (c != -1)
						ungetc(c, m_input);

					state = 7;
				}

				break;
			case 6:
				if (isdigit(c)) {
					lex.token += (char) c;
					state = 6;
				} else {
					if (c != -1)
						ungetc(c, m_input);

					lex.type = TT_NUMBER;
					state = 8;
				}

				break;
			default:
				throw std::string("invalid state");
		}
	}

	if (state == 7)
		lex.type = m_st.find(lex.token);

	return lex;
}
