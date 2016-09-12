#use make --debug=v
# if you care about the size of the project it is currently
# cat ./*/*h | wc -l ====== 1928 lines
#directories to search for
VPATH = RegularExpression/Lexer:RegularExpression/Parser:RegularExpression/NFAGenerator:RegularExpression/TestVSGrep/Test2:Regex:RegularExpression/DFAGenerator:Regex/TestPackage:JLexx:RegularExpression
CC = g++
FLAGS = -Wall -std=c++11

JLex : LexGenMain.o Utility.o
	$(CC) $(FLAGS) LexGenMain.o Utility.o -o LexGenMain
LexGenMain.o:  ../RegexToDFABuilder.h LexGenerator.h
	$(CC) $(FLAGS) -c LexGenMain.cpp
Utility.o: ../../RegularExpression/Utility.h
	$(CC) $(FLAGS) -c ../../RegularExpression/Utility.cpp
clean:
	rm *.o LexGenMain