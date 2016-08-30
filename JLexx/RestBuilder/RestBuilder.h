//
// Created by joshua on 8/24/16.
//

#ifndef JLEXX_RESTBUILDER_H
#define JLEXX_RESTBUILDER_H
#include "../RegexToDFABuilder.h"
#include "RestGenerator.h"
#include "RestToken.h"
#include "../FileLoader.h"
namespace Rest{
    class RestBuilder : public RegexToDFABuilder{
    public:
        RestGenerator & rg;

        std::vector<std::string> lexRegexList;
        RestBuilder() = delete;
        RestBuilder(RestGenerator & rgg,std::string fileName): RegexToDFABuilder(),rg(rgg){

            lexRegexList = {
                    "[gG][eE][tT]",
                    "[pP][uU][tT]",
                    "[pP][oO][sS][tT]",
                    "[dD][eE][lL][eE][tT][eE]",
                    "POST_PARAMS",
                    "file",
                    "[a-zA-Z][a-zA-Z0-9]*[.][a-zA-Z]+",
                    "string",
                    "array",
                    "return",
                    "\\\"[sS]uccess\\\"", // 10
                    "\\\"[eE]rror\\\"",
                    "dictionary",
                    ":",
                    "\\|",
                    "\\-\\>",
                    ";",
                    "^http://[^/]*/",
                    "[\t\n ]+",
                    "[a-zA-Z][a-zA-Z0-9]*",
                    "[^\t\n ]"


            };

            for(auto t : lexRegexList){
                this->add(t);
            }
            this->create(true);
            std::string str = FileLoader::fileToString(fileName,false);
            this->match(str,true,false,nullptr);
        }

        virtual bool match(string  &s, bool verbose, bool shortestMatch,Lexx::DataHandler * handler) override{

            if(shortestMatch){throw std::invalid_argument("I need to implement that feat");}
            unsigned int curr = 0;
            unsigned int start = 0;
            unsigned int lastState =0;
            JRegex::State & last = stateList[0];
            bool toReturn = false;
            s.insert(s.begin(),'\n');
            s.push_back('\n');
            // start of match end of match regex number
            //Lexx::Data d = {0,0,0};

            while(start  < s.length()){
                while((stateList[lastState][s[curr]]) != JRegex::STATE_SENTINEL){
                    lastState = stateList[lastState][s[curr++]];
                }
                if(stateList[lastState].isAccepting()){
                    if(verbose) {
                        printf("\n@BEGIN MATCH\n");
                        printf("\tMatched     : %s \n", s.substr(start, curr - start).c_str());
                        printf("\tWith Regex  : %u and token string %s\n", stateList[lastState].acceptingState,lexRegexList[stateList[lastState].acceptingState].c_str());
                        printf("@END   MATCH\n");
                    }

                    RestToken t = {stateList[lastState].acceptingState,s.substr(start, curr - start)};
                    rg.handleToken(std::move(t));
                   //d.startOfMatch = start;
                   //d.endOfMatch = curr;
                   //d.regexNumber =stateList[lastState].acceptingState;
                    //generator.handleData(d);

                    start+=(curr-start) - 1;
                }

                last = stateList[0];
                lastState = 0;
                start++;
                curr = start;
            }

            return toReturn;
        }



    };



};
#endif //JLEXX_RESTBUILDER_H
