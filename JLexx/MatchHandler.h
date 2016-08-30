//
// Created by joshua on 8/18/16.
//

#ifndef JLEXX_MATCHHANDLER_H
#define JLEXX_MATCHHANDLER_H
#include <string>
namespace Lexx {
    struct Data {
        unsigned int startOfMatch;
        unsigned int endOfMatch;
        unsigned int regexNumber;//== tokenNumber
        /*
         * [a-d]+[012basd]*      // regex 0
         * [d]+abcd*             // regex 1
         * .+                    // regex 2
         *
         */
    };
    class DataHandler{
    public:
        virtual void handleData(Data & d) = 0;
    };

    class Tokenizer : public DataHandler{
    private:

    public:

        virtual void handleData(Data &d) override;

    };


};
#endif //JLEXX_MATCHHANDLER_H
