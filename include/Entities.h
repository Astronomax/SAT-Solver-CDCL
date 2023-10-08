#ifndef CDCL_SAT_SOLVER_ENTITIES_H
#define CDCL_SAT_SOLVER_ENTITIES_H

#include <vector>
#include <string>
#include <map>
#include <set>
#include <cstddef>
#include <unordered_map>

using std::map;
using std::set;
using std::vector;

struct Literal {
    int num;
    bool value;

    Literal(int num, bool value);

    Literal get_opposite() const;

    bool operator<(const Literal &other) const;

    bool operator==(const Literal &other) const;

    bool operator!=(const Literal &other) const;
};

struct Clause {
    void add_literal(const Literal &l);

    const set<struct Literal> &get_literals() const;

    bool operator==(const Clause &another);

    bool is_trivial() const;

    bool contains(const Literal &l) const;

    void remove_literal(const Literal &l);

private:
    set<Literal> literals;
};

typedef vector<Literal> Interpretation;

struct Formula {
    void add_clause(const Clause &c);

    vector<Clause> &get_clauses();

    vector<int> get_literal_nums();

    Interpretation getInterpretationByAns(const vector<int> &ans, const std::unordered_map<int, int> &coords_t);

    std::unordered_map<int, int> compress();
private:
    vector<Clause> clauses;
};





enum Op {
    AND,
    OR,
    NOT,
    NAND,
    NOR,
    XOR,
    NXOR,
    INPUT
};

struct Gate {
    std::vector<Gate*> links;
    std::vector<Gate*> links_t;
    Op m_op;
};

struct Circuit {
    std::vector<Gate*> inputs;
    std::unordered_map<std::string, Gate*> gate_by_name;
    Gate *output;
};





#endif //CDCL_SAT_SOLVER_ENTITIES_H
