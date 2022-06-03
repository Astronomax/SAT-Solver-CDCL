#include "Entities.h"

void Formula::add_clause(const Clause &c) {
    clauses.push_back(c);
}

vector<Clause> &Formula::get_clauses() {
    return clauses;
}

vector<int> Formula::get_literal_nums() {
    set<int> ret;
    for (auto &i: clauses)
        for(auto &l : i.get_literals())
            ret.insert(l.num);
    return {ret.begin(), ret.end()};
}

size_t Formula::compress() {
    vector<int> literalNums = get_literal_nums();

    for (int i = 0; i < literalNums.size(); ++i) {
        coords[literalNums[i]] = i;
        coords_t[i] = literalNums[i];
    }

    vector<Clause> prev_clauses = clauses;
    clauses.clear();

    for (auto &i: prev_clauses) {
        Clause new_clause;
        for (auto j: i.get_literals()) {
            new_clause.add_literal(Literal(coords[j.num], j.value));
        }
        add_clause(new_clause);
    }
    return literalNums.size();
}

Interpretation Formula::getInterpretationByAns(const vector<bool> &ans) {
    Interpretation ret;
    for (int i = 0; i < ans.size(); ++i) {
        ret.push_back(Literal(coords_t[i], ans[i]));
    }
    return ret;
}

void Clause::add_literal(const Literal &l) {
    literals.insert(l);
}

void Clause::remove_literal(const Literal &l) {
    literals.erase(l);
}

const set<Literal>& Clause::get_literals() const {
    return literals;
}

bool Clause::operator==(const Clause &another) {
    return literals == another.literals;
}

bool Clause::contains(const Literal &l) const {
    return literals.find(l) != literals.end();
}

Literal::Literal(int num, bool value) : num(num), value(value) {}

Literal Literal::get_opposite() const {
    return {num, !value};
}

bool Literal::operator<(const Literal &other) const {
    return std::make_pair(num, value) < std::make_pair(other.num, other.value);
}

bool Literal::operator==(const Literal &other) const {
    return std::make_pair(num, value) == std::make_pair(other.num, other.value);
}

bool Literal::operator!=(const Literal &other) const {
    return std::make_pair(num, value) != std::make_pair(other.num, other.value);
}


