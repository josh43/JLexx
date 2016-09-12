#include <iostream>
#include "JLexx/RegexToDFABuilder.h"


#include "JLexx/RestBuilder/RestBuilder.h"
#include "JLexx/RestBuilder/Test/Test.h"
#include "RegularExpression/Regex/Regex.h"
#include "JLexx/LexLoader/LexGenerator.h"
#include "JLexx/LexLoader/GenFiles/testLexx.h"
using namespace std;

int main() {



     Lexx::LexGenerator sL;
     sL.match(FileLoader::fileToString("/home/josh/ClionProjects/JLexx/JLexx/a.txt",true));
     jmain();
    return 0;
}