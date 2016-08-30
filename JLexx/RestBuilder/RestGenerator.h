//
// Created by joshua on 8/24/16.
//

#ifndef JLEXX_RESTGENERATOR_H
#define JLEXX_RESTGENERATOR_H

#include <vector>
#include "RestToken.h"
#include "RestGenHelper.h"

namespace Rest {
    class RestGenerator {
    private:
        const vector<string> fileNames = {
                "Get.js","Put.js","Post.js","Del.js",
                "Get.m" ,"Put.m" ,"Post.m" ,"Del.m" ,
                "Get.h" ,"Put.h" ,"Post.h" ,"Del.h" ,

        };
        unsigned int FILE_TYPE = 2;
        unsigned int DICT_TYPE = 3;
        unsigned int ARRAY_TYPE = 4;
        FILE * outFiles[12];
        std::string url;


    public:

        RestGenerator() {
            shouldPrintAdvancedPost = false;
        }

        std::vector<RestToken> tokenList;

        void handleToken(RestToken t) {

            tokenList.push_back(t);
        }

         void printRouteStringOBJC(FILE *pFILE, const  vector<pair<string, string>> & vector,int postParamStartIndex = -1){
            bool init = false;
            unsigned int paramCount = 0;

             if(postParamStartIndex == -1){
                 postParamStartIndex = vector.size();
             }
            fprintf(pFILE,"\tNSString * route = [NSString stringWithFormat:@\"%s",url.c_str());
             std::vector<int> params;
            for(int i = 0; i < vector.size() && i < postParamStartIndex ; i ++){
                auto pair = vector[i];
                if(pair.second != ""){ // its a param
                        fprintf(pFILE, "/%%@");
                        params.push_back(i);
                }else{
                        fprintf(pFILE, "/%s",pair.first.c_str());
                }
            }
             fprintf(pFILE,"\"");

            if(params.size()){
                for(int index: params) {
                    auto pair = vector[index];
                 if(pair.second != "") {
                         fprintf(pFILE,",%s", pair.first.c_str());
                         init = true;
                 }
                }
            }
            fprintf(pFILE,"];");
             if(postParamStartIndex != vector.size()){
                 printPostDataStartingAt(pFILE,postParamStartIndex,vector);
             }

        }
        void printPostDataStartingAt(FILE * file,int startIndex, const std::vector<pair<string,string> > & vec){
            fprintf(file,"\n\tNSDictionary * dict = \n\t\t@{");
            for(;startIndex < vec.size(); startIndex++){
                int lengthOfWord = vec[startIndex].first.size();
                int padding = 20 - lengthOfWord;
                fprintf(file,"\n\t\t\t@\"%s\"%*c : %-15s",vec[startIndex].first.c_str(),padding,' ',vec[startIndex].first.c_str());
                if(!(startIndex+1 == vec.size())){
                    // dont print a new line
                    fprintf(file,",");
                }
            }
            fprintf(file,"\n\t\t};\n");
        }
        char quote = '\"';
         void printRouteStringJS(FILE *pFILE, vector<pair<string, string>> vector,int postParamStartIndex = -1) {
            bool init = false;
            unsigned int paramCount = 0;

             if(postParamStartIndex != -1 && postParamStartIndex < vector.size()){// must be a POST
                 fprintf(pFILE,"\",\n\tupload.fields([");
                 for(int i = postParamStartIndex;i < vector.size(); i++){
                     // define multer
                     if(i == vector.size() -1){
                         fprintf(pFILE, "\n\t\t{name:'%s'}",vector[i].first.c_str());
                     }else {
                         fprintf(pFILE, "\n\t\t{name:'%s'},",vector[i].first.c_str());
                     }

                 }
                 quote = ' ';
                 fprintf(pFILE,"\n\t])");
             }

             fprintf(pFILE, "%c,function(req,res){\n",quote);
             fprintf(pFILE, "\t//BEGIN PARAMS \n");


             int i;
             if(postParamStartIndex == -1){postParamStartIndex = vector.size();}
            for ( i = 0; i < vector.size() && i < postParamStartIndex; i++) {
                auto pair = vector[i];
                if (pair.second != "") { // its a param
                    fprintf(pFILE, "\tvar %-12s = req.params.%s;\n",pair.first.c_str(),pair.first.c_str());
                    paramCount++;

                }

            }
             fprintf(pFILE, "\t//END PARAMS \n");

             if(postParamStartIndex != -1 && postParamStartIndex < vector.size()){
                 fprintf(pFILE,"\n\t//BEGIN POST PARAMS\n");
                 for(;postParamStartIndex < vector.size(); postParamStartIndex++){
                     string & name = vector[postParamStartIndex].first;
                     fprintf(pFILE, "\tvar %-12s = JSON.parse(req.body.%s);\n",name.c_str(),name.c_str());

                 }
                 fprintf(pFILE,"\n\t//END   POST PARAMS\n");
             }



        }



