# Format: src/lib/ParserB.cpp	src/lib/Lexer.cpp	src/format.cpp
# 	g++	-std=c++17	-Wall	-Wextra	-Werror	src/lib/ParserB.cpp	src/lib/Lexer.cpp	src/format.cpp -o Format
CXX = g++
CXXFLAGS = -Wall -Wextra --std=c++17
DEBUG = -g -O0
all: Format Calc

Format:	format.o ParserB.o Lexer.o
	g++ -std=c++17 -Wall -Wextra -Werror $^  -o Format

Calc: Calc.o ParserB.o Lexer.o
	g++ -std=c++17 -Wall -Wextra -Werror $^  -o Calc

Calc.o: src/calc.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@
ParserB.o: src/lib/ParserB.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@
Lexer.o: src/lib/Lexer.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@
format.o: src/format.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $< -o $@

clean:
	rm	-f *.o	Format Calc

# Format:	format.o
# 	g++	-std=c++17	-Wall	-Wextra	-Werror	format.o	-o	Format

# format.o:	src/lib/ParserB.cpp	src/lib/Lexer.cpp	src/format.cpp
# 	g++	-std=c++17	-Wall	-Wextra	-Werror	-c	src/lib/ParserB.cpp	src/lib/Lexer.cpp	src/format.cpp