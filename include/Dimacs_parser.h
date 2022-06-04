#ifndef CDCL_SAT_SOLVER_DIMACS_PARSER_H
#define CDCL_SAT_SOLVER_DIMACS_PARSER_H

#include "CDCL.h"
#include <fstream>

namespace dimacs_parser {
    Formula parse(std::ifstream &input_file);
}

#endif //CDCL_SAT_SOLVER_DIMACS_PARSER_H
