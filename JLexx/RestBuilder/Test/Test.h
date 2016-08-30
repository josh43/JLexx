//
// Created by joshua on 8/24/16.
//

#ifndef JLEXX_TEST_H
#define JLEXX_TEST_H

#include <stdlib.h>
#include "../RestGenerator.h"
namespace Rest {

    void testPrintRoute() {

        using namespace std;
        FILE *testOut = fopen("/Users/josh/ClionProjects/JLexx/JLexx/RestBuilder/Test/test.txt", "w");
        if (!testOut) {
            perror("On testPrintRoute()");
            exit(0);
        }
        vector<pair<string, bool> > list = {
                {"login",    false},
                {"username", true},
                {"password", true},
                {"test",     false}
        };
       // RestGenerator::printRouteStringOBJC(testOut, list);
//
       // RestGenerator::printRouteStringJS(testOut, list);

        fclose(testOut);

    }
};

#endif //JLEXX_TEST_H
