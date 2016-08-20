//
// Created by joshua on 8/10/16.
//

#ifndef LANGUAGEIMPLEMENTATIONPATTERNS_REGULARPARSER_H
#define LANGUAGEIMPLEMENTATIONPATTERNS_REGULARPARSER_H


#include <stack>
#include "Parser.h"
#include "../Utility.h"
#include "RegularTreeNodes.h"
#include <stack>


class RegularParser : public Parser {


public:

    //
    string regularExpression;
    RegularDeque deque;

    RegularParser(Lexer *l) : Parser(l),regularExpression("") { }
    void parse() {
        basicRegexp();
        if(this->deque.size() > 1){
            throw std::invalid_argument("Error parsing regular expression stack size should be 1");
        }
    }
    void  NONTERMINAL(string x) {
        return;
        printf("%s() <- ", x.c_str());
        for (int i = this->markers.back(); i < curr; i++) {
            printf("%s\t", this->lookAhead[i].toString().c_str());
        }
        printf("\n");
    }

private:

    void basicRegexp() {
        if (this->getLookaheadTypeAt(1) == EOF_TOKEN){
            throw std::invalid_argument("Cant parse """);
        }

            if (this->getLookaheadTypeAt(1) == L_ANCHOR) {
                match(L_ANCHOR);
                deque.lAnchor  = true;
                prettyPrint("L_ANCHOR");
                if (this->getLookaheadTypeAt(1) == R_ANCHOR) {

                    throw std::invalid_argument("Cannot have just ^$");
                } else {
                    mark();
                    if (RE_expression()) {
                        while(RE_expression()){ deque.combineLastTwoExpressions();}
                        if (this->getLookaheadTypeAt(1) == R_ANCHOR) {
                            match(R_ANCHOR);
                            deque.rAnchor = true;
                            return;
                            //L_ANCHOR RE R_ANCHOR
                        } else {
                            // LANCHOR  RE
                        }
                    } else {
                        release();
                        throw std::invalid_argument("Cannot have just ^");

                    }
                }
        } else if (this->getLookaheadTypeAt(1) == R_ANCHOR) {
            match(R_ANCHOR);
                deque.rAnchor = true;
            prettyPrint("R_ANCHOR");
            return;
        }else {
                mark();
                if (RE_expression()) {
                    while (RE_expression()) { deque.combineLastTwoExpressions(); }
                    prettyPrint("RE_expression");
                    if (this->getLookaheadTypeAt(1) == R_ANCHOR) {
                        match(R_ANCHOR);
                        deque.rAnchor = true;
                    } else {
                    }

                } else {
                    release();
                    throw std::invalid_argument("Bad regular expresssion :(");
//            unexpectedErr("Unable to parse basic Regexp","asd ");
                }
            }
    }
/*
 * RE_expression    : ( RE_expression ) opDuplSymbol
 *                  : ( RE_expression  | RE_expression )  opDuplSymbol
 *                  : expr RE_expression
 *                  : expr
 *
 *
 */
    bool RE_expression() {

        if (this->getLookaheadTypeAt(1) == L_PAREN) {
            // try and do expression
            mark();
            match(L_PAREN);
            if (RE_expression()) { // this should succeed for case 12
                while(this->getLookaheadTypeAt(1) == OR) {
                    mark();
                    match(OR);
                    // try and do second RE
                    if (RE_expression()) {
                        // just fall off
                        deque.combineLastTwoExpressions(OR);
                    } else {
                        printError("You must match or expressions",this->lexer->input,'|',curr);
                        throw std::invalid_argument("Expecting another expression after |");
                    }
                }

                if (this->getLookaheadTypeAt(1) == R_PAREN) {
                    regularExpression.push_back(')');
                    match(R_PAREN);
                    if(deque.isEmpty()){throw std::invalid_argument("Stack shouldnt be empty!");}
                    RegularNode * top = deque.pop();
                    opDuplSymbol(top,2);
                    deque.push(top);
                   //stack.push(top); dont push back in this case opDupl will do for you!
                    NONTERMINAL("RE_expression()");
                    return true;
                } else {
                    return false;
                }
            } else {
                release();
                return false; // failed
            }
        }
        mark();
        if (simple_RE()) {
            mark();

            if (RE_expression()) {
                deque.combineLastTwoExpressions();
            } else {
                release();
            }
            NONTERMINAL("RE_expression()");
            return true;
        } else {
            release();
            return false;
        }
    }

