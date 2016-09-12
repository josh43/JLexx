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
        unsigned int tokenListLength;
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
            return  (curr >= (tokenListLength-1));
        }
        virtual void handleData(Data &d) override {

            jmatch.start = d.startOfMatch;
            jmatch.end = d.endOfMatch;

            //uint temp  = reFuncList[d.regexNumber].second();
            jTokens.push_back({jval,(TokenType)reFuncList[d.regexNumber].second()});
        }



        bool match(string s){
            jmatch.text = &myString[0];
            this->myString = std::move(s);
             bool res = RegexToDFABuilder::match(myString, false, false, this);
            tokenListLength = jTokens.size();
            return res;

        }

        const std::pair<JVAL,TokenType> & jlnext(){
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
            printf("\nAbout to use the dfa to match against %s\n",myString.c_str());
            printf("\nProceeding\n");

            RegexToDFABuilder::match(myString, true,false, this);

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
