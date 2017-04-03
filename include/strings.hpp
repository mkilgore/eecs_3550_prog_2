#ifndef INCLUDE_STRINGS_HPP
#define INCLUDE_STRINGS_HPP

#include "expression.hpp"
#include "parser.hpp"

class string_expression : public expression {
  public:
    bool is_equal();

    struct string_node : node {
        virtual std::string eval(bool is_algebra) = 0;
    };

    struct plus_node : string_node {
        std::string to_string();
        std::string eval(bool is_algebra);

        plus_node() {
            type = NODE_OP;
            prec = 1;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        };
    };

    struct mult_node : string_node {
        std::string to_string();
        std::string eval(bool is_algebra);

        mult_node() {
            type = NODE_OP;
            prec = 2;
            assoc = ASSOC_LEFT;
            is_binary = 1;
        };
    };

    struct value_node : string_node {
        std::string to_string();
        std::string eval(bool is_algebra);

        std::string str;

        value_node() {
            type = NODE_VALUE;
            str = "";
        };
    };
};

class string_token_generator : public token_generator {
  public:
    expression::node *get_next_token();
    string_token_generator(token_lexer *l) : token_generator(l) { };
};

#endif
