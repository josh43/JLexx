//
// Created by josh on 9/11/16.
//

#include "LexGenerator.h"
#include "../FileLoader.h"



int main(int argc, char * argv[]) {
    bool debugging = true;
    if(argc > 1){
        debugging = false;
    }
    Lexx::LexGenerator sL(debugging);
    std::string input;
    if(debugging){

        sL.match(FileLoader::fileToString("/home/josh/ClionProjects/JLexx/JLexx/LexLoader/a.txt",debugging));
    }else {
        // read from stdin
        sL.match(FileLoader::stdingToString(debugging));
    }
    //jmain();
    return 0;
}