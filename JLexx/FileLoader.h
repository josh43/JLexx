//
// Created by joshua on 8/23/16.
//

#ifndef JLEXX_FILELOADER_H
#define JLEXX_FILELOADER_H
#include <string>
#include <stdlib.h>
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
        unsigned int fileSize = ftell(f);
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
};
#endif //JLEXX_FILELOADER_H
