#include <iostream>
#include "JLexx/RegexToDFABuilder.h"


using namespace std;

int main() {


    RegexToDFABuilder lexx;
    RegexToDFABuilder lexx2;
    RegexToDFABuilder lexx3;
   //lexx.add("abc").add("abc").create();
   //lexx2.add("abc").add("abcd").create();
    lexx3.add(".*").add("[a-z]+").create();

    lexx3.match("a",true);
    lexx3.match("ab",true);
    lexx3.match("abc",true);
    lexx3.match("abcd",true);
    lexx3.match("aaaaaabcdabcd",true);
    lexx3.match("asda abc abcdef",true);
    lexx3.match("AAAAAAAAAAAf",true);
    cout << "Hello, World!" << endl;
    return 0;
}