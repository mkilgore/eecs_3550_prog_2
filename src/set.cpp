
#include <cstdlib>
#include "expression.hpp"
#include "parser.hpp"
#include "set.hpp"

class alnum_char_set : public char_set {
  public:
    alnum_char_set() {
        add_range('a', 'z');
        add_range('A', 'Z');
        add_range('0', '9');
    }
};

static alnum_char_set alnum_set;

expression::node *set_token_generator::get_next_token()
{
    expression::node *new_node = nullptr;
    set_expression::set_value_node *set_node = nullptr;
    token_lexer::token tok;

    if (!lexer->get_token(&tok)) {
        printf("Lexer is empty\n");
        return nullptr;
    }

    printf("TOKEN: %c\n", tok.letter);

    switch (tok.letter) {
    case '+':
        new_node = new set_expression::union_node();
        break;

    case '*':
        new_node = new set_expression::intersect_node();
        break;

    case '(':
        new_node = new expression::node_paren_open();
        break;

    case ')':
        new_node = new expression::node_paren_close();
        break;

    case '{':
        set_node = new set_expression::set_value_node();
        printf("Parsing set\n");

        do {
            lexer->get_token(&tok);

            set_node->set.add(tok.letter);

            lexer->get_token(&tok);
        } while (tok.letter == ',');

        if (tok.letter == '}') {
            new_node = set_node;
            break;
        }

        printf("Unexpected token: \"%c\"\n", tok.letter);
        delete set_node;
        break;

    default:
        lexer->push_token(&tok);
        return nullptr;
    }

    return new_node;
}

std::string set_expression::intersect_node::to_string()
{
    if (this->left && this->right)
        return "(" + this->left->to_string() + " * " + this->right->to_string() + ")";
    else
        return "*";
}

std::string set_expression::union_node::to_string()
{
    if (this->left && this->right)
        return "(" + this->left->to_string() + " + " + this->right->to_string() + ")";
    else
        return "+";
}

std::string set_expression::set_value_node::to_string()
{
    return this->set.to_string();
}

char_set set_expression::intersect_node::eval()
{
    set_expression::set_node *left = static_cast<set_expression::set_node *>(this->left);
    set_expression::set_node *right = static_cast<set_expression::set_node *>(this->right);
    char_set left_set = left->eval();
    char_set right_set = right->eval();

    left_set.set_intersect(&right_set);

    return left_set;
}

char_set set_expression::union_node::eval()
{
    set_expression::set_node *left = static_cast<set_expression::set_node *>(this->left);
    set_expression::set_node *right = static_cast<set_expression::set_node *>(this->right);
    char_set left_set = left->eval();
    char_set right_set = right->eval();

    left_set.set_union(&right_set);

    return left_set;
}

char_set set_expression::set_value_node::eval()
{
    return this->set;
}

bool set_expression::is_equal()
{
    bool res = true;
    char_set result;
    set_node *node;
    int i = 1;

    if (this->nodes.empty())
        return true;

    node = static_cast<set_node *>(this->nodes.front());
    result = node->eval();

    printf("sets: Eval 0: %s\n", result.to_string().c_str());

    for (std::list<expression::node *>::iterator intr = ++this->nodes.begin(); intr != this->nodes.end(); ++intr) {
        char_set result2;
        node = static_cast<set_node *>(*intr);

        result2 = node->eval();

        printf("sets: Eval %d: %s\n", i, result2.to_string().c_str());
        if (result2 != result) {
            res = false;
            break;
        }
    }

    return res;
}