        void generateCode() {


            if(tokenMap[tokenList[0].ID] != "URL" ){
                throw std::invalid_argument("You need to put the base url in the beginning of every file!! ex\n http://localhost:3000/ the regex is http://[^/]*/ so you need a /!!\n");
            }else{
                this->url = tokenList[0].text;
                this->url.erase(this->url.begin());
                this->url.pop_back();
            }
            openFiles();
            for(unsigned int i = 0; i < tokenList.size(); i ++){
                if(tokenMap[tokenList[i].ID] == "WHITESPACE"){
                    // remove it
                    tokenList.erase(tokenList.begin() + i);
                }
            }

            for (unsigned int i = 0; i < tokenList.size(); i++) {

                // look for GET token
                unsigned int jsFileNo = -1;
                unsigned int objFileNo = -1;

                string verb = "";
                while (tokenMap[tokenList[i].ID] != "VERB" && i < tokenList.size()) {
                    i++;
                }
                if(i == tokenList.size()){
                    break;
                }
                verb = tokenList[i].text;
                // i hate the c++ library it sucks so bad
                for(unsigned int i= 0 ; i < verb.size(); i ++){
                    if((verb[i] - 'A') < 26 && verb[i] >= 'A'){
                        verb[i] = (verb[i] -'A') + 'a';
                    }
                }
                setFiles(verb,jsFileNo,objFileNo);

                i++;
                if(i > tokenList.size()){
                    break;
                }
                // only handling get atm


                fprintf(outFiles[jsFileNo],"\nrouter.%s(\"",verb.c_str());
                fprintf(outFiles[objFileNo],"\n+(void) %s",verb.c_str());
                fprintf(outFiles[objFileNo+4],"\n+(void) %s",verb.c_str());
                bool methodStarted = false;
                bool returnSeen = false;
                int postParamIndex = -1;
                unsigned int numParams = 0;
                // unsigned char type = 0;
                typedef pair<string,string> PPair;
                vector<pair< string, string > > paramList; // used for obj c method stub
                FILE *& js = outFiles[jsFileNo];
                FILE *& objm = outFiles[objFileNo];
                FILE *& objh = outFiles[objFileNo+4];

                while (tokenMap[tokenList[i].ID] != ";" && i < tokenList.size()) {
                    if (!returnSeen) { // still writing route
                        if(tokenMap[tokenList[i].ID] == "TYPE"){
                            string type = getObjCType(tokenList[i].text);
                            i++;
                            if(postParamIndex == -1) {
                                fprintf(js, "/:%s", tokenList[i].text.c_str());
                            }
                            if(!methodStarted){
                                fprintf(objm, "%s:(%s)%s ",tokenList[i].text.c_str(),type.c_str(),tokenList[i].text.c_str());
                                fprintf(objh,"%s:(%s)%s ", tokenList[i].text.c_str(),type.c_str(),tokenList[i].text.c_str());

                                methodStarted = true;
                            }else{
                                fprintf(objm, " and%s:(%s)%s",tokenList[i].text.c_str() ,type.c_str(),tokenList[i].text.c_str());
                                fprintf(objh, " and%s:(%s)%s",tokenList[i].text.c_str(),type.c_str(),tokenList[i].text.c_str());

                            }

                            numParams++;
                            if((numParams % 3)  == 0){
                                fprintf(objh,"\n\t");
                            }
                            paramList.push_back(PPair(tokenList[i].text,type));

                        }else if(tokenMap[tokenList[i].ID] == "RETURN"){
                            if(numParams != 0){
                                fprintf(objm ," withComp:(void (^)(BOOL, id))onCompletion{\n");
                                fprintf(objh ," withComp:(void (^)(BOOL, id))onCompletion;\n");
                            }else{
                                fprintf(objm ,"withComp:(void (^)(BOOL, id))onCompletion{\n");
                                fprintf(objh ,"withComp:(void (^)(BOOL, id))onCompletion;\n");
                            }

                            printRouteStringJS(js,paramList,postParamIndex);
                            printRouteStringOBJC(objm,paramList,postParamIndex);
                            fprintf(objm,"\n");
                            printJSReturn(js,i);
                            if(postParamIndex>=0){
                                printAdvancedPost(objm,fileNames[objFileNo],postParamIndex);
                            }else {
                                printURLSessionBegin(objm, fileNames[objFileNo]);
                                printCompletionStatement(objm, i);
                                printURLSessionEnd(objm);
                            }
                            fprintf(objm,"\n");
                            i++;
                            break;

                            returnSeen = true;
                        }else if(tokenMap[tokenList[i].ID] == "WHITESPACE") {
                            // do nada
                        }else if(tokenMap[tokenList[i].ID] == "NAME"){
                            // just a routing varaib;e
                            fprintf(js,"/%s",tokenList[i].text.c_str());
                            if(!methodStarted){
                               fprintf(objm, "%s", tokenList[i].text.c_str());
                               fprintf(objh, "%s", tokenList[i].text.c_str());

                            }
                            paramList.push_back(PPair(tokenList[i].text,""));

                        }else if(tokenMap[tokenList[i].ID] == "POST_PARAMS"){

                            postParamIndex  = paramList.size() ;
                        }

                    } else {


                    }

                    i++;
                }
                // print the end of the methods
                fprintf(js,"\n});\n");
                fprintf(objm ,"\n}\n");


            }
            closeFiles();
        }

