#ifndef INCLUDE_LEXER_HPP
#define INCLUDE_LEXER_HPP

#include <iostream>
#include <list>

#include "char_set.hpp"

class token_lexer {
  public:
    struct token {
        char letter;
    };

  private:
    std::istream *input_stream;
    std::list<token> token_stack;

  public:
    void get_string(std::string *, char_set *);
    bool get_token(token *);
    void push_token(token *);

    token_lexer(std::istream *stream) : token_stack() {
        input_stream = stream;
    };
};

#endif
