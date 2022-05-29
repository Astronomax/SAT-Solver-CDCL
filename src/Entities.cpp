#include "Entities.h"
#include <algorithm>

void Formula::add_clause(const Clause &c) {
    clauses.push_back(c);
}

vector<Clause> &Formula::get_clauses() {
    return clauses;
}

vector<int> get_unique_literal_nums(Formula &f) {
    vector<int> ret;
    for (auto &i: f.get_clauses()) {
        auto &it = i.get_literals();
        std::transform(it.begin(), it.end(), std::back_inserter(ret), [ ](Literal &l) { return l.num; });
    }
    std::sort(ret.begin(), ret.end());
    ret.erase(std::unique(ret.begin(), ret.end()), ret.end());
    return ret;
}

size_t Formula::compress() {
    auto literalNums = get_unique_literal_nums(*this);

    for (int i = 0; i < literalNums.size(); ++i) {
        coords[literalNums[i]] = i;
        coords_t[i] = literalNums[i];
    }

    auto old_clauses = clauses;
    clauses.clear();

    for (auto &i: old_clauses) {
        Clause newClause;
        for (auto j: i.get_literals()) {
            newClause.add_literal(Literal(coords[j.num], j.value));
        }
        add_clause(newClause);
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

void Clause::normalize() {
    std::sort(literals.begin(), literals.end());
    literals.erase(std::unique(literals.begin(), literals.end()), literals.end());
}

void Clause::add_literal(const Literal &l) {
    literals.push_back(l);
}

vector<Literal> &Clause::get_literals() {
    return literals;
}

Literal::Literal(int num, bool value) : num(num), value(value) {}

Literal Literal::get_opposite() const {
    return Literal(num, !value);
}

bool Literal::operator<(const Literal &other) const {
    if (num != other.num) {
        return num < other.num;
    }
    return value < other.value;
}

bool Literal::operator==(const Literal &other) const {
    return !(*this < other) && !(other < *this);
}

bool Literal::operator!=(const Literal &other) const {
    return !(*this == other);
}

int Literal::id() const {
    return (num << 1) + value;
}


