
#include <stack>
#include <string>
#include <iostream>

#include "lexer.hpp"

void token_lexer::get_string(std::string *str, char_set *set)
{
    /*
     *
     * FIXME: Check token stack first
     *
     */
    char next;

    while (next = this->input_stream->get(),
           !this->input_stream->eof() && (next == '\t' || next == ' ' || next == '\n'))
        ;

    do {
        if (set->has(next)) {
            *str += next;
        } else {
            token tok = { .letter = next };
            this->token_stack.push_front(tok);
            return ;
        }
    } while (next = this->input_stream->get(),
             !this->input_stream->eof() && next != '\t' && next != ' ' && next != '\n');
}

bool token_lexer::get_token(token *tok)
{
    if (!this->token_stack.empty()) {
        *tok = this->token_stack.front();
        this->token_stack.pop_front();
        return true;
    }

    if (this->input_stream->eof())
        return false;

    char next;
    while (next = this->input_stream->get(),
           !this->input_stream->eof() && (next == '\t' || next == ' ' || next == '\n'))
        ;

    if (this->input_stream->eof())
        return false;

    tok->letter = next;

    return true;
}

void token_lexer::push_token(token *tok)
{
    this->token_stack.push_front(*tok);
}

