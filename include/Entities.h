#ifndef CDCL_SAT_SOLVER_ENTITIES_H
#define CDCL_SAT_SOLVER_ENTITIES_H

#include <vector>
#include <map>

using std::map;
using std::vector;

struct Literal {
    int num;
    bool value;

    Literal(int num, bool value);

    Literal get_opposite() const;

    int id() const;

    bool operator<(const Literal &other) const;

    bool operator==(const Literal &other) const;

    bool operator!=(const Literal &other) const;
};

struct Clause {
    void add_literal(const Literal &l);

    vector<Literal> &get_literals();

    void normalize();

private:
    vector<Literal> literals;
};

typedef vector<Literal> Interpretation;

struct Formula {
    void add_clause(const Clause &c);

    vector<Clause> &get_clauses();

    Interpretation getInterpretationByAns(const vector<bool> &ans);

    size_t compress(); // return count distinct variables
private:
    vector<Clause> clauses;

    map<int, int> coords, coords_t; // invariant: coords_t[coords[a]] = a
};

#endif //CDCL_SAT_SOLVER_ENTITIES_H