        void printAll() {
            for (RestToken &t : tokenList) {
                printf("Token ID : %d\n Token string : %s\n", t.ID, t.text.c_str());
            }

        }

        void closeFiles(){
            for(unsigned int i = 0 ; i <12 ; i++){
                if(i > 3){
                    if(i == 6 && shouldPrintAdvancedPost){
                        fprintf(outFiles[i],"%s\n",advancedPost.c_str());
                    }
                    fprintf(outFiles[i],"\n@end // %s\n",fileNames[i].c_str());
                }else{
                    fprintf(outFiles[i],"\nmodule.exports = router;");
                }
                fclose(outFiles[i]);

            }
        }
        const string OBJHeader = "#import \"HTTPHelper.h\"\n";


        std::string splitGetfirst(std::string s, const char split){
            std::string toReturn = "";
            unsigned int curr = 0;
            while(curr < s.length() && s[curr] != split){
                toReturn.push_back(s[curr++]);
            }
            return toReturn;
        }
        void openFiles() {

            std::string jsPath =  "/Users/josh/ClionProjects/JLexx/JLexx/RestBuilder/Rest/JS/";
            std::string objPath = "/Users/josh/ClionProjects/JLexx/JLexx/RestBuilder/Rest/OBJC/";

            for(unsigned int i = 0; i < 12; i ++){
                outFiles[i] = nullptr;
            }
            for(unsigned int i = 0; i <4 ;i++){
                outFiles[i] = fopen((jsPath +  fileNames[i]).c_str(),"w");
                fprintf(outFiles[i],"\n//-------GENERATED FILE-------\n");
                fprintf(outFiles[i],"var express = require('express');\nvar router = express.Router();\n");
                if(i == 2){
                     // post
                    fprintf(outFiles[i],"var multer = require('multer');\n"
                                        "var upload = multer({ dest: 'uploads/' });\n");
                }
            }
            for(unsigned int i = 4; i < 12; i++){
                outFiles[i] = fopen((objPath+  fileNames[i]).c_str(),"w");
                fprintf(outFiles[i],"\n//-------GENERATED FILE-------\n");
                if(i > 7) {
                    string fileName = splitGetfirst(fileNames[i],'.');
                    fprintf(outFiles[i],"%s", OBJHeader.c_str());
                    fprintf(outFiles[i],"@interface %s : NSObject\n",fileName.c_str());
                }else{
                    string fileName = splitGetfirst(fileNames[i],'.');
                    fprintf(outFiles[i],"#import \"%s\"\n",fileNames[i+4].c_str());
                    fprintf(outFiles[i],"@implementation %s \n",fileName.c_str());
                    fprintf(outFiles[i],"static const NSString * baseURL = @\"%s\";\n",url.c_str());
                    const string & basicHeader = i == 4 ? basicGet : i == 5? basicPut : i == 6 ? basicPost : basicDelete;
                    if(i >= 4 && i <= 7){
                        // basic get
                        fprintf(outFiles[i],"%s\n",basicHeader.c_str());
                    }
                }

            }

            if(outFiles[0] == nullptr){
                perror("\nFailed to open files Make Sure you have folders with Rest/JS/ and Rest/OBJC/ !!!\n");
                exit(0);
            }
        }

