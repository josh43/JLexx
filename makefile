#use make --debug=v
# if you care about the size of the project it is currently
# cat ./*/*h | wc -l ====== 1928 lines
#directories to search for
VPATH = RegularExpression/Lexer:RegularExpression/Parser:RegularExpression/NFAGenerator:RegularExpression/TestVSGrep/Test2:Regex:RegularExpression/DFAGenerator:Regex/TestPackage:JLexx:RegularExpression
CC = g++
FLAGS = -Wall -std=c++14

main : main.o Utility.o
	$(CC) $(FLAGS) main.o Utility.o -o main
main.o:  RegexToDFABuilder.h
	$(CC) $(FLAGS) -c main.cpp
Utility.o: Utility.h
	$(CC) $(FLAGS) -c RegularExpression/Utility.cpp
#Regex.o: Regex.h EpsilonNFAMatcher.h RegularParser.h NFAGenerator.h RegularLexer.h DFAGenerator.h
# $(CC) $(FLAGS) -c RegularExpression/Regex/Regex.cpp





clean:
	rm *.o main
