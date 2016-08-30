//
// Created by joshua on 8/23/16.
//

#ifndef JLEXX_PARSER_H
#define JLEXX_PARSER_H

#include "../JLexx.h"
#include "Generator.h"

namespace LexLoader{
  class Parser: public RegexToDFABuilder{
  public:
      Generator & generator;
      std::vector<std::pair<std::string,unsigned int > > tokenList;
      Parser(Generator & g):RegexToDFABuilder(),generator(g){
          tokenList = {
                       {"[\\t\\n ]+",   0},
                       {"^struct",       1},
                       {"{",            2},
                       {"}",            3},
                       {"^int",          4},
                       {"^char *",       5},
                       {"BEGIN TOKENS", 6},
                       {"END TOKENS",   7},
                       {"[a-zA-Z]+",    8},
                       {".",           9},
          };

          //         accept 0
          this->add("SENTINEL");
          //            1           2       3         4          5
          this->add("^struct").add("{").add("}").add("^int").add("^char *");
          //            6                   7               8               9
          this->add("^BEGIN TOKENS").add("^[a-zA-Z]+").add("^END TOKENS").add(".");
          this->create(true);
      }
/*
      virtual bool match(string s, bool verbose, bool shortestMatch) override{

          if(shortestMatch){throw std::invalid_argument("I need to implement that feat");}
        unsigned int curr = 0;
        unsigned int start = 0;
        unsigned int lastState =0;
        JRegex::State & last = stateList[0];
        bool toReturn = false;
        s.insert(s.begin(),'\n');
        s.push_back('\n');
          // start of match end of match regex number
          Lexx::Data d = {0,0,0};

        while(start  < s.length()){
            while((stateList[lastState][s[curr]]) != JRegex::STATE_SENTINEL){
                lastState = stateList[lastState][s[curr++]];
            }
            if(stateList[lastState].isAccepting()){
                if(verbose) {
                    printf("\n@BEGIN MATCH\n");
                    printf("\tMatched     : %s \n", s.substr(start, curr - start).c_str());
                    printf("\tWith Regex  : %u and token string %s\n", stateList[lastState].acceptingState,tokenList[stateList[lastState].acceptingState].first.c_str());
                    printf("@END   MATCH\n");
                }
                d.startOfMatch = start;
                d.endOfMatch = curr;
                d.regexNumber =stateList[lastState].acceptingState;
                generator.handleData(d);

                start+=(curr-start) - 1;
            }

            last = stateList[0];
            lastState = 0;
            start++;
            curr = start;
        }

        return toReturn;
    }
*/


  };



};
#endif //JLEXX_PARSER_H
