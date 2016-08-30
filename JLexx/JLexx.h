//
// Created by joshua on 8/18/16.
//

#ifndef JLEXX_JLEXX_H
#define JLEXX_JLEXX_H

#include "RegexToDFABuilder.h"

namespace Lexx {
    class JLexx : public RegexToDFABuilder , public DataHandler{

    private:
        std::string myString;
    protected:
    public:

        // DEFINE TOKEN TYPES HERE
        enum TokenType : unsigned int{
            INCLUDE=               0,
            STRUCT                  ,
            BEGINTOK                ,
            ENDTOK                  ,
            BEGINDEF                ,
            ENDDEF                  ,
            TOKEN_OR_REGEX          ,
            REGEX_STMT              ,
            WHITESPACE              ,
            OTHER                   ,
            AZ                   ,



            COUNT
        };

        typedef TokenType TT;
        // [\t][a-zA-Z]+  is not working but [a-zA-Z] is wokring :(wo
        // #include [^\n]+[\n]"  is, this could be just

        std::map<TT,std::string> tokenToRegex ={
                {INCLUDE            ,"#include [^\n]+[\n]"},
                {STRUCT             ,"struct[ ]*{[^}]*}"},
                {BEGINTOK           ,"BEGIN TOKENS"},
                {ENDTOK             ,"END TOKENS"},
                {BEGINDEF           ,"BEGIN DEF"},
                {ENDDEF             ,"END DEF"}, //5
                {TOKEN_OR_REGEX     ,"[^\n]+"}, // just have a function that removes all trailing whitespace
                {REGEX_STMT         ,"{[^}]+}"},
                {WHITESPACE         ,"[\t\n ]+"}, //
                {OTHER              ,"."},
        };


        struct Token{
            TT tokenType;

            union {
                int  value;
                char * str;
            };
        };


        JLexx() : RegexToDFABuilder() {
            for(auto pair : tokenToRegex){RegexToDFABuilder::addToList(pair.second);}
            RegexToDFABuilder::create(false);
        }

        virtual void handleData(Data &d) override {
            printf("Matched %s with Regex %s\n",myString.substr(d.startOfMatch,d.endOfMatch-d.startOfMatch).c_str(),tokenToRegex[(TT)d.regexNumber].c_str());
        }


        bool match(string s){
            this->myString = std::move(s);
            return RegexToDFABuilder::match(myString, false, false, this);

        }

        virtual ~JLexx() {

            int x = 3;
        }

    };



}
#endif //JLEXX_JLEXX_H



