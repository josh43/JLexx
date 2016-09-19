//
// Created by josh on 9/9/16.
//

#ifndef JLEXX_STATELEXER_H
#define JLEXX_STATELEXER_H

#include <string>
#include "../RegexToDFABuilder.h"

namespace JLexx {
    class LexGenerator :  public DataHandler{

    private:
        // the first string will be the re, the second will be the function pointer text
        std::vector<std::pair<std::string,std::string > > reNameList;

        //std::vector<std::pair<unsigned int,RegexFunction> > reFuncList; this will be used once its done!
        std::vector<RegexToDFABuilder *> stateLexer;
        std::string myString;
        bool shouldSwitchLexers;
        uint offsetForNextLexer;
        uint stateLexerCurrent;
        FILE * outFile;
        FILE * bridgeFile;
        bool foundOBracket;
        bool readingRegex;
        bool hasPrintedEnumStart;
        bool hasPrintedEnumEnd;
        bool hasPrintedRegexList;
        bool isDebugging;
        std::string currentRegex;
        std::string currentReturnStatement;
        uint foundOCurly; // there could potentially be a couple
    protected:
    public:

        // DEFINE TOKEN TYPES HERE
        enum TokenType : unsigned int{

            BEGINTOK             = 0,
            BEGINMAIN               ,
            BEGINDEFINITIONS        ,
            BIGCOMMENT              ,
            COMMENT                 ,
            L_BRACKET               ,
            OR_EXPRESSION           ,
            R_BRACKET               ,
            WHITESPACE              ,
            TOKEN                   ,
            NL                   ,
            OTHER                   ,
            UNION               ,
            JVAL               ,
            AZ                      ,



            COUNT
        };
        typedef uint (*RegexFunction);
        typedef TokenType TT;
        typedef std::pair<TT,std::string> REPair;
        typedef std::vector<REPair> REList;
        //std::vector<std::string  > ={
        REList stateZero ={
                {BEGINTOK           ,"BEGIN TOKENS"},
                {UNION              ,"union"},
                {JVAL               ,"jval;"},
                {NL                 ,"[\n]"},
                {OTHER              ,"[^\n]"}
        };

        REList stateOne ={
                {BEGINDEFINITIONS   ,"BEGIN DEFINITIONS"},
                {TOKEN              ,"[a-zA-Z]+"},
                {COMMENT             ,"//[^\n]*[\n]"},
                {BIGCOMMENT          ,"/[*]([^*/]|[*]+)*[*]/"},
                {NL                 ,"[\n]"},
                {OTHER              ,"[^\n]"}
        };

        // use \\ to escape any character
        REList stateTwo ={

                {BEGINMAIN          ,"BEGIN MAIN"},
                {COMMENT            ,"//[^\n]*[\n]"}, //
                {BIGCOMMENT          ,"/[*]([^*/]|[*]+)*[*]/"},
                {OR_EXPRESSION       ,"[\n\t ]*[|][\n\t ]*"},
                {WHITESPACE         ,"[\t\n ]+"}, //
                {OTHER              ,"[^\n]"}
        };



        struct Token{
            TT tokenType;

            union {
                int  value;
                char * str;
            };
        };


        virtual bool shouldStop() override {
            return shouldSwitchLexers;
        }

        virtual void streamToken(const std::string &s, uint regexMatch) override {
            // dont do anything
        }

        LexGenerator(bool debugging = false,bool ideInput = false) {



            isDebugging = debugging;
            currentRegexNumber =  nextOrLocation= 0 ;
            currentRegex = "";
            foundOBracket =
            readingRegex  =
            hasPrintedEnumStart= readingOrExpression = hasPrintedRegexList =
            hasPrintedEnumEnd = false;
            foundOCurly = 0;

            std::string base = "";
            if(ideInput){
                base = "/home/josh/ClionProjects/JLexx/JLexx/LexLoader/";
            }else{

                base = std::string("/usr/share/JLexx/JLexx/LexLoader/");
            }
            std::string lexMain = base + "GenFiles/LexMain.cpp";
            std::string bridge = base + "GenFiles/BridgeHeader.h";
            if(debugging) {
                printf("Final LexMain path is %s \n",lexMain.c_str());
                printf("Final bridge  path is %s \n",bridge.c_str());
            }
            outFile = fopen(lexMain.c_str(),"w+");
            if(outFile == NULL){
                printf("Couldnt open %s\n",lexMain.c_str());
                throw std::invalid_argument("Couldn't open file") ;
            }

            bridgeFile = fopen(bridge.c_str(),"w+");
            if(bridgeFile == NULL){
                printf("Couldnt open %s\n",bridge.c_str());
                throw std::invalid_argument("Couldn't open file") ;
            }
            fprintf(bridgeFile,
                    "#include <iostream>\n"
                            "#include <vector>\n");
            fprintf(bridgeFile,
                    "struct{\n"
                    "    char * text;\n"
                    "    unsigned int start;\n"
                    "    unsigned int end;\n"
                    "}jmatch;\n");
            fprintf(outFile,"#include \"../TheRealJLexx.h\"\n");
            shouldSwitchLexers = false;
            stateLexerCurrent = 0;
            RegexToDFABuilder * rdfaOne, * rdfaTwo,* rdfaThree;
            rdfaOne =   new RegexToDFABuilder();
            rdfaTwo =   new RegexToDFABuilder();
            rdfaThree = new RegexToDFABuilder();
            stateLexer.push_back(rdfaOne);
            stateLexer.push_back(rdfaTwo);
            stateLexer.push_back(rdfaThree);
            int currState = 0;
            offsetForNextLexer = 0;
            for(auto pair : stateZero){
                stateLexer[currState]->addToList(pair.second);
            }
            currState++;

            for(auto pair : stateOne){
                stateLexer[currState]->addToList(pair.second);
            }
            currState++;
            for(auto pair : stateTwo){
                stateLexer[currState]->addToList(pair.second);
            }
            // state four is just output everything

            for(currState; currState >=0; currState--){
                stateLexer[currState]->create(true);
            }

        }

