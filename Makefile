# Format: src/lib/ParserB.cpp	src/lib/Lexer.cpp	src/format.cpp
# 	g++	-std=c++17	-Wall	-Wextra	-Werror	src/lib/ParserB.cpp	src/lib/Lexer.cpp	src/format.cpp -o Format

# Format:	format.o
# 	g++ -std=c++17 -Wall -Wextra -Werror format.o -o Format

# format.o:	src/lib/ParserB.cpp	src/lib/Lexer.cpp	src/format.cpp
# 	g++	-std=c++17	-Wall	-Wextra	-Werror	-c	src/lib/ParserB.cpp	src/lib/Lexer.cpp	src/format.cpp

# clean:
# 	rm	*.o	Format

# Format:	format.o
# 	g++	-std=c++17	-Wall	-Wextra	-Werror	format.o	-o	Format

# format.o:	src/lib/ParserB.cpp	src/lib/Lexer.cpp	src/format.cpp
# 	g++	-std=c++17	-Wall	-Wextra	-Werror	-c	src/lib/ParserB.cpp	src/lib/Lexer.cpp	src/format.cpp