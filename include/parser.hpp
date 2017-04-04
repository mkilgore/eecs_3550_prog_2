#ifndef INCLUDE_PARSER_HPP
#define INCLUDE_PARSER_HPP

#include <list>

#include "expression.hpp"
#include "lexer.hpp"

class token_generator {
  protected:
      token_lexer *lexer;

  public:
    virtual void start_new_expression() { };
    virtual expression::node *get_next_token();
    token_generator(token_lexer *l) : lexer(l) { };
};

bool parse_expression_file(const char *file, std::list<expression *> *list);

#endif
