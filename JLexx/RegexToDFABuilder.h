//
// Created by joshua on 8/18/16.
//

#ifndef JLEXX_REGEXTODFABUILDER_H
#define JLEXX_REGEXTODFABUILDER_H

#include <iosfwd>
#include <vector>
#include "../RegularExpression/NFAGenerator/NFAGenerator.h"
#include "../RegularExpression/NFAGenerator/EZGraph.h"
#include "DFABuilderHelper.h"
#include "../RegularExpression/Lexer/RegularLexer.h"
#include "../RegularExpression/Parser/RegularParser.h"


class RegexToDFABuilder{

public:
    typedef std::pair<JRegex::Vertex *, JRegex::Vertex *> VPair;
    RegexToDFABuilder(){hasBeenCreated = false;}
    RegexToDFABuilder & add(const std::string & str){
        regexList.push_back(str);
        return *this;
    }



    ~virtual RegexToDFABuilder(){
        if(start.first) {
            start.first->removeAll();
        }

    }
    RegexToDFABuilder & create(bool verbose = true){
        if(hasBeenCreated){

            throw std::invalid_argument("This Converter has already been created");
        }else{
            hasBeenCreated = true;
            createStates(true);
        }

        return *this;
    }
    void addToList(const std::string & str){
        regexList.push_back(str);

    }
    virtual bool match(string s,bool verbose = true,bool shortestMatch = false){
        if(shortestMatch){throw std::invalid_argument("I need to implement that feat");}
        unsigned int curr = 0;
        unsigned int start = 0;
        unsigned int lastState =0;
        JRegex::State & last = stateList[0];
        bool toReturn = false;
        s.insert(s.begin(),'\n');
        s.push_back('\n');
        while(start  < s.length()){
            while((stateList[lastState][s[curr]]) != JRegex::STATE_SENTINEL){
                lastState = stateList[lastState][s[curr++]];
            }
            if(stateList[lastState].isAccepting()){
                if(verbose) {
                    printf("\n@BEGIN MATCH\n");
                    printf("\tMatched     : %s \n", s.substr(start, curr - start).c_str());
                    printf("\tWith Regex  : %u \n", stateList[lastState].acceptingState);
                    printf("@END   MATCH\n");
                }

                start+=(curr-start) - 1;
            }

            last = stateList[0];
            lastState = 0;
            start++;
            curr = start;
        }

        return toReturn;
    }

private:
    void createStates(bool verbose){
        using namespace JRegex;
        maxState = 0;
        if(!regexList.size()){throw std::invalid_argument("When creating states you need at least one regex");}
         start = {nullptr,nullptr};
        for( int i = regexList.size()-1; i >=0; i--) {

            RegularParser p(new RegularLexer(regexList[i]));
            p.parse();
            RegularGraph graph(p.deque.regularDeque.front(),p.deque.lAnchor,p.deque.rAnchor);
            graph.dontDeallocate();
            VPair curr;
            if(start.first == nullptr) {
                start = graph.getGraph();
                start.second->isAccepting = true;
                acceptList.emplace(Priority(start.second,i));
            }else{
                // or them together
                curr = graph.getGraph();
                curr.second->isAccepting = true;
                acceptList.insert(Priority(curr.second,i));
                start = orExpression(curr,start);
            }

        }

        DFAHelper h(start, acceptList);
        stateList.reserve(1024);// roughly 1 mb

        vector<DFAStateHelper*> list =h.list;

        for(const DFAStateHelper * h : list){
            State state(h,maxState++);
            unsigned int acceptingState = getMaxStateFrom(h->fromState);
            state.acceptingState = acceptingState;
            stateList.push_back(state);
            if(verbose) {
                h->printSelf();
            }
            delete h;
        }

    }
    // one will always be the new VPair, two will always have the end vertex
    VPair orExpression(const VPair & one, const VPair &two){
        using namespace JRegex;
        Vertex *start = Vertex::createEpsilon();
        one.second->edge->v2IsEpsilon = true;
        two.second->edge->v2IsEpsilon = true;
        start->edge->v1 = one.first;
        start->edge->v2 = two.first;
        one.second->edge->v2 = two.second;

        return VPair(start, two.second);

    }

    unsigned int getMaxStateFrom(const std::set<JRegex::Vertex *> & s){
        unsigned int toReturn = JRegex::STATE_SENTINEL;
        for(JRegex::Vertex * v : s){
            if(acceptList.find(v) != acceptList.end()) {
                if (acceptList[v] < toReturn) {
                    toReturn = acceptList[v];
                }
            }

        }
        return toReturn;
    }

    // the way you add to this list will determine
    // the precedence of the regex
    // index 0 == highest precedence

    bool hasBeenCreated;
    unsigned int maxState;
    std::vector<std::string> regexList;
    vector<JRegex::State >  stateList;
    VPair start;
    std::unordered_map<JRegex::Vertex *,unsigned int> acceptList;
    typedef std::pair<JRegex::Vertex *, unsigned int> Priority;


};
#endif //JLEXX_REGEXTODFABUILDER_H


