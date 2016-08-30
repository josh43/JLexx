//
// Created by joshua on 8/24/16.
//

#ifndef JLEXX_TOKEN_H
#define JLEXX_TOKEN_H

#include <string>
#include <map>
namespace Rest {
    static std::vector<std::string> tokenMap =
            {
                    "VERB",
                    "VERB",
                    "VERB",
                    "VERB",
                    "POST_PARAMS",
                    "TYPE",
                    "FILENAME",
                    "TYPE",
                    "TYPE",
                    "RETURN",
                    "SUCCESS",
                    "ERROR",
                    "TYPE",
                    "COLON",
                    "OR",
                    "RETURN_MARKER",
                    "SEMI-COLON",
                    "URL",
                    "WHITESPACE",
                    "NAME",
                    "OTHER"
            };

    struct RestToken {
        unsigned int ID;
        std::string text;
    };


};
#endif //JLEXX_TOKEN_H
