#use make --debug=v
# if you care about the size of the project it is currently
# cat ./*/*h | wc -l ====== 1928 lines
#directories to search for
VPATH = GenFiles:RegularExpression/Lexer:RegularExpression/Parser:RegularExpression/NFAGenerator:RegularExpression/TestVSGrep/Test2:Regex:RegularExpression/DFAGenerator:Regex/TestPackage:JLexx:RegularExpression
CC = g++
FLAGS = -Wall -std=c++11

JLex : LexMain.o Utility.o
	$(CC) $(FLAGS) LexMain.o Utility.o -o LexMain
LexMain.o:  ../../RegexToDFABuilder.h ../LexGenerator.h ../GenFiles/BridgeHeader.h ../TheRealJLexx.h
	$(CC) $(FLAGS) -c ../GenFiles/LexMain.cpp
Utility.o: ../../../RegularExpression/Utility.h
	$(CC) $(FLAGS) -c ../../../RegularExpression/Utility.cpp
clean:
	rm *.o LexMain