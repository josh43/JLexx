//
// Created by joshua on 8/23/16.
//

#ifndef JLEXX_FILELOADER_H
#define JLEXX_FILELOADER_H
#include <string>
#include <stdlib.h>
#include <string.h>
class FileLoader{
public:
    static std::string  fileToString(const std::string & fileName,bool verbose = true) {

        FILE *f = fopen(fileName.c_str(), "r");
        if (f == nullptr) {

            printf("Error opening file %s\n", fileName.c_str());
            perror("");
            exit(0);
        }
        fseek(f, 0, SEEK_END);
        unsigned int fileSize = 0;
        fileSize = ftell(f);
        rewind(f);
        std::string toReturn;
        toReturn.resize(fileSize * sizeof(char));
        fread(&toReturn[0], 1,fileSize, f);
        fclose(f);
        if(verbose) {
            printf("Input string %s\n", toReturn.c_str());
        }

        return toReturn;
    }

    static std::string stdingToString(bool verbose = true){
        std::string toReturn = "";
        stdinToString(toReturn);
        return toReturn;
    }
    static void stdinToString( std::string & buffer,bool verbose = true) {

        buffer = "";
        buffer.reserve(4096);
        char buff[512];

        memset(buff,0,sizeof(buff));

        uint readSize =0;
        while((readSize = read(0,buff,sizeof(buff))) > 0){
            buffer.append(buff,readSize);
            memset(buff,0,sizeof(buff));
            readSize = 0;

        }

        buffer.push_back('\0');



        if(verbose){
            printf("%s \n End reading from stdin\n",buffer.c_str());
        }



    }


};
#endif //JLEXX_FILELOADER_H
