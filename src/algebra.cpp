
#include <cstdlib>
#include "expression.hpp"
#include "parser.hpp"
#include "algebra.hpp"

class digit_char_set : public char_set {
  public:
    digit_char_set() {
        add_range('a', 'z');
        add_range('A', 'Z');
        add_range('0', '9');
    }
};

static const digit_char_set digit_set;

expression::node *algebra_token_generator::get_next_token()
{
    int i;
    expression::node *new_node = nullptr;
    algebra_expression::value_node *algebra_node = nullptr;
    algebra_expression::variable_node *var_node = nullptr;
    token_lexer::token tok;

    if (!lexer->get_token(&tok)) {
        printf("Lexer is empty\n");
        return nullptr;
    }

    switch (tok.letter) {
    case '+':
        new_node = new algebra_expression::plus_node();
        break;

    case '-':
        new_node = new algebra_expression::minus_node();
        break;

    case '*':
        new_node = new algebra_expression::mult_node();
        break;

    case '^':
        new_node = new algebra_expression::power_node();
        break;

    case '(':
        new_node = new expression::node_paren_open();
        break;

    case ')':
        new_node = new expression::node_paren_close();
        break;

    case '0' ... '9':
        algebra_node = new algebra_expression::value_node();
        algebra_node->value = tok.letter - '0';
        new_node = algebra_node;
        break;

    case 'a' ... 'z':
    case 'A' ... 'Z':
        var_node = new algebra_expression::variable_node();
        var_node->var = tok.letter;

        new_node = var_node;
        break;

    default:
        lexer->push_token(&tok);
        return nullptr;
    }

    printf("TOKEN: \"%s\"\n", new_node->to_string().c_str());
    return new_node;
}

std::string algebra_expression::plus_node::to_string()
{
    if (this->left && this->right)
        return "(" + this->left->to_string() + " + " + this->right->to_string() + ")";
    else
        return "+";
}

std::string algebra_expression::minus_node::to_string()
{
    if (this->left && this->right)
        return "(" + this->left->to_string() + " - " + this->right->to_string() + ")";
    else
        return "-";
}

std::string algebra_expression::mult_node::to_string()
{
    if (this->left && this->right)
        return "(" + this->left->to_string() + " * " + this->right->to_string() + ")";
    else
        return "*";
}

std::string algebra_expression::power_node::to_string()
{
    if (this->left && this->right)
        return "(" + this->left->to_string() + " ^ " + this->right->to_string() + ")";
    else
        return "^";
}

bool algebra_expression::plus_node::is_value()
{
    algebra_expression::algebra_node *left  = static_cast<algebra_expression::algebra_node *>(this->left);
    algebra_expression::algebra_node *right = static_cast<algebra_expression::algebra_node *>(this->right);

    return left->is_value() && right->is_value();
}

bool algebra_expression::minus_node::is_value()
{
    algebra_expression::algebra_node *left  = static_cast<algebra_expression::algebra_node *>(this->left);
    algebra_expression::algebra_node *right = static_cast<algebra_expression::algebra_node *>(this->right);

    return left->is_value() && right->is_value();
}

bool algebra_expression::mult_node::is_value()
{
    algebra_expression::algebra_node *left  = static_cast<algebra_expression::algebra_node *>(this->left);
    algebra_expression::algebra_node *right = static_cast<algebra_expression::algebra_node *>(this->right);

    return left->is_value() && right->is_value();
}

bool algebra_expression::power_node::is_value()
{
    algebra_expression::algebra_node *left  = static_cast<algebra_expression::algebra_node *>(this->left);
    algebra_expression::algebra_node *right = static_cast<algebra_expression::algebra_node *>(this->right);

    return left->is_value() && right->is_value();
}

std::string algebra_expression::value_node::to_string()
{
    return std::to_string(this->value);
}

std::string algebra_expression::variable_node::to_string()
{
    return std::string(1, this->var);
}

bool algebra_expression::is_equal()
{
    return true;
}

