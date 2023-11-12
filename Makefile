Format: src/lib/ParserB.o src/lib/Lexer.o
	g++ -std=c++17 -Wall -Wextra -Werror src/lib/ParserB.o src/lib/Lexer.o -o Format

ParserB.o: src/lib/ParserB.cpp
	g++ -c src/lib/ParserB.cpp -std=c++17 -Wall -Wextra -Werror 

Lexer.o: src/lib/Lexer.cpp
	g++ -c src/lib/Lexer.cpp -std=c++17 -Wall -Wextra -Werror

clean:
	rm *.o Format

