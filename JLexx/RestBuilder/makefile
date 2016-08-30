

#use make --debug=v
# if you care about the size of the project it is currently
# cat ./*/*h | wc -l ======
#directories to search for
VPATH = ../../RegularExpression/Lexer:../../RegularExpression/Parser:../../RegularExpression/NFAGenerator:../../RegularExpression/TestVSGrep/Test2:../../Regex:RegularExpression/DFAGenerator:../../Regex/TestPackage:../JLexx:RegularExpression
CC = g++
FLAGS = -Wall -std=c++14

main : RestMain.o Utility.o
	$(CC) $(FLAGS) RestMain.o Utility.o -o main
RestMain.o:  RestBuilder.h
	$(CC) $(FLAGS) -c RestMain.cpp
Utility.o: ../../RegularExpression/Utility.h
	$(CC) $(FLAGS) -c ../../RegularExpression/Utility.cpp
#Regex.o: Regex.h EpsilonNFAMatcher.h RegularParser.h NFAGenerator.h RegularLexer.h DFAGenerator.h
# $(CC) $(FLAGS) -c RegularExpression/Regex/Regex.cpp





clean:
	rm *.o main
