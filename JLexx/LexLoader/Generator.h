//
// Created by joshua on 8/23/16.
//

#ifndef JLEXX_GENERATOR_H
#define JLEXX_GENERATOR_H
#include <string>

#include "../FileLoader.h"
#include "../MatchHandler.h"

struct jval{
    int ID;
    union{
        int number;
        char * text;
    };
};
class Parser;

    class Generator : public Lexx::DataHandler {
    public:

        Generator(const std::string &input);


        virtual void handleData(Lexx::Data &d);
    };


#endif //JLEXX_GENERATOR_H
