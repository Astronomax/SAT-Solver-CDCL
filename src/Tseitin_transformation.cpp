#include "Tseitin_transformation.h"
#include "Entities.h"
#include <cassert>
#include <iostream>

int get_or_create(std::unordered_map<Gate*, int> &id_by_gate, Gate* gate) {
    auto it = id_by_gate.find(gate);
    int id;
    if(it != id_by_gate.end()) {
        id = it->second;
    } else {
        id = id_by_gate.size();
        id_by_gate[gate] = id;
    }
    return id;
}

std::vector<Clause> NAND_to_CNF(int in1, int in2, int out) {
    std::vector<Clause> cnf;
    {
        Clause clause;
        clause.add_literal({in1, true});
        clause.add_literal({out, true});
        cnf.push_back(clause);
    }
    {
        Clause clause;
        clause.add_literal({in2, true});
        clause.add_literal({out, true});
        cnf.push_back(clause);
    }
    {
        Clause clause;
        clause.add_literal({in1, false});
        clause.add_literal({in2, false});
        clause.add_literal({out, false});
        cnf.push_back(clause);
    }
    return cnf;
}

int dfs_helper(Gate *gate, Formula &formula, std::unordered_map<Gate*, int> &id_by_gate, int mex_id) {
    int cur_gate_id = get_or_create(id_by_gate, gate);
    std::vector<int> ancestor_ids;
    for(auto &ancestor : gate->links_t)
        ancestor_ids.push_back(get_or_create(id_by_gate, ancestor));
    switch(gate->m_op) {
        case(Op::AND): {
            //std::cout << "AND\n";
            assert(ancestor_ids.size() == 2);
            {
                auto clauses = NAND_to_CNF(ancestor_ids[0], ancestor_ids[1], mex_id);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(mex_id, mex_id, cur_gate_id);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            mex_id += 1;
            break;        
        }
        case(Op::OR): {
            //std::cout << "OR\n";
            assert(ancestor_ids.size() == 2);
            {
                auto clauses = NAND_to_CNF(ancestor_ids[0], ancestor_ids[0], mex_id);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(ancestor_ids[1], ancestor_ids[1], mex_id + 1);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(mex_id, mex_id + 1, cur_gate_id);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            mex_id += 2;
            break;
        }
        case(Op::NOT): {
            //std::cout << "NOT\n";
            assert(ancestor_ids.size() == 1);
            auto clauses = NAND_to_CNF(ancestor_ids[0], ancestor_ids[0], cur_gate_id);
            for(auto &clause : clauses)
                formula.add_clause(clause);
            break;
        }
        case(Op::NAND): {
            //std::cout << "NAND\n";
            assert(ancestor_ids.size() == 2);
            auto clauses = NAND_to_CNF(ancestor_ids[0], ancestor_ids[1], cur_gate_id);
            for(auto &clause : clauses)
                formula.add_clause(clause);
            break;
        }
        case(Op::NOR): {
            //std::cout << "NOR\n";
            assert(ancestor_ids.size() == 2);
            {
                auto clauses = NAND_to_CNF(ancestor_ids[0], ancestor_ids[0], mex_id);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(ancestor_ids[1], ancestor_ids[1], mex_id + 1);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(mex_id, mex_id + 1, mex_id + 2);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(mex_id + 2, mex_id + 2, cur_gate_id);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }            
            mex_id += 3;
            break;
        }
        case(Op::XOR): {
            //std::cout << "XOR\n";
            assert(ancestor_ids.size() == 2);
            {
                auto clauses = NAND_to_CNF(ancestor_ids[0], ancestor_ids[1], mex_id);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(ancestor_ids[0], mex_id, mex_id + 1);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(ancestor_ids[1], mex_id, mex_id + 2);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(mex_id + 1, mex_id + 2, cur_gate_id);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }            
            mex_id += 3;
            break;
        }
        case(Op::NXOR): {
            //std::cout << "NXOR\n";
            assert(ancestor_ids.size() == 2);
            {
                auto clauses = NAND_to_CNF(ancestor_ids[0], ancestor_ids[0], mex_id);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(ancestor_ids[1], ancestor_ids[1], mex_id + 1);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(mex_id, mex_id + 1, mex_id + 2);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(ancestor_ids[0], ancestor_ids[1], mex_id + 3);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            {
                auto clauses = NAND_to_CNF(mex_id + 2, mex_id + 3, cur_gate_id);
                for(auto &clause : clauses)
                    formula.add_clause(clause);
            }
            mex_id += 4;
            break;
        }
        case(Op::INPUT): {
            //std::cout << "INPUT\n";
            assert(ancestor_ids.size() == 0);
            break;
        }
        case(Op::BUFF): {
            //std::cout << "NXOR\n";
            assert(ancestor_ids.size() == 1);
            {
                Clause clause;
                clause.add_literal({ancestor_ids[0], true});
                clause.add_literal({cur_gate_id, false});
                formula.add_clause(clause);
            }
            {
                Clause clause;
                clause.add_literal({ancestor_ids[0], false});
                clause.add_literal({cur_gate_id, true});
                formula.add_clause(clause);
            }
            break;
        }
        default: {
            assert(false);
        }
    }

    for(auto &ancestor : gate->links_t)
        mex_id = dfs_helper(ancestor, formula, id_by_gate, mex_id);
    return mex_id;
}

std::pair<Formula, std::unordered_map<Gate*, int>>
tseitin_transformation::tseitin_transform(const Circuit &circuit)
{
    Formula formula;
    std::unordered_map<Gate*, int> id_by_gate;
    dfs_helper(circuit.output, formula, id_by_gate, circuit.gate_by_name.size());
    {
        Clause clause;
        clause.add_literal({id_by_gate[circuit.output], true});
        formula.add_clause(clause);
    }
    return {formula, id_by_gate};
}