//
// Created by josh on 9/10/16.
//

#ifndef JLEXX_THEREALJLEXX_H
#define JLEXX_THEREALJLEXX_H

#include "../RegexToDFABuilder.h"
#include "GenFiles/BridgeHeader.h"
#include "../FileLoader.h"
//#include "BridgeHeader.h"


namespace Lexx{



    class TheRealJLexx : public RegexToDFABuilder , public DataHandler{

    private:
        std::string myString;
        std::vector<string> funcNameList;
        unsigned int curr;
        std::vector<std::pair<JVAL,TokenType > > jTokens;
        FILE * stdinFD;
        bool verbose;
    protected:
    public:





        TheRealJLexx(bool verbose = false) : RegexToDFABuilder() {

            stdin = NULL;
            this->verbose = verbose;


            for(auto pair : reFuncList){
                RegexToDFABuilder::addToList(pair.first);
            }
            RegexToDFABuilder::create(false);
        }

        void matchSTDIN(){


            FileLoader::stdinToString(this->myString,false);
            this->matchFromSTDIN();
        }

        bool endOfInput(){
            return  (curr >= this->jTokens.size());
        }
        virtual void handleData(Data &d) override {
           // jmatch.text = &myString[0];
           // jmatch.start = d.startOfMatch;
           // jmatch.end = d.endOfMatch;

            jmatch = {&myString[0],d.startOfMatch,d.endOfMatch};
            //uint temp  = reFuncList[d.regexNumber].second();
            jTokens.push_back({jval,(TokenType)reFuncList[d.regexNumber].second()});
        }



        bool match(string s){
            this->myString = std::move(s);

            bool res = RegexToDFABuilder::match(myString, false, false, this);
            return res;

        }

        const std::pair<JVAL,TokenType> & jlnext(){
            if(curr >= this->jTokens.size()){
                throw std::invalid_argument("Error triyng to get the next token when there is not one!! user endOfInpu()\n");
            }
            return jTokens[curr++];
        }




        virtual ~TheRealJLexx() {

        }

        virtual bool shouldStop() override {
            return false;
        }
    private:
        void matchFromSTDIN(){
            jmatch.text = &myString[0];



            RegexToDFABuilder::match(myString, false,false, this);

        }

    };




}

static Lexx::TheRealJLexx jl;
void matchSTDIN(){

    jl.matchSTDIN();
}
bool endOfInput(){
    return jl.endOfInput();
}
const std::pair<JVAL,TokenType> & jlnext(){
    return jl.jlnext();
}
#endif //JLEXX_THEREALJLEXX_H
