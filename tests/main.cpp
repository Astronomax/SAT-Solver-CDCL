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

    auto res = s.solve(formula, nullptr);

    if (res.empty()) {
        cout << "UNSAT";
    } else {
        cout << "SAT";
    };
    cout << endl;
    return 0;
}