        void printEnumEnd() {
            fprintf(bridgeFile,"\t COUNT\n\n };//End TokenType def;");
        }
        void printEnumStart(const std::string & s) {
            fprintf(bridgeFile,"\nenum TokenType : unsigned int {\n\n \t%-12s = 256,\n",s.c_str());
        }
        void printToken(const std::string  & s){
            fprintf(bridgeFile,"\t %-15s,\n",s.c_str());

        }


        void printRegexList() {
            std::string prefixName = "jre";
            unsigned int curr = 0;
            for(auto x : this->reNameList){
                std::string finalFuncName = prefixName + to_string(curr);
                fprintf(bridgeFile,"\nuint %s()%s\n",finalFuncName.c_str(),x.second.c_str());
                curr++;
            }
            curr = 0;
            fprintf(bridgeFile,"std::vector<std::pair<std::string,uint (*)()> > reFuncList =\n{\n");
            for(;curr < reNameList.size(); curr++){
                auto x = reNameList[curr];
                std::string finalFuncName = prefixName + to_string(curr);
                if(curr == reNameList.size() -1){
                    fprintf(bridgeFile, "\t{\"%s\",%10s}\n};", x.first.c_str(), finalFuncName.c_str());
                }else {
                    fprintf(bridgeFile, "\t{\"%s\",%10s},\n", x.first.c_str(), finalFuncName.c_str());
                }
            }


        }