        void printCompletionStatement(FILE * f,unsigned int currLoc){
            for(;currLoc < this->tokenList.size(); currLoc++){
                if(tokenList[currLoc].text == "dictionary"){
                    printDictHandler(f);
                    return;
                }else if(tokenList[currLoc].text == "array"){
                    printArrayHandler(f);
                    return;
                }
            }
            throw std::invalid_argument("Need a return statement in the form of \nreturn \"Success\" -> dictionary|array\nreturn \"Error\" -> string");

        }
        void printAdvancedPost(FILE * f, string fileName ,int postStartIndex){
            fprintf(f,"\n\t[Post postHTTP:route andParams:dict andCompletion:onCompletion];\n");
            shouldPrintAdvancedPost = true;
        }
        void printJSReturn(FILE * f, unsigned int currLoc){
            fprintf(f,"\n\tvar success = false;                       // set this to true or false on completion\n");
            fprintf(f,  "\tvar toReturn = \"Not yet implemented :(\"; // set this\n");
            fprintf(f,"\n\t// TODO: Implement Body!\n\n");
            fprintf(f,"\tif(success){\n\t\tres.send({\"Success\":toReturn});\n\t}else{\n\t\tres.send({\"Error\":toReturn});\n\t}\n");
            bool found = false;

        }
        void printURLSessionBegin(FILE * f,string fileName){
            fileName = splitGetfirst(fileName,'.');
            fprintf(f,"\tNSURLSessionTask  * task = [%s getBasicTaskWithStringURL:route completionHandler:^(NSData *data, NSURLResponse *response, NSError *error) {\n",fileName.c_str());
        }
        void printDictHandler(FILE * f){
            fprintf(f,"\t\tbasicResponseHandler(DictionaryResponseHandler);\n");
        }
        void printArrayHandler(FILE * f){
            fprintf(f,"\t\tbasicResponseHandler(ArrayResponseHandler);\n");

        }
        void printURLSessionEnd(FILE * f) {
            fprintf(f,"\t}];\n\t[task resume];\n");

        }


        bool shouldPrintAdvancedPost;
    };




};

#endif //JLEXX_RESTGENERATOR_H
