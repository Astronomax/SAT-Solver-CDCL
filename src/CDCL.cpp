//
// Created by arrias on 28.05.22.
//
#include "CDCL.h"
#include <algorithm>
#include <queue>

using std::queue;
using namespace CDCL;

double time_::get_time(clock_t start) {
    return (double)(clock() - start) / CLOCKS_PER_SEC;
}

const int CDCL::SolverState::U;

void SolverState::make_new_decision() {
    int var = *unassigned.begin();
    set_value(Literal(var, false));
    assignation_order.push_back(var);
    ++decision_level;
    level[var] = decision_level;
    level_time[decision_level] = (int) assignation_order.size() - 1;
}

SolverState::SolverState(Formula &f) : decision_level(0) {
    int distinct = f.compress();

    values.assign(distinct, U);
    level.assign(distinct, U);

    level_time.assign(distinct, U);
    level_time[decision_level] = -1;

    implications.resize(distinct);
    implications_t.resize(distinct);

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
    Clause res;
    for (auto &i: clauses[conflict].get_literals())
        implications[i.num].insert(U); //edges to bottom

    vector<bool> is_pred(values.size());
    for (int i = (int) assignation_order.size() - 1; i >= level_time[decision_level]; i--) {
        int current_var = assignation_order[i];
        for (auto &j: implications[current_var])
            if (j == -1 || is_pred[j])
                is_pred[current_var] = true;
    }

    int last_decision = assignation_order[level_time[decision_level]];
    int UIP = last_decision;
    for (int i = level_time[decision_level]; i < (int) assignation_order.size(); i++) {
        int current_var = assignation_order[i];
        if (is_pred[current_var]) {
            bool ok = true;
            set<int> used;
            queue<int> que;
            que.push(last_decision);
            while (!que.empty()) {
                int v = que.front();
                que.pop();
                if (used.find(v) != used.end())
                    continue;
                used.insert(v);
                if (v == current_var) continue;
                else if (v == -1) ok = false;
                if (v != -1)
                    for (auto &j: implications[v])
                        que.push(j);
            }
            if (ok) UIP = current_var;
        }
    }

    set<int> B;
    queue<int> que;
    for (auto &j: implications[UIP])
        que.push(j);
    while (!que.empty()) {
        int v = que.front();
        que.pop();
        if (B.find(v) != B.end())
            continue;
        B.insert(v);
        if (v != -1)
            for (auto &j: implications[v])
                que.push(j);
    }

    for (int v: assignation_order) {
        if (B.find(v) == B.end()) {
            for (auto &j: implications[v]) {
                if (B.find(j) != B.end()) {
                    res.add_literal(Literal(v, 1 ^ values[v]));
                    break;
                }
            }
        }
    }

    for (auto &i: clauses[conflict].get_literals())
        implications[i.num].erase(U); //edges to bottom
    return res;
}

int SolverState::unit_propagate() {
    while (!units.empty()) {
        int c = *units.begin();
        auto l = *clause_unassigned_literals[c].begin();
        set_value(l);
        assignation_order.push_back(l.num);
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
    clock_t start = clock();

    SolverState state(f);

    while (!state.all_variables_assigned()) {
        if(time_::get_time(start)>60)
            break;

        int conflict = state.unit_propagate();
        if (conflict != state.U) {
            if (state.decision_level == 0) {
                return false;
            }
            auto conflict_clause = state.analyze_conflict(conflict);
            state.back_jump(conflict_clause);
            state.add_clause(conflict_clause);
        } else if (!state.all_variables_assigned()) {
            state.make_new_decision();
        }
    }

    return true;
}
