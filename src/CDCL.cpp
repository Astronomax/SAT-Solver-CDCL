#include "CDCL.h"
#include <queue>

using std::queue;
using namespace CDCL;

const int CDCL::SolverState::U;

void SolverState::make_new_decision() {
    int var = *unassigned.begin();
    set_value(Literal(var, false));
    assignation_order.push_back(var);
    assignation_time[var] = (int)assignation_order.size() - 1;
    ++decision_level;
    level[var] = decision_level;
    level_time[decision_level] = (int) assignation_order.size() - 1;
}

SolverState::SolverState(Formula &f) : decision_level(0) {
    size_t distinct = f.compress();

    values.assign(distinct, U);
    level.assign(distinct, U);

    level_time.assign(distinct, U);
    level_time[decision_level] = -1;

    implications.resize(distinct);
    implications_t.resize(distinct);

    assignation_time.resize(distinct);

    for (auto &clause: f.get_clauses()) {
        add_clause(clause);
    }
}

void SolverState::add_clause(Clause &c) {
    true_literals.push_back(0);
    false_literals.push_back(0);
    clause_unassigned_literals.emplace_back();

    c.normalize();
    for (auto literal: c.get_literals()) {
        literal_clauses[literal].push_back((int) clauses.size());

        if (values[literal.num] == U) {
            unassigned.insert(literal.num);
            clause_unassigned_literals[(int) clauses.size()].insert(literal);
        } else {
            if (values[literal.num] == literal.value) {
                ++true_literals[(int) clauses.size()];
            } else {
                ++false_literals[(int) clauses.size()];
            }
        }
    }

    if (clause_unassigned_literals[(int) clauses.size()].size() == 1)
        units.insert((int) clauses.size());
    clauses.push_back(c);
}

void SolverState::set_value(const Literal &l) {
    unassigned.erase(l.num);
    values[l.num] = l.value;

    for (auto &i: literal_clauses[l]) {
        if (clause_unassigned_literals[i].size() == 1)
            if (!true_literals[i])
                units.erase(i);
        ++true_literals[i];
        clause_unassigned_literals[i].erase(l);
    }

    for (auto &i: literal_clauses[l.get_opposite()]) {
        ++false_literals[i];
        clause_unassigned_literals[i].erase(l.get_opposite());
        if (clause_unassigned_literals[i].empty()) {
            if (!true_literals[i]) {
                units.erase(i);
                conflicts.insert(i);
            }
        } else if (clause_unassigned_literals[i].size() == 1) {
            if (!true_literals[i])
                units.insert(i);
        }
    }
}

void SolverState::reset_value(int var) {
    unassigned.insert(var);
    Literal l = Literal(var, values[var]);
    values[var] = U;
    for (auto &i: literal_clauses[l]) {
        --true_literals[i];
        clause_unassigned_literals[i].insert(l);
        if (clause_unassigned_literals[i].size() == 1)
            if (!true_literals[i])
                units.insert(i);
    }
    for (auto &i: literal_clauses[l.get_opposite()]) {
        if (clause_unassigned_literals[i].empty()) {
            if (!true_literals[i]) {
                units.insert(i);
                conflicts.erase(i);
            }
        } else if (clause_unassigned_literals[i].size() == 1) {
            if (!true_literals[i])
                units.erase(i);
        }
        --false_literals[i];
        clause_unassigned_literals[i].insert(l.get_opposite());
    }
}

void SolverState::back_jump(Clause &c) {
    std::pair<int, int> max_levels = std::make_pair(0, 0);
    for (auto &j: c.get_literals()) {
        int num = j.num;

        if (level[num] < decision_level) {
            if (level[num] > max_levels.second) {
                max_levels.first = max_levels.second;
                max_levels.second = level[num];
            } else if (level[num] > max_levels.first)
                max_levels.first = level[num];
        }
    }

    while ((int) assignation_order.size() - 1 > level_time[max_levels.first]) {
        int current_var = *assignation_order.rbegin();
        level[current_var] = U;
        for (auto &i: implications_t[current_var])
            implications[i].erase(current_var);
        implications_t[current_var].clear();
        reset_value(current_var);
        assignation_order.pop_back();
    }
    decision_level = max_levels.first;
}

Clause SolverState::analyze_conflict(int conflict) {
    set<int> atLastLevel;
    Clause learned = clauses[conflict];
    for(auto &i : learned.get_literals())
        if(level[i.num] == decision_level)
            atLastLevel.insert(assignation_time[i.num]);

    while(atLastLevel.size() > 1) {
        int t = assignation_order[*atLastLevel.rbegin()];
        for(auto lit: implications_t[t]) {
            if(learned.contains(Literal(lit, values[lit]))) {
                if(level[lit] == decision_level)
                    atLastLevel.erase(assignation_time[lit]);
                learned.remove_literal(Literal(lit, values[lit]));
            }
            else {
                if (level[lit] == decision_level)
                    atLastLevel.insert(assignation_time[lit]);
                learned.add_literal(Literal(lit, !values[lit]));
            }
        }
        atLastLevel.erase(assignation_time[t]);
        learned.remove_literal(Literal(t, !values[t]));
    }
    return learned;
}

int SolverState::unit_propagate() {
    while (!units.empty()) {
        int c = *units.begin();
        auto l = *clause_unassigned_literals[c].begin();
        set_value(l);
        assignation_order.push_back(l.num);
        assignation_time[l.num] = (int)assignation_order.size() - 1;
        level[l.num] = decision_level;
        for (auto &i: clauses[c].get_literals()) {
            if (i == l) continue;
            implications[i.num].insert(l.num);
            implications_t[l.num].insert(i.num);
        }
        if (!conflicts.empty())
            return *conflicts.begin();
    }
    return U;
}

int SolverState::all_variables_assigned() const {
    return unassigned.empty();
}

bool Solver::solve(Formula f) {
    SolverState state(f);
    while (!state.all_variables_assigned()) {
        int conflict = state.unit_propagate();
        if (conflict != state.U) {
            if (state.decision_level == 0) {
                return false;
            }
            Clause conflict_clause = state.analyze_conflict(conflict);
            state.back_jump(conflict_clause);
            state.add_clause(conflict_clause);
        } else if (!state.all_variables_assigned()) {
            state.make_new_decision();
        }
    }
    return true;
}
