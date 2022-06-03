#include "CDCL.h"

using namespace CDCL;

void SolverState::make_new_decision() {
    int var = *unassigned.begin();
    set_value(Literal(var, false));
    assignation_order.push_back(var);
    assignation_time[var] = (int)assignation_order.size() - 1;
    ++decision_level;
    level[var] = decision_level;
    level_time[decision_level] = (int) assignation_order.size() - 1;
}

SolverState::SolverState(Formula f) : decision_level(0) {
    size_t distinct = f.compress();

    values.assign(distinct, -1);
    level.assign(distinct, -1);

    level_time.assign(distinct, -1);
    level_time[decision_level] = -1;

    implications_t.resize(distinct);

    assignation_time.resize(distinct);
    literal_clauses[0].resize(distinct);
    literal_clauses[1].resize(distinct);

    vector<int> cnt[2];
    cnt[0].resize(distinct);
    cnt[1].resize(distinct);

    vector<Clause> non_trivial;
    for (auto &clause: f.get_clauses()) {
        bool trivial = false;
        for(auto &l : clause.get_literals())
            if(clause.contains(l.get_opposite()))
                trivial = true;
        if(!trivial) non_trivial.push_back(clause);
    }
    for(auto &clause : non_trivial)
        for(auto &l : clause.get_literals())
            cnt[(int)l.value][l.num]++;
    for(int i = 0; i < distinct; i++) {
        if (!cnt[0][i] && cnt[1][i])
            values[i] = 1;
        else if (!cnt[1][i] && cnt[0][i])
            values[i] = 0;
    }
    for(auto &clause: non_trivial) {
        bool easy_to_sat = false;
        for(auto &l : clause.get_literals())
            if(!cnt[0][l.num] || !cnt[1][l.num])
                easy_to_sat = true;
        if(!easy_to_sat) add_clause(clause);
    }
}

void SolverState::add_clause(const Clause &c) {
    true_literals.push_back(0);
    false_literals.push_back(0);
    clause_unassigned_literals.emplace_back();

    for (auto l: c.get_literals()) {
        clauses_with_literal(l).push_back((int) clauses.size());

        if (values[l.num] == -1) {
            unassigned.insert(l.num);
            clause_unassigned_literals.back().insert(l);
        } else {
            if (values[l.num] == l.value) {
                ++true_literals.back();
            } else {
                ++false_literals.back();
            }
        }
    }

    if (clause_unassigned_literals.back().size() == 1)
        units.insert((int) clauses.size());
    clauses.push_back(c);
}

void SolverState::set_value(const Literal &l) {
    unassigned.erase(l.num);
    values[l.num] = l.value;

    for (auto &i: clauses_with_literal(l)) {
        if (clause_unassigned_literals[i].size() == 1)
            if (!true_literals[i])
                units.erase(i);
        ++true_literals[i];
        clause_unassigned_literals[i].erase(l);
    }

    for (auto &i: clauses_with_literal(l.get_opposite())) {
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
    values[var] = -1;
    for (auto &i: clauses_with_literal(l)) {
        --true_literals[i];
        clause_unassigned_literals[i].insert(l);
        if (clause_unassigned_literals[i].size() == 1)
            if (!true_literals[i])
                units.insert(i);
    }
    for (auto &i: clauses_with_literal(l.get_opposite())) {
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
    int jump = 0;
    for (auto &j: c.get_literals())
        if(level[j.num] < decision_level)
            jump = std::max(jump, level[j.num]);

    while ((int) assignation_order.size() - 1 > level_time[jump]) {
        int current_var = assignation_order.back();
        level[current_var] = -1;
        implications_t[current_var].clear();
        reset_value(current_var);
        assignation_order.pop_back();
    }
    decision_level = jump;
}

Clause SolverState::analyze_conflict(int conflict) const {
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
            implications_t[l.num].insert(i.num);
        }
        if (!conflicts.empty())
            return *conflicts.begin();
    }
    return -1;
}

bool SolverState::all_variables_assigned() const {
    return unassigned.empty();
}

vector<int> &SolverState::clauses_with_literal(const Literal &l) {
    return literal_clauses[(int)l.value][l.num];
}

bool Solver::solve(const Formula &f) {
    SolverState state(f);
    while (!state.all_variables_assigned()) {
        int conflict = state.unit_propagate();
        if (conflict != -1) {
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
