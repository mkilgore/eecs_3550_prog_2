#ifndef INCLUDE_EXPRESSION_HPP
#define INCLUDE_EXPRESSION_HPP

#include <string>
#include <list>

class expression {
  public:
    virtual std::string to_string();
    virtual bool is_equal();

    struct node {
        enum node_type {
            NODE_OP,
            NODE_VALUE,
            NODE_PAREN_OPEN,
            NODE_PAREN_CLOSE,
        } type;

        int prec;
        enum op_assoc {
            ASSOC_LEFT,
            ASSOC_RIGHT,
        } assoc;
        int is_binary;

        struct node *left, *right;

        virtual std::string to_string() = 0;
        node() {
            left = nullptr;
            right = nullptr;
        };

        virtual ~node() {
            delete left;
            delete right;
        };
    };

    struct node_paren_open : node {
        std::string to_string() { return "("; };

        node_paren_open() {
            type = NODE_PAREN_OPEN;
            prec = 0;
        };
    };

    struct node_paren_close : node {
        std::string to_string() { return ")"; };

        node_paren_close() {
            type = NODE_PAREN_CLOSE;
            prec = 0;
        };
    };

    std::list<node *> nodes;

    virtual ~expression() {
        int s = this->nodes.size();
        int i;

        for (i = 0; i < s; i++) {
            node *n = this->nodes.front();
            this->nodes.pop_front();

            delete n;
        }
    }
};

class algebra_expression : public expression {

};

#endif
