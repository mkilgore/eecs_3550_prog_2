
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
    token_lexer::token tok;
    int negative = 0;

  parse_again:
    if (!lexer->get_token(&tok)) {
        printf("Lexer is empty\n");
        return nullptr;
    }

    switch (tok.letter) {
    case '+':
        new_node = new algebra_expression::plus_node();
        break;

    case '-':
        if (this->past_node != expression::node::node_type::NODE_VALUE) {
            negative = 1;
            goto parse_again;
        }
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

        if (negative)
            algebra_node->value = -algebra_node->value;

        new_node = algebra_node;
        break;

    default:
        lexer->push_token(&tok);
        return nullptr;
    }

    this->past_node = new_node->type;
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

int algebra_expression::plus_node::eval()
{
}

int algebra_expression::minus_node::eval()
{
}

int algebra_expression::mult_node::eval()
{
}

int algebra_expression::power_node::eval()
{
}

int algebra_expression::value_node::eval()
{
}

std::string algebra_expression::value_node::to_string()
{
    return std::to_string(this->value);
}

bool algebra_expression::is_equal()
{
    bool res = true;
    int result;
    algebra_node *node;
    int i = 1;

    if (this->nodes.empty())
        return true;

    node = static_cast<algebra_node *>(this->nodes.front());
    result = node->eval();

    printf("algebra: Eval 0: %d\n", result);

    for (std::list<expression::node *>::iterator intr = ++this->nodes.begin(); intr != this->nodes.end(); ++intr) {
        int result2;
        node = static_cast<algebra_node *>(*intr);

        result2 = node->eval();

        printf("algebra: Eval %d: %d\n", i, result2);
        if (result2 != result) {
            res = false;
            break;
        }
    }

    return res;
}

