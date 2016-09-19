//
// Created by josh on 9/11/16.
//

#include "LexGenerator.h"
#include "../FileLoader.h"

/*
 *  argv[1] debug
 *  argv[2] stdin
 *
 */


int main(int argc, char * argv[]) {


    bool debugging = false;
    bool readFromStdin = false;
    bool ideInput = false;
    if(argc >1){
        if(strcmp(argv[1],"--debug=true") == 0){
            debugging = true;
        }
    }
    if(argc > 2){
        if(strcmp(argv[2],"--stdin=true") == 0){
            readFromStdin = true;
        }
    }
    if(argc > 3){
        if(strcmp(argv[3],"--ideInput=true") == 0){
            ideInput = true;
        }
    }

    JLexx::LexGenerator sL(debugging,ideInput);
    std::string input;
    if(readFromStdin){
        sL.match(FileLoader::stdingToString(debugging));

    }else{
        if(ideInput) {
            sL.match(FileLoader::fileToString(
                    "/home/josh/ClionProjects/JLexx/JLexx/LexLoader/TestFiles/AsciiHTMLParser.jlx", debugging));
        }
    }

    //jmain();
    return 0;
}