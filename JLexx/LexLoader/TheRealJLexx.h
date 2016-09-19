//
// Created by josh on 9/10/16.
//

#ifndef JLEXX_THEREALJLEXX_H
#define JLEXX_THEREALJLEXX_H

#include "../RegexToDFABuilder.h"
#include "GenFiles/BridgeHeader.h"
#include "../FileLoader.h"
#include <stdio.h>
//#include "BridgeHeader.h"


static std::vector<JLexx::State >  passInStateList = {JLexx::State{},JLexx::State{}};


namespace JLexx{



    class TheRealJLexx : public RegexToDFABuilder , public DataHandler{

    private:
        std::string myString;
        std::vector<string> funcNameList;
        unsigned int curr;
        std::vector<std::pair<JVAL,TokenType > > jTokens;
        std::pair<JVAL,TokenType> builtStreamToken;
        bool foundToken;
        uint streamCurr;
        uint streamCurr;
        char buff[1024];
        FILE * stdinFD;
        bool matchOne;
        bool reachedEOF;
    protected:
    public:
        bool isStreaming;
        bool verbose;





        TheRealJLexx(std::vector<JLexx::State > && sList,
                     bool verbose = false) :RegexToDFABuilder(std::move(sList),nullptr)
        {


            memset(buff,0,sizeof(buff));
            verbose = false;
            reachedEOF = false;
            isStreaming = false;
            matchOne = foundToken =false;
        }
        TheRealJLexx(bool verbose = false) : RegexToDFABuilder() {

            //stdin = NULL;
            foundToken = false;
            this->verbose = verbose;
            matchOne =false;

            for(auto pair : reFuncList){
                RegexToDFABuilder::addToList(pair.first);
            }
            RegexToDFABuilder::create(false);
        }

        void matchSTDIN(bool debug){


            FileLoader::stdinToString(this->myString,verbose | debug);
            this->matchFromSTDIN(verbose | debug);
        }
        // can only do from stdin


        // streaming doesnt work atm :|
        std::pair<JVAL,TokenType> streamNextToken(){
            if(verbose){
                ENTERING_FUNCTION();
            }

            int in;

            while((in = getchar()) !=  EOF){
                if(verbose){
                    printf("Made it past getchar()\n");
                }
                if(!this->readChar((u_char)in)){
                    if(foundToken){
                        JAssert(ungetc(in,stdin) == EOF,"Error pushing character back on stream\n");
                        foundToken = false;
                        if(verbose){
                            LEAVING_FUNCTION();
                        }
                        return builtStreamToken;
                    }
                }// else we are matching continue to match


            }
            reachedEOF = true;
            builtStreamToken.second = (TokenType )std::numeric_limits<uint>::max();


            if(verbose){
                LEAVING_FUNCTION();
            }

            return builtStreamToken;

        }

        bool endOfInput(){
            if(isStreaming){
                return reachedEOF;
            }else {
                return (curr >= this->jTokens.size());
            }
        }
        virtual void handleData(Data &d) override {


            jmatch = {&myString[0],d.startOfMatch,d.endOfMatch};
            //uint temp  = reFuncList[d.regexNumber].second();
            jTokens.push_back({jval,(TokenType)reFuncList[d.regexNumber].second()});
        }

        virtual void streamToken( const std::string & s,uint regexMatch) override {
            myString = s;
            jmatch = {&myString[0],0,(uint)s.size()};
            foundToken = true;
            builtStreamToken = {jval,(TokenType)reFuncList[regexMatch].second()};
        }



        bool match(string s,bool debug = false){
            this->myString = std::move(s);

            bool res = RegexToDFABuilder::match(myString, verbose | debug, false, this);
            return res;

        }

         std::pair<JVAL,TokenType>  jlnext(){
             if(isStreaming){
                 return streamNextToken();
             }
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
        void matchFromSTDIN(bool debug){
            jmatch.text = &myString[0];



            RegexToDFABuilder::match(myString, verbose | debug,false, this);

        }

    };




}

// std::move(stateList)
static JLexx::TheRealJLexx jl;


void compress(bool compress ,bool debug = false){
    if(compress){
        jl.minimizeStates(debug);
    }
}
void setVerbose(bool x){
    jl.verbose = x;
}
void matchSTDIN(bool stream = false,bool debug = false){

   // if(!stream){
        jl.matchSTDIN(debug);
        jl.isStreaming = false;

   // }else {
   //     jl.isStreaming = true;
   // }
}
bool endOfInput(){
    return jl.endOfInput();
}

std::pair<JVAL,TokenType>  jlnext(){
    return jl.jlnext();

}
#endif //JLEXX_THEREALJLEXX_H
