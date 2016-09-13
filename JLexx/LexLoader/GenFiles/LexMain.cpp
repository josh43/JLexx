#include "../TheRealJLexx.h"


void handle(uint type){
            switch(type){
                case NUMB:{
                    break;
                }case MINUS:{
                    break;
                }case WORD:{
                    wordCount++;
                    break;
                }case '\n':{
                    lineCount++;
                }
            }
}
// If main is defined than JYacc will not be used
int main(){



    matchSTDIN();

// JVAL is the type but you use jval when passing information!
    while(!endOfInput()){

        std::pair<JVAL,TokenType> next = jlnext();
        handle(next.second);

    }
    printf("The linecount was %i and the wc was %i so cool!\n",lineCount,wordCount);

    return 0;

}


