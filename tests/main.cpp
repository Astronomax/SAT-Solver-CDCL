#include "CDCL.h"
#include <iostream>
#include "Dimacs_parser.h"

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    std::ifstream in(argv[1]);
    Formula formula = dimacs_parser::parse(in);
    CDCL::Solver s;
    cout << (s.solve(formula) ? "SAT" : "UNSAT") << endl;
    return 0;
}