    /* simple_RE        : oneOrMoreSymbols
     *                  : BRE
     *                  : BRE duplSymbol
     * oneOrMoreSymbols : ORD_CHAR opWildCardSymbols oneOrMoreSymbols
     *                  : ORD_CHAR opWildCardSymbols
     *
     * opWildCardSymbols: '.' '*'
     *                  : '.' '+'
     *                  : '.'
     *                  : '+'
     *                  : '*'
     *                  : '?'
     *                  :
     *
     */
    bool simple_RE() {
        mark();

        if (oneOrMoreChars()) {
            NONTERMINAL("simple_RE()");
            return true;
        }
        release();
        mark();
        if (BRE()) {
            NONTERMINAL("simple_RE()");
            return true;
        }
        release();
        return false;

    }

    bool oneOrMoreChars() {
        RegularNode *  gen = nullptr;
       bool matched = false;
        bool shouldCombine = false;
        while(true) {
            switch(this->getLookaheadTypeAt(1)) {
                case SPEC_CHAR:
                case ORD_CHAR: case DOT:
                    gen = new RegularNode();
                    gen->nodeType = DATA_NODE;
                    if(this->getLookaheadAt(1).text.front() == '\\'){
                        gen->regularData += this->getLookaheadAt(1).text[1];

                    }else {
                        gen->regularData += this->getLookaheadAt(1).text;
                    }
                    match(this->getLookaheadTypeAt(1));
                    opDuplSymbol(gen);
                    deque.push(gen);
                    matched = true;
                    if(shouldCombine){
                        deque.combineLastTwoExpressions();
                    }
                    shouldCombine = true;
                    continue;
                default:
                    return matched;

            }
        }
    }

    void opDuplSymbol(RegularNode * regular,unsigned int expression = 0){
        // 0 -- basic symbol on letter or a SPEC Char like  2
        // 1->inf -- expression  2
        switch (this->getLookaheadTypeAt(1)) {
            case QUESTION:
            case STAR:
            case PLUS:
               // if(!expression) {
               //     stack.duplSymbolLast(regular, this->getLookaheadAt(1));
               // }else{
                   //RegularNode * dupNode = new RegularNode();dupNode->left = regular;
                   //dupNode->nodeType = 0;
                    deque.duplSymbolLast(regular, this->getLookaheadAt(1));
                  //  stack.push(regular);
                //}
                match(this->getLookaheadTypeAt(1));

                return;
            default:
                return;
        }
    }

    bool BRE() {
        mark();


        if (this->getLookaheadTypeAt(1) == L_BRACKET) {
            RegularNode * regular = new RegularNode();
            regular->nodeType = BRACKET_NODE;
            match(L_BRACKET);
            if (this->getLookaheadTypeAt(1) == L_ANCHOR) {
                regular->nodeType |= NEGATED_BRACKET;
                match(L_ANCHOR); // negation
            }
            // match everything
            int charactersBeforeDash = 0;
            while (this->getLookaheadTypeAt(1) != R_BRACKET) {
                if(this->getLookaheadTypeAt(1) == EOF_TOKEN){
                    throw std::invalid_argument(
                            "Reached enf of input...\n");
                }
                if (this->getLookaheadTypeAt(1) == DASH) {
                    if (charactersBeforeDash == 0) {
                        throw std::invalid_argument(
                                "You cannot have a dash as the character right after the brakcet\n");
                    }
                    charactersBeforeDash = 0;

                }
                regular->regularData+=this->getLookaheadAt(1).text;
                charactersBeforeDash++;
                match(this->getLookaheadTypeAt(1));
            }
            match(R_BRACKET);
            NONTERMINAL("BRE()");
            opDuplSymbol(regular,true);
            deque.push(regular);
            return true;
        }else{
            return false;
        }
    }
    /*
 basic_reg_exp  :          RE_expression
               | L_ANCHOR
               |                        R_ANCHOR
               | L_ANCHOR               R_ANCHOR
               | L_ANCHOR RE_expression
               |          RE_expression R_ANCHOR
               | L_ANCHOR RE_expression R_ANCHOR
               ;
RE_expression  : simple_RE  RE_Right_expr
               | open_paren RE_expression close_paren
               |
RE_Right_expr  |
               ; simple_RE RE_expression
               ;
simple_RE      : nondupl_RE
               | nondupl_RE RE_dupl_symbol
               ;
nondupl_RE     : one_char_or_coll_elem_RE
               | BACKREF
               ;
one_char_or_coll_elem_RE : ORD_CHAR
               | QUOTED_CHAR
               | '.'
               | bracket_expression
               ;
RE_dupl_symbol : '*'
               | '+'
               | '?'
               | Back_open_brace DUP_COUNT               Back_close_brace
               | Back_open_brace DUP_COUNT ','           Back_close_brace
               | Back_open_brace DUP_COUNT ',' DUP_COUNT Back_close_brace
 */



};

#endif //LANGUAGEIMPLEMENTATIONPATTERNS_REGULARPARSER_H

