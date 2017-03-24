#ifndef INCLUDE_PARSER_H
#define INCLUDE_PARSER_H

#include "expression.h"

bool parse_expression_file(const char *file, List<expression> *);

#endif
