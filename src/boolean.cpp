
#include <cstdlib>
#include "expression.hpp"
#include "parser.hpp"
#include "lexer.hpp"
#include "bool.hpp"

expression::node *bool_token_generator::get_next_token()
{
    int i;
    expression::node *new_node = nullptr;
    token_lexer::token tok;

    if (!lexer->get_token(&tok)) {
        printf("Lexer is empty\n");
        return nullptr;
    }

    switch (tok.letter) {
    case '+':
        new_node = new bool_expression::or_node();
        break;

    case '*':
        new_node = new bool_expression::and_node();
        break;

    case '1':
        new_node = new bool_expression::bool_value_node(true);
        break;

    case '0':
        new_node = new bool_expression::bool_value_node(false);
        break;

    case '(':
        new_node = new expression::node_paren_open();
        break;

    case ')':
        new_node = new expression::node_paren_close();
        break;

    default:
        lexer->push_token(&tok);
        return nullptr;
    }

    return new_node;
}

std::string bool_expression::and_node::to_string()
{
    if (this->left && this->right)
        return "(" + this->left->to_string() + " * " + this->right->to_string() + ")";
    else
        return "&";
}

std::string bool_expression::or_node::to_string()
{
    if (this->left && this->right)
        return "(" + this->left->to_string() + " + " + this->right->to_string() + ")";
    else
        return "|";
}

std::string bool_expression::bool_value_node::to_string()
{
    return (this->val)? "1": "0";
}

bool bool_expression::and_node::eval()
{
    bool_expression::bool_node *left = static_cast<bool_expression::bool_node *>(this->left);
    bool_expression::bool_node *right = static_cast<bool_expression::bool_node *>(this->right);

    return left->eval() & right->eval();
}

bool bool_expression::or_node::eval()
{
    bool_expression::bool_node *left = static_cast<bool_expression::bool_node *>(this->left);
    bool_expression::bool_node *right = static_cast<bool_expression::bool_node *>(this->right);

    return left->eval() | right->eval();
}

bool bool_expression::bool_value_node::eval()
{
    return this->val;
}

bool bool_expression::is_equal()
{
    bool res = true;
    bool result;
    bool_node *node;
    int i = 1;

    if (this->nodes.empty())
        return true;

    node = static_cast<bool_node *>(this->nodes.front());
    result = node->eval();

    printf("bool: Eval 0: %d\n", result);

    for (std::list<expression::node *>::iterator intr = ++this->nodes.begin(); intr != this->nodes.end(); ++intr) {
        bool result2;
        node = static_cast<bool_node *>(*intr);

        result2 = node->eval();

        printf("bool: Eval %d: %d\n", i, result2);
        if (result2 != result) {
            res = false;
            break;
        }
    }

    return res;

}

