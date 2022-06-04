#ifndef CDCL_SAT_SOLVER_DIMACS_PARSER_H
#define CDCL_SAT_SOLVER_DIMACS_PARSER_H

#include "CDCL.h"

#include <fstream>

namespace DimacsParser {
    Formula parse_dimacs_file(std::ifstream &input_file);
}

#endif //CDCL_SAT_SOLVER_DIMACS_PARSER_H
