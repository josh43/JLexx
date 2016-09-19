#include "../TheRealJLexx.h"



int main(){




    matchSTDIN(true);
    // turns all debug flags on
    setVerbose(true);
    std::pair<JVAL,TokenType> next = jlnext();
    printf("About to read input");
    while(!endOfInput()){
        next = jlnext();
    }
    if(next.second != std::numeric_limits<uint>::max()){
    }

    return 0;

}