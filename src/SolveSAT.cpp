#include "CDCL.h"
#include <iostream>
#include <string>
#include "Dimacs_parser.h"
#include <chrono>

using std::cin;
using std::cout;
using std::endl;
using std::string;

struct Args {
    string in_file;
    bool print_time;
    bool need_learned;
    bool need_values;

    void parse_args(int argc, char **argv) {
        for (int i = 1; i < argc; ++i) {
            string cur = string(argv[i]);
            if (cur == "-i" || cur == "--input") {
                in_file = string(argv[i + 1]);
            } else if (cur == "-t" || cur == "--time") {
                print_time = true;
            } else if (cur == "-l" || cur == "--learned") {
                need_learned = true;
            } else if (cur == "-v" || cur == "--values") {
                need_values = true;
            }
        }
    }
};

int main(int argc, char *argv[]) {
    Args args;
    args.parse_args(argc, argv);

    if (args.in_file.empty()) {
        cout << "No input file specified\n";
        exit(0);
    }

    std::ifstream in(args.in_file);
    Formula formula = dimacs_parser::parse(in);
    CDCL::Solver s;

    vector<Clause> learned;

    auto start_time = std::chrono::steady_clock::now();
    auto res = s.solve(formula, (args.need_learned ? &learned : nullptr));
    auto end_time = std::chrono::steady_clock::now();

    if (res.empty()) {
        cout << "unsatisfiable.\n" << endl;

    } else {
        cout << "satisfiable.\n" << endl;

        if (args.need_values) {
            cout << "values are:" << endl;
            for (auto &i: res) {
                cout << "x_" << i.num << " = " << i.value << endl;
            }
            cout << endl;
        }
    }

    if (args.need_learned) {
        cout << "learned clauses:" << endl;
        for (auto &clause: learned) {
            for (auto &i: clause.get_literals()) {
                cout << ((i.value == 1) ? i.num : -i.num) << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    if (args.print_time) {
        auto elapsed_ns = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time);
        cout << "done in " << elapsed_ns.count() << " s" << endl;
    }

    return 0;
}