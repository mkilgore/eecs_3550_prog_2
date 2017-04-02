#ifndef INCLUDE_BOOL_HPP
#define INCLUDE_BOOL_HPP

#include "expression.hpp"
#include "parser.hpp"

class bool_expression : public expression {
  public:
    bool is_equal();

    struct bool_node : node {
        virtual bool eval() = 0;
    };

    struct and_node : bool_node {
        std::string to_string();
        bool eval();

        and_node() {
            type = NODE_OP;
            prec = 2;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        };
    };

    struct or_node : bool_node {
        std::string to_string();
        bool eval();

        or_node() {
            type = NODE_OP;
            prec = 1;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        };
    };

    struct bool_value_node : bool_node {
        std::string to_string();
        bool eval();

        bool val;

        bool_value_node() {
            type = NODE_VALUE;
            val = false;
        };

        bool_value_node(bool v) {
            type = NODE_VALUE;
            val = v;
        };
    };
};

class bool_token_generator : public token_generator {
  public:
    expression::node *get_next_token();
    bool_token_generator(token_lexer *l) : token_generator(l) { };
};

#endif
