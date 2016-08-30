#include <iostream>
#include "JLexx/RegexToDFABuilder.h"
#include "JLexx/LexLoader/Parser.h"
#include "JLexx/LexLoader/Generator.h"
#include "JLexx/RestBuilder/RestBuilder.h"
#include "JLexx/RestBuilder/Test/Test.h"
#include "RegularExpression/Regex/Regex.h"

using namespace std;

int main() {












  //    Rest::testPrintRoute();
//
//
  //  Rest::RestGenerator rg;
  //  Rest::RestBuilder b(rg,"/Users/josh/ClionProjects/JLexx/JLexx/RestBuilder/restEx.rest");
//
  //  rg.generateCode();
//
//
//
  //  rg.printAll();

    Lexx::JLexx lex;

    char * pwd = nullptr;
    pwd = getenv("PWD");
    if(pwd == nullptr){
        printf("pwd env variable was nullptr \n :(");
        exit(0);
    }else{
        printf("pwd = %s\n",pwd);
    }
    lex.match(FileLoader::fileToString("/Users/josh/ClionProjects/JLexx/JLexx/a.txt",true));

   //Generator g("/Users/josh/ClionProjects/JLexx/JLexx/a.txt");

    cout << "Hello, World!" << endl;
    return 0;
}