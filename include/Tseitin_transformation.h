#ifndef CDCL_SAT_SOLVER_TSEITIN_TRANSFORMATION_H
#define CDCL_SAT_SOLVER_TSEITIN_TRANSFORMATION_H

#include "Entities.h"
#include <fstream>
#include <unordered_map>

namespace tseitin_transformation {
    std::pair<Formula, std::unordered_map<Gate*, int>> tseitin_transform(const Circuit &circuit);
}

#endif //CDCL_SAT_SOLVER_TSEITIN_TRANSFORMATION_H
