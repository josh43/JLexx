//
// Created by joshua on 8/17/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_EPSILONNFAMATCHER_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_EPSILONNFAMATCHER_H

#include "EZGraph.h"
#include "../Utility.h"
#include <string>
#include <stdexcept>
#include <limits>
namespace JRegex {
    using namespace std;
class EpsilonNFAMatcher {
    public:
        vector<string> wordMatches;
        Vertex *start;
        Vertex *end;

        EpsilonNFAMatcher(Vertex *s, Vertex *e) : start(s), end(e) {

        }
        EpsilonNFAMatcher(std::pair<Vertex *,Vertex *> e) : start(e.first), end(e.second) {

        }
        ~EpsilonNFAMatcher(){

        }
        bool match( std::string & str, bool verbose = false,bool matchLongest = true) {
            vector<pair<string, int> > matches;

            int currentIndex = 0;
            int longestMatch = -1;
            int indexToPush = -1;
            int lastIndex = 0;
            bool shouldReset = false;
            set<Vertex *>  lastClosure;
            lastClosure.insert(start);
            int startIndex = 0;
            str.insert(str.begin(),'\n');
            str.push_back('\n');
            while ((uint)currentIndex < str.length()) {
                set<Vertex *> closure;
                if (str[currentIndex] < 0) {
                    str[currentIndex] = std::numeric_limits<unsigned char>::min() + (-str[currentIndex]);
                }
                if (canTransitionOn(str[currentIndex++], lastClosure, closure)) {
                    //transition(closure,str[currentIndex]);
                    if (foundEnd(closure, end)) {
                        if(!matchLongest){
                            shouldReset = true;
                        }else {
                            if (longestMatch < currentIndex - startIndex) {
                                longestMatch = currentIndex - startIndex;
                                indexToPush = lastIndex++;
                            }
                        }
                        matches.push_back({str.substr(startIndex, currentIndex - startIndex), startIndex});
                    }
                    lastClosure = closure;
                }else{

                    shouldReset = true;
                }
                if(shouldReset) {
                    // reset because not in a valid state
                    if (indexToPush >= 0 && longestMatch != -1) {
                        longestMatch = -1;
                        wordMatches.push_back(matches[indexToPush].first + "\n");

                    }
                    indexToPush = -1;

                    startIndex = currentIndex;
                    lastClosure.clear();
                    lastClosure.insert(start);
                    shouldReset = false;
                }

                //currentIndex+=
            }
            if (verbose) {

                for (pair<string, int> match : matches) {
                    printf(ANSI_COLOR_BLUE "  Match Found : %s @ %i" ANSI_COLOR_RESET " \n ", match.first.c_str(),
                           match.second);
                }
            }

            return matches.size();
        }





        static bool canTransitionOn(unsigned char letter, const set<Vertex *> &lastClosure, set<Vertex *> &newClosure) {
            set < Vertex * > temp = lastClosure;
            for (Vertex *v : lastClosure) {
                set < Vertex * > returned = epsilonClosure(v);
                temp.insert(returned.begin(), returned.end());
            }
            for (Vertex *v : temp) {
                v->visited = false;
            }
            bool canTransition = false;

            for (Vertex *v : temp) {
                if (v->edge->canTransitionOn(letter)) {
                    if (v->edge->v1) {
                        newClosure.insert(v->edge->v1);
                    } else {
                        // it must have been the end
                        newClosure.insert(v->edge->v2);
                    }
                    canTransition = true;
                }
            }
            temp.clear();
            for (Vertex *v : newClosure) { // out of the ones you can transition to
                // you must compute the closure for every vertex
                set < Vertex * > returned = epsilonClosure(v);
                temp.insert(returned.begin(), returned.end());
            }

            newClosure = std::move(temp);
            return canTransition;
        }

        static set<Vertex *> epsilonClosure(Vertex *theVertex) {
            set < Vertex * > verts;
            closureHelper(theVertex, verts);
            for (auto v : verts) {
                v->visited = false;
            }
            return verts;
        }

        static void closureHelper(Vertex *v, set<Vertex *> &vertList) {
            if (v == nullptr) { return; }
            if (v->visited) { return; }
            v->visited = true;
            vertList.insert(v);
            if (v->edge->v1IsEpsilon || v->edge->val == Edge::EPSILON) {
                closureHelper(v->edge->v1, vertList);
            }
            if (v->edge->v2IsEpsilon || v->edge->val == Edge::EPSILON) {
                closureHelper(v->edge->v2, vertList);
            }


        }

        bool foundEnd(set<Vertex *> &closure, Vertex *end) {
            for (auto x : closure) {
                if (x == end) {
                    return true;
                }
            }
            return false;
        }

    };


};
#endif //LANGUAGEIMPLEMENTATIONPATTERNS_EPSILONNFAMATCHER_H
