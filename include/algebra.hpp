#ifndef INCLUDE_ALGEBRA_HPP
#define INCLUDE_ALGEBRA_HPP

#include "expression.hpp"
#include "parser.hpp"

class algebra_expression : public expression {
  public:
    bool is_equal();

    struct algebra_node : node {
        virtual bool is_value() = 0;
        //virtual algebra_node *eval() = 0;
    };

    struct plus_node : algebra_node {
        std::string to_string();
        bool is_value();

        plus_node() {
            type = NODE_OP;
            prec = 1;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        };
    };

    struct minus_node : algebra_node {
        std::string to_string();
        bool is_value();

        minus_node() {
            type = NODE_OP;
            prec = 1;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        };
    };

    struct mult_node : algebra_node {
        std::string to_string();
        bool is_value();

        mult_node() {
            type = NODE_OP;
            prec = 2;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        };
    };

    struct power_node : algebra_node {
        std::string to_string();
        bool is_value();

        power_node() {
            type = NODE_OP;
            prec = 3;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        };
    };

    struct value_node : algebra_node {
        std::string to_string();
        bool is_value() { return true; };

        int value;

        value_node() {
            type = NODE_VALUE;
            value = 0;
        };
    };

    struct variable_node : algebra_node {
        std::string to_string();
        bool is_value() { return false; };

        char var;

        variable_node() {
            type = NODE_VALUE;
            var = 0;
        };
    };
};

class algebra_token_generator : public token_generator {
  public:
    expression::node *get_next_token();
    algebra_token_generator(token_lexer *l) : token_generator(l) { };
};

#endif
