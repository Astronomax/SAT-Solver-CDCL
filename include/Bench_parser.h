#ifndef CDCL_SAT_SOLVER_BENCH_PARSER_H
#define CDCL_SAT_SOLVER_BENCH_PARSER_H

#include "CDCL.h"
#include <fstream>

namespace bench_parser {
    Circuit parse(std::ifstream &input_file);
}

#endif //CDCL_SAT_SOLVER_BENCH_PARSER_H