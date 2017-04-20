#ifndef INCLUDE_ALGEBRA_HPP
#define INCLUDE_ALGEBRA_HPP

#include "expression.hpp"
#include "parser.hpp"

class algebra_expression : public expression {
  public:
    bool is_equal();

    struct algebra_node : node {
        virtual int eval() = 0;
    };

    struct plus_node : algebra_node {
        std::string to_string();
        int eval();

        plus_node() {
            type = NODE_OP;
            prec = 1;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        };
    };

    struct minus_node : algebra_node {
        std::string to_string();
        int eval();

        minus_node() {
            type = NODE_OP;
            prec = 1;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        };
    };

    struct mult_node : algebra_node {
        std::string to_string();
        int eval();

        mult_node() {
            type = NODE_OP;
            prec = 2;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        };
    };

    struct power_node : algebra_node {
        std::string to_string();
        int eval();

        power_node() {
            type = NODE_OP;
            prec = 3;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        };
    };

    struct value_node : algebra_node {
        std::string to_string();
        int eval();

        int value;

        value_node() {
            type = NODE_VALUE;
            value = 0;
        };
    };
};

class algebra_token_generator : public token_generator {
  private:
    expression::node::node_type past_node;

  public:
    void start_new_expression() { past_node = expression::node::node_type::NODE_OP; };
    expression::node *get_next_token();
    algebra_token_generator(token_lexer *l) : token_generator(l) { };
};

#endif
