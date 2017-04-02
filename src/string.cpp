
#include <cstdlib>
#include "expression.hpp"
#include "parser.hpp"
#include "strings.hpp"

class alnum_char_set : public char_set {
  public:
    alnum_char_set() {
        add_range('a', 'z');
        add_range('A', 'Z');
        add_range('0', '9');
    }
};

static alnum_char_set alnum_set;

expression::node *string_token_generator::get_next_token()
{
    int i;
    expression::node *new_node = nullptr;
    string_expression::string_node_value *str_node = nullptr;
    token_lexer::token tok;

    if (!lexer->get_token(&tok)) {
        printf("Lexer is empty\n");
        return nullptr;
    }

    switch (tok.letter) {
    case '+':
        new_node = new string_expression::plus_node();
        break;

    case '*':
        new_node = new string_expression::mult_node();
        break;

    case '(':
        new_node = new expression::node_paren_open();
        break;

    case ')':
        new_node = new expression::node_paren_close();
        break;

    case 'a' ... 'z':
    case 'A' ... 'Z':
    case '0' ... '9':
        str_node = new string_expression::string_node_value();

        str_node->str += tok.letter;
        lexer->get_string(&str_node->str, &alnum_set);

        new_node = str_node;
        break;

    default:
        lexer->push_token(&tok);
        return nullptr;
    }

    return new_node;
}

std::string string_expression::plus_node::to_string()
{
    if (this->left && this->right)
        return "(" + this->left->to_string() + " + " + this->right->to_string() + ")";
    else if (this->left)
        return this->left->to_string();
    else if (this->right)
        return this->right->to_string();
    else
        return " + ";
}

std::string string_expression::mult_node::to_string()
{
    if (this->left && this->right)
        return "(" + this->left->to_string() + " * " + this->right->to_string() + ")";
    else
        return "*";
}

std::string string_expression::string_node_value::to_string()
{
    return this->str;
}

std::string string_expression::plus_node::eval(bool is_algebra)
{
    string_expression::string_node *left = static_cast<string_expression::string_node *>(this->left);
    string_expression::string_node *right = static_cast<string_expression::string_node *>(this->right);

    if (!is_algebra)
        return left->eval(false) + right->eval(false);
    else
        return std::to_string(std::stoi(left->eval(true)) + std::stoi(right->eval(true)));
}

std::string string_expression::mult_node::eval(bool is_algebra)
{
    int i;
    int count;
    std::string result = "";
    string_expression::string_node *left = static_cast<string_expression::string_node *>(this->left);
    string_expression::string_node *right = static_cast<string_expression::string_node *>(this->right);

    if (!is_algebra) {
        std::string left_eval = left->eval(false);

        count = stoi(right->eval(true));
        for (i = 0; i < count; i++)
            result += left_eval;
    } else {
        result = std::to_string(std::stoi(left->eval(true)) * std::stoi(right->eval(true)));
    }

    return result;
}

std::string string_expression::string_node_value::eval(bool is_algebra)
{
    return this->str;
}

bool string_expression::is_equal()
{
    bool res = true;
    std::string result;
    string_node *node;
    int i = 1;

    if (this->nodes.empty())
        return true;

    node = static_cast<string_node *>(this->nodes.front());
    result = node->eval(false);

    printf("string: Eval 0: \"%s\"\n", result.c_str());

    for (std::list<expression::node *>::iterator intr = ++this->nodes.begin(); intr != this->nodes.end(); ++intr) {
        std::string result2;
        node = static_cast<string_node *>(*intr);

        result2 = node->eval(false);

        printf("string: Eval %d: \"%s\"\n", i, result2.c_str());
        if (result2 != result) {
            res = false;
            break;
        }
    }

    return res;
}

