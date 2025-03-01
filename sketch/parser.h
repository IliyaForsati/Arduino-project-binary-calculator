#ifndef PARSER_H
#define PARSER_H

/*
PARSER CFG:
S -> E
E -> T + E | T - E | T
T -> A * T | A / T | A
A -> (E) | N
N -> D | DN
D -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
*/

#include "Arduino.h"

enum ResultType {
        SUCCESS,
        FAILURE
};

enum Operator {
        ADD,
        SUB,
        MUL,
        DIV,
        AXIOM
};

struct ParseTree {
        Operator op;
        ParseTree *left = NULL;
        ParseTree *right = NULL;
        int value;
        ResultType type;

        ParseTree() {
                this->op = AXIOM;
                this->left = NULL;
                this->right = NULL;
                this->value = 0;
                this->type = FAILURE;
        }

        ParseTree(Operator op, ParseTree *left, ParseTree *right, int value) {
                this->op = op;
                this->left = left;
                this->right = right;
                this->value = value;
                this->type = SUCCESS;
        }

        ParseTree(int value) {
                this->op = AXIOM;
                this->left = NULL;
                this->right = NULL;
                this->value = value;
                this->type = SUCCESS;
        }

        ParseTree(Operator op, ParseTree *left, ParseTree *right) {
                this->op = op;
                this->left = left;
                this->right = right;
                this->value = 0;
                this->type = FAILURE;

                if (left == NULL || right == NULL) {
                        this->type = FAILURE;
                        return;
                }

                if (left->type == FAILURE || right->type == FAILURE) {
                        this->type = FAILURE;
                        return;
                }

                if (op == ADD) {
                        this->value = left->value + right->value;
                } else if (op == SUB) {
                        this->value = left->value - right->value;
                } else if (op == MUL) {
                        this->value = left->value * right->value;
                } else if (op == DIV) {
                        this->value = left->value / right->value;
                } else if (op == AXIOM) {
                        this->value = left->value;
                }

                this->type = SUCCESS;
        }
};

struct ParserContainer {
        String input;
        int index;

        ParserContainer(String input) {
                this->input = input;
                this->index = 0;
        }

        char curr() {
                skipWhitespace();
                return input[index];
        }

        char next() {
                skipWhitespace();
                return input[index++];
        }

        bool hasNext() {
                return index < input.length();
        }

        bool isEnd() {
                skipWhitespace();
                return index == input.length();
        }

        void skipWhitespace() { // TODO: changed
                while (!index != input.length() && (input[index] == ' ' || input[index] == '\n' || input[index] == '\t')) {
                        index++;
                }
        }
};

struct ParsingResult {
        ResultType type;
        int result;

        ParsingResult(ResultType type, int result) {
                this->type = type;
                this->result = result;
        }
};

ParseTree* parse(String input);
ParseTree* parseS(ParserContainer &container);
ParseTree* parseE(ParserContainer &container);
ParseTree* parseT(ParserContainer &container);
ParseTree* parseA(ParserContainer &container);
ParseTree* parseN(ParserContainer &container);
ParseTree* parseD(ParserContainer &container);

ParseTree* parse(String input) {
        ParserContainer container(input);
        ParseTree* tree = parseS(container);
        if (!container.isEnd())
                tree -> type = FAILURE;

        return tree;
}

ParseTree* parseS(ParserContainer &container) {
        return parseE(container);
}

ParseTree* parseE(ParserContainer &container) {
        ParseTree* left_node = parseT(container);
        // Serial.println("parseE called");
        // Serial.flush();
        if (left_node == NULL) {
                // Serial.println("parseE failed");
                return NULL;
        }

        if (container.isEnd()) {
                return left_node;
        }


        char c = container.curr();
        if (c == '+') {
                container.next();
                return new ParseTree(ADD, left_node, parseE(container));
        }

        if (c == '-') {
                container.next();
                return new ParseTree(SUB, left_node, parseE(container));
        }

        return left_node; // TODO: changed
}

ParseTree* parseT(ParserContainer &container) {
        ParseTree* left_node = parseA(container);
        if (left_node == NULL) {
                return NULL;
        }

        if (container.isEnd()) {
                return left_node;
        }

        char c = container.curr();
        if (c == '*') {
                container.next();
                return new ParseTree(MUL, left_node, parseE(container));
        }

        if (c == '/') {
                container.next();
                return new ParseTree(DIV, left_node, parseE(container));
        }
        return left_node; // TODO: changed
}

ParseTree* parseA(ParserContainer &container) {
        if (container.curr() == '(') {
                container.next();
                ParseTree* node = parseE(container);
                if (container.next() != ')') {
                        return NULL;
                }

                return node;
        }
 
        return parseN(container);
}

ParseTree* parseN(ParserContainer &container) {
        if (!isdigit(container.curr())) {
                return NULL;
        }

        int value = 0;
        while (!container.isEnd() && isdigit(container.curr())) {
                value = value * 10 + (container.next() - '0');
        }

        return new ParseTree(value);
}

#endif