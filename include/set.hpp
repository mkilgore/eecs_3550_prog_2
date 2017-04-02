#ifndef INCLUDE_SET_HPP
#define INCLUDE_SET_HPP

#include "expression.hpp"
#include "parser.hpp"
#include "char_set.hpp"

class set_expression : public expression {
  public:
    bool is_equal();

    struct set_node : node {
        virtual char_set eval() = 0;
    };

    struct intersect_node : set_node {
        std::string to_string();
        char_set eval();

        intersect_node() {
            type = NODE_OP;
            prec = 2;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        }
    };

    struct union_node : set_node {
        std::string to_string();
        char_set eval();

        union_node() {
            type = NODE_OP;
            prec = 1;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        }
    };

    struct set_value_node : set_node {
        std::string to_string();
        char_set eval();

        char_set set;

        set_value_node() {
            type = NODE_VALUE;
        }
    };
};

class set_token_generator : public token_generator {
  public:
    expression::node *get_next_token();
    set_token_generator(token_lexer *l) : token_generator(l) { };
};

#endif
