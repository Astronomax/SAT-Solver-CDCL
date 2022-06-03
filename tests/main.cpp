#include "CDCL.h"
#include <iostream>
#include <iomanip>
#include "Dimacs_parser.h"

using std::cin;
using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    std::ifstream in(argv[1]);
    Formula formula = dimacs_parser::parse(in);
    CDCL::Solver s;
    cout << (s.solve(formula) ? "SAT" : "UNSAT") << endl;
    //cout << std::fixed << std::setprecision(3) << ((double)clock() / (double)CLOCKS_PER_SEC);
    return 0;
}