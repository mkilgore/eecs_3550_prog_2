
#include <stack>
#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>

#include "parser.hpp"
#include "strings.hpp"
#include "bool.hpp"
#include "set.hpp"
#include "algebra.hpp"

class alnum_char_set : public char_set {
  public:
    alnum_char_set() {
        add_range('a', 'z');
        add_range('A', 'Z');
        add_range('0', '9');
    }
};

class alpha_char_set : public char_set {
  public:
    alpha_char_set() {
        add_range('a', 'z');
        add_range('A', 'Z');
    }
};

static alnum_char_set alnum_set;
static alpha_char_set alpha_set;

/*
 * Implements the shunting-yard algorithm for parsing infix expressions.
 */
bool parse_infix_expression(expression::node **root, token_generator *gen_token)
{
    std::stack<expression::node *> op_stack;
    std::stack<expression::node *> value_stack;
    expression::node *tmp;

    while ((tmp = gen_token->get_next_token()) != nullptr) {

        switch (tmp->type) {
        case expression::node::NODE_OP:
            if (op_stack.empty()) {
                op_stack.push(tmp);
                break;
            } else {
                expression::node *op, *op2;
                op = tmp;

                while (!op_stack.empty()
                  && (op2 = op_stack.top())
                  && op2->prec >= op->prec) {
                    op_stack.pop();

                    if (op2->is_binary) {
                        op2->right = value_stack.top();
                        value_stack.pop();
                    }

                    op2->left = value_stack.top();
                    value_stack.pop();

                    value_stack.push(dynamic_cast<expression::node *>(op2));
                }

                op_stack.push(op);
            }
            break;

        case expression::node::NODE_PAREN_OPEN:
            op_stack.push(tmp);
            break;

        case expression::node::NODE_PAREN_CLOSE:
            while (op_stack.top()->type != expression::node::NODE_PAREN_OPEN) {
                expression::node *op = op_stack.top();

                op_stack.pop();

                if (op->is_binary) {
                    op->right = value_stack.top();
                    value_stack.pop();
                }

                op->left = value_stack.top();
                value_stack.pop();

                value_stack.push(dynamic_cast<expression::node *>(op));
            }

            delete tmp;
            delete op_stack.top();
            op_stack.pop();
            break;

        case expression::node::NODE_VALUE:
            value_stack.push(tmp);
            break;
        }
    }


    while (!op_stack.empty()) {
        expression::node *op = op_stack.top();
        op_stack.pop();

        if (op->is_binary) {
            op->right = value_stack.top();
            value_stack.pop();
        }

        op->left = value_stack.top();
        value_stack.pop();

        value_stack.push(dynamic_cast<expression::node *>(op));
    }

    *root = value_stack.top();

    return true;
}

enum expr_type {
    EXPR_NONE,
    EXPR_BOOL,
    EXPR_STRING,
    EXPR_SET,
    EXPR_ALGEBRA,
};

static bool parse_expression(token_lexer *lexer, std::list<expression *> *list, expr_type type)
{
    expression *expr = nullptr;
    expression::node *new_node = nullptr;
    token_lexer::token tok;
    bool ret = true;

    lexer->get_token(&tok);

    if (tok.letter != '<') {
        lexer->push_token(&tok);
        tok.letter = ';';
    }

    do {
        if (tok.letter == ';' || tok.letter == '<') {
            if (expr != nullptr)
                list->push_back(expr);

            switch (type) {
            case EXPR_BOOL:
                expr = new bool_expression();
                break;

            case EXPR_STRING:
                expr = new string_expression();
                break;

            case EXPR_SET:
                expr = new set_expression();
                break;

            case EXPR_ALGEBRA:
                expr = new algebra_expression();
                break;
            }
        }

        if (tok.letter == '<') {
            std::string id = "";

            lexer->get_token(&tok);

            if (tok.letter == '/') {
                id = "/";
            } else {
                id += tok.letter;
                lexer->get_string(&id, &alpha_set);
            }

            lexer->get_token(&tok);
            if (tok.letter != '>') {
                printf("Unexpected token: \"%c\"\n", tok.letter);
                ret = false;
                break;
            }

            if (strcasecmp(id.c_str(), "/") == 0) {
                printf("End context\n");
                break;

            } else if (strcasecmp(id.c_str(), "boolean") == 0) {
                printf("Boolean context\n");
                if (!parse_expression(lexer, list, EXPR_BOOL)) {
                    ret = false;
                    break;
                }
            } else if (strcasecmp(id.c_str(), "strings") == 0) {
                printf("String context\n");
                if (!parse_expression(lexer, list, EXPR_STRING)) {
                    ret = false;
                    break;
                }
            } else if (strcasecmp(id.c_str(), "sets") == 0) {
                printf("Set context\n");
                if (!parse_expression(lexer, list, EXPR_SET)) {
                    ret = false;
                    break;
                }
            } else if (strcasecmp(id.c_str(), "algebra") == 0) {
                printf("Algebra context\n");
                if (!parse_expression(lexer, list, EXPR_ALGEBRA)) {
                    ret = false;
                    break;
                }
            } else {
                printf("Unknown expression type: \"%s\"\n", id.c_str());
                ret = false;
                break;
            }

            lexer->get_token(&tok);
            if (tok.letter != '<') {
                lexer->push_token(&tok);
                tok.letter = ';';
            }
            continue;
        } else {
            switch (type) {
            case EXPR_BOOL: {
                bool_token_generator bool_toks(lexer);
                parse_infix_expression(&new_node, &bool_toks);
                }
                break;

            case EXPR_STRING: {
                string_token_generator str_toks(lexer);
                parse_infix_expression(&new_node, &str_toks);
                }
                break;

            case EXPR_SET: {
                set_token_generator set_toks(lexer);
                parse_infix_expression(&new_node, &set_toks);
                }
                break;

            case EXPR_ALGEBRA: {
                algebra_token_generator algebra_toks(lexer);
                parse_infix_expression(&new_node, &algebra_toks);
                }
                break;
            }

            if (new_node) {
                printf("New Expr, to_string: \"%s\"\n", new_node->to_string().c_str());
                expr->nodes.push_back(new_node);
                new_node = nullptr;
            }
        }

        lexer->get_token(&tok);
    } while (tok.letter == '=' || tok.letter == ';' || tok.letter == '<');

    if (expr) {
        if (expr->nodes.empty())
            delete expr;
        else
            list->push_back(expr);
    }

    return ret;
}

bool parse_expression_file(const char *file, std::list<expression *> *list)
{
    std::fstream f;
    f.open(file, std::fstream::in);
    token_lexer lexer(&f);
    bool ret;

    ret = parse_expression(&lexer, list, EXPR_NONE);

    f.close();
    return ret;
}

