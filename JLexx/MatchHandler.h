//
// Created by joshua on 8/18/16.
//

#ifndef JLEXX_MATCHHANDLER_H
#define JLEXX_MATCHHANDLER_H
#include <string>
namespace JLexx {
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
        virtual void streamToken(const std::string & s,uint regexMatch) = 0;
        virtual bool shouldStop() = 0;
    };

    class Tokenizer : public DataHandler{
    private:

    public:

        virtual void handleData(Data &d) override;

        virtual bool shouldStop() override {
            return false;
        }

    };


};
#endif //JLEXX_MATCHHANDLER_H
