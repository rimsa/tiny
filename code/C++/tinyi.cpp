#include <iostream>
#include <cstdlib>
#include "lexical/LexicalAnalysis.h"
#include "syntatic/SyntaticAnalysis.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Usage: " << argv[0] << " [Tiny program]" << std::endl;
		exit(1);
	}

	try {
		Lexeme lex;
		LexicalAnalysis l(argv[1]);
		SyntaticAnalysis s(l);

		s.start();
	} catch (const std::string& error) {
		std::cerr << "error: " << error << std::endl;
	}

	return 0;
}