        virtual void handleData(Data &d) override {
            printf("Matched %s with Regex\n",
                   myString.substr(d.startOfMatch, d.endOfMatch - d.startOfMatch).c_str());
            offsetForNextLexer += (d.endOfMatch - d.startOfMatch);
            if (d.regexNumber == 0) {
                if(stateLexerCurrent != 2) { // this is kind of a hack, I use stateLexer 2 for outputing the remainder of the file :|
                    // print main details
                    shouldSwitchLexers = true;
                }

                stateLexerCurrent++;

                return;
            }


            if(stateLexerCurrent == 1){

                if(d.regexNumber == 1){// this is the word match
                    if(!hasPrintedEnumStart){
                        printEnumStart(myString.substr(d.startOfMatch, d.endOfMatch - d.startOfMatch));
                        hasPrintedEnumStart = true;
                    }else{
                        printToken(myString.substr(d.startOfMatch, d.endOfMatch - d.startOfMatch));
                    }
                }

            }else if(stateLexerCurrent == 2){

                if(d.regexNumber == 0){
                    printRegexList();
                    hasPrintedRegexList = true;
                }
                if(!hasPrintedEnumEnd){
                    hasPrintedEnumEnd = true;
                    printEnumEnd();
                }
                    /*
                     *
                {BEGINMAIN          ,"BEGIN MAIN"},
                {COMMENT            ,"//[^\n]*[\n]"}, //
                {BIGCOMMENT          ,"/[*]([^*]|[*]+[^/]*)*[*]/"},
                {WHITESPACE         ,"[\t\n ]+"}, //
                {OTHER              ,"[^\n]"}
                     */
                if(myString[d.startOfMatch] == '{' &&  !readingRegex){
                    this->foundOCurly++;
                }
                if(myString[d.startOfMatch] == '}' &&  !readingRegex){
                    this->foundOCurly--;
                    if(this->foundOCurly == 0){
                        currentReturnStatement+=myString.substr(d.startOfMatch, d.endOfMatch - d.startOfMatch);
                        //fprintf(outFile,"RETURN STATEMENT -> %s \n",currentReturnStatement.c_str());
                        reNameList[currentRegexNumber++].second = currentReturnStatement;
                        currentReturnStatement = "";
                        return;
                    }
                }
                if(foundOCurly){ // just print it all out for now :)
                    currentReturnStatement+=myString.substr(d.startOfMatch, d.endOfMatch - d.startOfMatch);
                    fprintf(stdout, "%s", myString.substr(d.startOfMatch, d.endOfMatch - d.startOfMatch).c_str());

                }else {
                    bool isRegexCharacter = (d.regexNumber == 5) || (this->foundOBracket && d.regexNumber == 4);
                    if (isRegexCharacter) { // its a regex

                        switch (myString[d.startOfMatch]) {
                            case ']': {
                                this->foundOBracket = false;
                                currentRegex.push_back(']');
                                break;
                            }
                            case '[': {
                                this->foundOBracket = true;
                                currentRegex.push_back('[');
                                break;
                            }
                            default:
                                    while (d.startOfMatch < d.endOfMatch) {
                                        currentRegex.push_back(myString[d.startOfMatch++]);
                                    }
                                break;
                        }




                        if (!readingRegex) {
                            // start the regex
                            readingRegex = true;
                        }
                    } else if(d.regexNumber == 3 && readingRegex){
                        // or expression
                        // remember in order to read an or expression I need to put () around the expression lol
                            if(!readingOrExpression) {
                                readingOrExpression = true;
                                currentRegex.insert(currentRegex.begin(),'(');

                            }
                                currentRegex.push_back('|');


                    }else if (readingRegex) {
                        //printRegex(currentRegex);
                        if(readingOrExpression){
                            currentRegex.push_back(')');

                            nextOrLocation = 0;
                            readingOrExpression = false;
                        }
                        reNameList.push_back({currentRegex,""});
                        currentRegex = "";
                        readingRegex = false;
                    }
                }




            }else{ // just output everything to the file for states 0 and 4 :)
                if(d.regexNumber == 1 && stateLexerCurrent == 0){
                    readingRegex = true; //I am using
                    currentRegex+=myString.substr(d.startOfMatch, d.endOfMatch - d.startOfMatch);
                    // union
                }else if(d.regexNumber == 2 && stateLexerCurrent == 0){
                    // print to the other file
                    currentRegex+=myString.substr(d.startOfMatch, d.endOfMatch - d.startOfMatch);

                    fprintf(bridgeFile,"%s\n",currentRegex.c_str());
                    currentRegex = "";
                    readingRegex = false;
                    // end union
                }else {
                    if(readingRegex){
                        currentRegex+=myString.substr(d.startOfMatch, d.endOfMatch - d.startOfMatch);
                    }else {
                        if(stateLexerCurrent == 0){
                            fprintf(bridgeFile, "%s",
                                    myString.substr(d.startOfMatch, d.endOfMatch - d.startOfMatch).c_str());
                        }else {
                            fprintf(outFile, "%s",
                                    myString.substr(d.startOfMatch, d.endOfMatch - d.startOfMatch).c_str());
                        }
                    }
                }

            }
        }


        bool match(string s){
            this->myString = std::move(s);
            if(this->isDebugging) {
                printf("Starting on first phase of parsing\n");
            }
            bool res = stateLexer[stateLexerCurrent]->match(myString, false, false, this);
            shouldSwitchLexers = false;
            this->myString = myString.substr(offsetForNextLexer);
            offsetForNextLexer = 0;
            if(this->isDebugging) {
                printf("Starting on second phase of parsing\n");
            }
            res |= stateLexer[stateLexerCurrent]->match(myString, false, false, this);
            shouldSwitchLexers = false;
            this->myString = myString.substr(offsetForNextLexer);
            offsetForNextLexer = 0;

            if(this->isDebugging) {
                printf("Starting on third phase of parsing\n");
            }
            res |= stateLexer[stateLexerCurrent]->match(myString, false, false, this);
            shouldSwitchLexers = false;
            if(!hasPrintedRegexList){
                printRegexList();
                hasPrintedRegexList = true;
            }
            fclose(this->outFile);
            fclose(this->bridgeFile);
            if(this->isDebugging) {
                printf("Finished parsing input\n");
            }
            if(this->foundOCurly){
                throw std::invalid_argument("Unmatched { !! check your code!\n");
            }
            return res;

        }

        virtual ~LexGenerator() {

            int x = 3;
        }


        uint currentRegexNumber;
        bool readingOrExpression;
        uint nextOrLocation;
    };



}
#endif //JLEXX_STATELEXER_H
