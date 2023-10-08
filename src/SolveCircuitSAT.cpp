#include "CDCL.h"
#include <iostream>
#include <string>
#include "Bench_parser.h"
#include "Tseitin_transformation.h"
#include "CDCL.h"
#include <chrono>

using std::cin;
using std::cout;
using std::endl;
using std::string;

struct Args {
    string in_file = "";
    bool print_time = false;
    bool need_learned = false;
    bool need_values = false;

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
    Circuit circuit = bench_parser::parse(in);
    std::unordered_map<Gate*, std::string> name_by_gate;
    for(auto &p : circuit.gate_by_name)
        name_by_gate[p.second] = p.first;    
    auto p = tseitin_transformation::tseitin_transform(circuit);
    Formula &formula = p.first;
    std::unordered_map<Gate*, int> &id_by_gate = p.second;
    std::unordered_map<int, Gate*> gate_by_id;
    for(auto &p : id_by_gate)
        gate_by_id[p.second] = p.first;

    /*
    std::cout << formula.get_clauses().size() << "\n";
    auto clauses = formula.get_clauses();
    for(int i = 0; i < clauses.size(); i++) {
        Clause &clause = clauses[i];
        std::vector<Literal> literals;
        for(auto &literal : clause.get_literals())
            literals.push_back(literal);
        std::cout << "(";
        for(int j = 0; j < literals.size(); j++) {
            Literal &literal = literals[j];
            if(!literal.value)
                std::cout << "~";
            std::cout << literal.num;
            if(j < literals.size() - 1)
                std::cout << "\\/";
        }
        std::cout << ")";
        if(i < clauses.size() - 1)
            std::cout << "/\\";
    }
    std::cout << "\n";
    */

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
                auto it = gate_by_id.find(i.num);
                if(it != gate_by_id.end() && it->second->m_op == Op::INPUT) {
                    cout << name_by_gate[it->second] << " = " << i.value << endl;
                }
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