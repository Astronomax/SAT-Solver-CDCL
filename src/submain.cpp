#include "CDCL.h"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main() {
    freopen("input.txt", "r", stdin);
    int n;
    cin >> n;
    Formula formula;
    while (n--) {
        Clause clause;
        int k;
        while (true) {
            cin >> k;
            if (!k) break;
            clause.add_literal(Literal(abs(k) - 1, k > 0));
        }
        formula.add_clause(clause);
    }
    CDCL::Solver s;
    cout << (s.solve(formula) ? "SAT" : "UNSAT") << endl;
    return 0;
}