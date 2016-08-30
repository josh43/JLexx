//
// Created by joshua on 8/24/16.
//

#include "Generator.h"
#include "Parser.h"


Generator::Generator(const std::string &input){
    LexLoader::Parser p(*this);
    std::string str = FileLoader::fileToString(input.c_str());
    p.match(str,true,false);
}

void Generator::handleData(Lexx::Data &d) {

}
