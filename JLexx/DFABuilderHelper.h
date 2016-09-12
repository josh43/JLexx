//
// Created by joshua on 8/18/16.
//

#ifndef JLEXX_DFABUILDERHELPER_H
#define JLEXX_DFABUILDERHELPER_H
//
// Created by joshua on 8/16/16.
//



#include <map>
#include <unordered_map>
#include "../RegularExpression/NFAGenerator/EZGraph.h"
#include "../RegularExpression/NFAGenerator/NFAGenerator.h"
#include "../RegularExpression/Utility.h"
#include "../RegularExpression/NFAGenerator/EpsilonNFAMatcher.h"

namespace JRegex {
    typedef unsigned char  INFO;
    const static unsigned int STATE_SENTINEL = std::numeric_limits<unsigned int>::max();
    //const static unsigned int DEAD = std::numeric_limits<unsigned int>::max()-1;
    const static unsigned char    STATE_REGULAR=(1<<8) - 1;
    const static unsigned char    STATE_START = 2;
    const static unsigned char    STATE_ACCEPTING=4;

    struct DFAStateHelper{
        set<Vertex *> fromState;
        INFO stateType;
        static const unsigned char HELPER_EPSILON = (1<<8) -1;
        set<Vertex *> * transitions[256];
        set<unsigned char> transitionsUsed;
        unsigned int stateNumber;
        vector<pair<unsigned char,unsigned int> > stateTransitions;


        ~DFAStateHelper(){
            for(int i = 0 ; i < 256;i++){
                if(transitions[i]) {
                    delete transitions[i];

                }
            }
        }
        DFAStateHelper(){
            //toState = nullptr;
            stateType = STATE_REGULAR;
            for(int i = 0 ; i < 256 ; i ++){transitions[i] = nullptr;}
        }

        void checkAcceptingAgainst (const std::unordered_map<Vertex *, unsigned int> & aList){
            for(Vertex *v : fromState){
                if(aList.find(v) != aList.end()){
                    this->stateType |= STATE_ACCEPTING;
                }
            }
        }
        static set<Vertex *> move(DFAStateHelper & helper, unsigned char c){
            bool needToRecalculateClosure = false;
            if(!helper.transitions[c]){
                helper.transitions[c] = new set<Vertex *>();
                needToRecalculateClosure = true;
                for(Vertex * v : helper.fromState){
                    if(v->edge->canTransitionOn(c)){
                        if(v->edge->v1 != nullptr) {
                            helper.transitions[c]->insert(v->edge->v1);
                        }

                    }
                }
            }

                union JVal{
                    int x;
                    float y;
                    char * text;
                }JT;
           //if(helper.transitions[c]==nullptr){
           //    return set<Vertex *>();
           //}
            if(needToRecalculateClosure) {
                JVal v1;

                set<Vertex *> final;
                for (Vertex *v : *helper.transitions[c]) {
                    set<Vertex *> res = EpsilonNFAMatcher::epsilonClosure(v);
                    final.insert(res.begin(), res.end());
                }
                helper.transitions[c]->insert(final.begin(), final.end());
                helper.transitionsUsed.insert(c);
            }
            return *helper.transitions[c];
        }

        void printSelf() const{
            printf("@stateType : %c\n",this->stateType);
            printf("@Printing Transitions\n");
            for(unsigned char c : transitionsUsed) {
                printf(ANSI_COLOR_BLUE "\t@transition[%c]\n" ANSI_COLOR_RESET, c);
            }
            if(stateTransitions.size()){
                printf("@START StateTransitions\n\n");
                for(int i = 0; i < stateTransitions.size(); i ++){
                    if(i +10 % 10 == 0){
                        printf("\n");
                    }
                    printf("T['%c']=%-10d ",stateTransitions[i].first,stateTransitions[i].second);
                }
                printf("@END   StateTransitions\n\n");
            }
            printf("@End \n");
        }

    };
    struct State{
        unsigned char info;//    4 bytes used for padding
        unsigned int acceptingState;
        // but first byte will be for INFO next 3 for action
        unsigned int stateNumber; // 4 bytes
        unsigned int jump[256];  // 1024 bytes


        bool isAccepting(){
            return acceptingState != STATE_SENTINEL;
        }

        unsigned int operator[](unsigned char index){return jump[index];}
        State(const DFAStateHelper * stateHelper,unsigned int stateNo){
            this->stateNumber = stateHelper->stateNumber;
            acceptingState = STATE_SENTINEL;
            info = stateHelper->stateType;

            for(int i = 0; i < 256; i ++){
                jump[i] = STATE_SENTINEL;
            }
            for(auto pair : stateHelper->stateTransitions){
                jump[pair.first] = pair.second;
            }
        }
        void printState(bool printTransitionsOnly = true){
            printf("Printing State %d\n\n",this->stateNumber);
            printf("Accepting State %d\n",this->acceptingState);
            unsigned int numUsed = 0;
            for(unsigned int c = 0; c < 256; c ++){
                if(((numUsed + 11) % 10 )== 0){
                    printf("\n");
                }

                if(jump[c] == STATE_SENTINEL){
                    if(!printTransitionsOnly) {
                        printf("%c = \"\" , ", c, jump[c]);
                        numUsed++;
                    }

                }else {
                    if(c == '\n'){
                        printf("\\n = %u , ", jump[c]);
                    }else if(c == '\t'){
                        printf("\\t = %u , ", jump[c]);
                    }else if(c == ' '){

                        printf("SPACE = %u, ", jump[c]);

                    }else {
                        printf("%c = %u , ", c, jump[c]);
                    }
                    numUsed++;
                }
            }
            printf("End Printing State %d\n\n",this->stateNumber);
        }
        State(unsigned int stateNo){
            acceptingState = STATE_SENTINEL;
            info = STATE_REGULAR;
            stateNumber = stateNo;
            for(int i = 0; i < 256; i ++){jump[i] = STATE_SENTINEL;}
        }

