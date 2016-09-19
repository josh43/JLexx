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


    /*
     void compress(bool compress, bool debug = false);
     void matchSTDIN(bool stream = false,bool debug = false)
     If you put stream = true, then there is no need to call matchSTDIN()
     compress makes the DFA compressed which saved a ton of memory, on simple DFA's for example
     the one I use to parse this input, I had a compression ratio of 20, so I used about 5% as much mem as normal
     The downfall is that when moving on states turns into
     vector<vector<uint> > jumpTable
     jumpTable[currentSate][inputCharacter] == some state
     but on compression its
     u_char compressedIndex = compressState.binSearch(inputCharacter);
     return jumpTable[currentState][compressedIndex];

     But it could still potentially be faster
    */
    // note it defaults to not compressing
    // it will  also print out compression information
    // you can grep it and search for
    // "COMPRESSION RATIO -> Total space saved"


    compress(true,true);
    matchSTDIN(true,true);
    // turns all debug flags on
    setVerbose(true);
// JVAL is the type but you use jval when passing information!
    std::pair<JVAL,TokenType> next = jlnext();
    // get the first token to check for inital endOfInput
    printf("About to read input");
    while(!endOfInput()){

        handle(next.second);
        next = jlnext();
    }
    if(next.second != std::numeric_limits<uint>::max()){
        // check the last token It might be valid
        handle(next.second);
    }
    printf("The linecount was %i and the wc was %i so cool!\n",lineCount,wordCount);

    return 0;

}