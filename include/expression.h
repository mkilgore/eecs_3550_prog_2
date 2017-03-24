#ifndef INCLUDE_EXPRESSION_H
#define INCLUDE_EXPRESSION_H

#include <string>
#include <list>

class expression {
     virtual std::string to_string();
     virtual bool is_equal();
};

class string_expresion : expression {
     class node {
          std::string get_value();
     };
  
     class plus_node : node {
           std::string left;
           std::string right;
     };
  
     class mult_node : node {
            std::string left;
            int count;
     };
  
     List<node> nodes;
};

class bool_expression : expression {
     class node {
     };
     List<node> nodes;
};

class algebra_expression : expression {
  
};

class set_expression : expression {
  
};

#endif
