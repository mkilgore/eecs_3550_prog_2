
#include <stdio.h>

#include "expression.hpp"
#include "parser.hpp"
#include "eval.hpp"

int main(int argc, char **argv)
{
    const char *file = "test.txt";
    std::list<expression *> expression_list;

    if (!parse_expression_file(file, &expression_list))
        return 1;

    if (!eval_expression_list(&expression_list))
        return 1;

    int s = expression_list.size();
    int i;

    for (i = 0; i < s; i++) {
        expression *n = expression_list.front();
        expression_list.pop_front();

        delete n;
    }

    return 0;
}

