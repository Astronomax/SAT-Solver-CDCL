#include "Entities.h"
#include <algorithm>

void Formula::add_clause(const Clause &c) {
    clauses.push_back(c);
}

vector<Clause> &Formula::get_clauses() {
    return clauses;
}

vector<int> Formula::get_literal_nums() {
    vector<int> ret;
    for (auto &i: clauses) {
        auto it = i.get_literals();
        vector<int> literal_nums;
        literal_nums.reserve(it.size());
        for(auto &l : it)
            literal_nums.push_back(l.num);
        //std::transform(it.begin(), it.end(), back_inserter(literal_nums), [ ](Literal &l) { return l.num; });
        ret.insert(ret.end(), literal_nums.begin(), literal_nums.end());
    }
    return ret;
}

size_t Formula::compress() {
    auto literalNums = get_literal_nums();
    std::sort(literalNums.begin(), literalNums.end());
    literalNums.erase(std::unique(literalNums.begin(), literalNums.end()), literalNums.end());

    for (int i = 0; i < literalNums.size(); ++i) {
        coords[literalNums[i]] = i;
        coords_t[i] = literalNums[i];
    }

    auto literals = clauses;
    clauses.clear();

    for (auto &i: literals) {
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


