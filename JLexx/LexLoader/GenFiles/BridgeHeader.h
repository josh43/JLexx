#include <iostream>
#include <vector>
struct{
    char * text;
    unsigned int start;
    unsigned int end;
}jmatch;
/*
Make sure you have cd'd into the InstalDir and have done sudo ./install.sh
Than to compile an input like this one run
jlGen JLexxExample.txt

and to run the lexer on input just do
jlGen exInput.txt

*/

// From here all the way down tocan be any valid c/c++!!
#include <stdlib.h>
#include <string.h>

// put whatever you want
uint unique;
uint wordCount =0;
uint lineCount =0;
// insert very useful function ... :|
void helloWorld(){
 printf("Hello world");
}
/* TYPE must be called JVAL but the contents of the union can by whatever are accepted by c++*/
union JVAL {
    int x;
    int y;
    char * text;
}jval;

/* instance must be called jval */

/*
    When matched this value will be filled
    struct {
        char * text;  // this will only be a pointer to the start of the match
        uint start;   // thus to get the text you want to substr(text,start,end)
        uint end;     // It is this way to allow more customization later on!
    }jmatch;

*/



// There will be a 256 offset because I want to allow returning of characters
// So NUM  will be defined as  1 << 8 + 1
// So WORD will be defined as  1 << 8 + 2

// BEGIIN TOKENS without the second I is a keyword and is used to define enums to be returned

enum TokenType : unsigned int {

 	NUMB         = 256,
	 MINUS          ,
	 WORD           ,
	 COUNT

 };//End TokenType def;
uint jre0(){
                                        printf("matched a newline\n");return jmatch.text[0];
                                    }

uint jre1(){
                                        printf("matched a simple character\n");
                                        return jmatch.text[0];
                                    }

uint jre2(){
                                        printf("matched a minus\n");
                                        return MINUS;
                                    }

uint jre3(){
                                      printf("matched a word\n");jval.text = strndup(jmatch.text,jmatch.end-jmatch.start); // Set JVal
                                      return WORD;
                                    }

uint jre4(){
                                        printf("matched an integer\n");
                                        jval.x = atoi(jmatch.text);
                                        return NUMB;
                                    }

uint jre5(){ return NUMB;}

uint jre6(){return jmatch.text[0];}
std::vector<std::pair<std::string,uint (*)()> > reFuncList =
{
	{"[\n]",      jre0},
	{"(\\)+|\\.|\\-|}|{|\\||[\\\\])",      jre1},
	{"\\-",      jre2},
	{"[a-zA-Z][a-zA-Z0-9]+",      jre3},
	{"[0-9]+",      jre4},
	{"[@]+",      jre5},
	{".",      jre6}
};