        State():State(0){ }
        // want to go into something divisible by 8
        void set(unsigned  char c, const unsigned int i1){
            jump[c] = i1;
        }
    };
    class DFAHelper {









        vector<string> actionList;
        pair<Vertex *, Vertex *> initialNfa;
        const std::unordered_map<Vertex *, unsigned int> & acceptList;

    public:
        vector<DFAStateHelper*> list;

        ~DFAHelper(){
        }
        // i own it
        DFAHelper(pair<Vertex *, Vertex *> nfaState, const std::unordered_map<Vertex *, unsigned int> &aList):acceptList(aList){
            initialNfa = nfaState;


            list = createDFA();
            setTransitions(list);




        }

    private:
        void setTransitions(const vector<DFAStateHelper *> & list,bool safeChecker = true){
            typedef set<Vertex *>  SVP;
            map<set<Vertex *>,unsigned int > stateMap;
            if(!list.size()){throw std::invalid_argument("Illegal DFA needs to be greater than size 0!!");}
            unsigned int index = 0;
            if(safeChecker) {
                for (unsigned int i = 0; i < list.size(); i++) {
                    for(unsigned int j = i +1; j < list.size(); j++){
                        if(list[i] == list[j]){
                            throw std::invalid_argument("Cannot have multiple of the same items on the list!! most likely an error on move(...)\n");
                        }
                    }
                }
            }
            for(DFAStateHelper * h : list){
                h->stateNumber = index;
                stateMap.insert(pair<SVP,unsigned int>(h->fromState, index++));
            }
            for(DFAStateHelper * h : list) {
                for(unsigned char trans: h->transitionsUsed){
                    unsigned int final = stateMap.find(*h->transitions[trans])->second;
                    h->stateTransitions.push_back(pair<unsigned char,  unsigned int>(trans,final));
                }
            }
        }
        bool isNewTransition(const vector<DFAStateHelper *> & list,const set<Vertex *> & verts){
            for(const DFAStateHelper * helper : list){
                /*
                 * Checks if the contents of lhs and rhs are equal, that is, whether lhs.size() == rhs.size()
                 * and each element in lhs has equivalent element in rhs at the same position.
                 */
                if(helper->fromState == verts){
                    return false;
                }
            }
            return true;
        }
        vector<DFAStateHelper *> createDFA(){
            vector<DFAStateHelper *> list;
            DFAStateHelper * start = new DFAStateHelper(); start->stateType = STATE_START;
            start->fromState = EpsilonNFAMatcher::epsilonClosure(this->initialNfa.first);
            start->checkAcceptingAgainst(acceptList);

            list.push_back(start);
            stack<DFAStateHelper * > statesLeft;
            statesLeft.push(start);
            while(!statesLeft.empty()){
                DFAStateHelper * curr = statesLeft.top();
                statesLeft.pop();
                for(Vertex * v : curr->fromState){
                    set<Vertex *> verts;
                    if(dynamic_cast<BracketEdge *>(v->edge)){
                        BracketEdge * b = dynamic_cast<BracketEdge *>(v->edge);
                        //set<Vertex *> res = EpsilonNFAMatcher::epsilonClosure(v->edge->v1);
                        //set<Vertex *> resTwo = EpsilonNFAMatcher::epsilonClosure(v->edge->v2);

                        for(unsigned int i = 0; i < 256; i ++) {
                            if(b->canTransitionOn(i)) {
                                verts = DFAStateHelper::move(*curr, i);
                                if(!verts.size()){continue;}
                                if(isNewTransition(list,verts)){
                                    DFAStateHelper * toPush = new DFAStateHelper(); toPush->fromState = verts;
                                    toPush->checkAcceptingAgainst(acceptList);
                                    list.push_back(toPush);
                                    statesLeft.push(toPush);
                                }else{
                                  //  curr->transitions[v->edge->val]->insert(res.begin(),res.end());
                                  //  curr->transitions[v->edge->val]->insert(resTwo.begin(),resTwo.end());
                                }
                            }
                        }
                    } else if(v->edge->val < Edge::UNINIT ){
                        verts = DFAStateHelper::move(*curr,v->edge->val);
                        if(!verts.size()){continue;}
                        if(isNewTransition(list,verts)){
                            DFAStateHelper * toPush = new DFAStateHelper(); toPush->fromState = verts;
                            toPush->checkAcceptingAgainst(acceptList);
                            list.push_back(toPush);
                            statesLeft.push(toPush);
                        }else{
                           // set<Vertex *> res = EpsilonNFAMatcher::epsilonClosure(v);
                           // curr->transitions[v->edge->val]->insert(res.begin(),res.end());
                        }

                    }// else do nada
                }

            }

            return list;
        }


    };


};

#endif //JLEXX_DFABUILDERHELPER_H
