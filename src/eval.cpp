
#include "eval.hpp"

bool eval_expression_list(std::list<expression *> *list)
{
    int i = 1;
    for (std::list<expression *>::iterator iter = list->begin(); iter != list->end(); ++iter, i++) {
        printf("Expresison %d: %s\n", i, (*iter)->is_equal()? "true": "false");
    }
    return true;
}

