#ifndef CDCL_SAT_SOLVER_CDCL_SOLVER_H
#define CDCL_SAT_SOLVER_CDCL_SOLVER_H

#include "Entities.h"
#include <set>

using std::set;
using std::pair;

namespace CDCL {
    struct SolverState {
        explicit SolverState(Formula f);

        void make_new_decision();

        void update_trivial(int var);

        void update_cnt(const Literal &l, int d);

        void set_value(const Literal &lit);

        void reset_value(int var);

        void add_clause(const Clause &c);

        void back_jump(Clause &c);

        Clause analyze_conflict(int conflict) const;

        int unit_propagate();

        bool all_variables_assigned() const;

        vector<int> &clauses_with_literal(const Literal &l);

        set<Literal> trivial;

        int decision_level;
        set<int> units;
        set<int> conflicts;
        set<pair<int, Literal>> unassigned;
        vector<int> values;
        vector<Clause> clauses;

        vector<int> level, level_time;
        vector<int> assignation_order, assignation_time;
        vector<set<int>> implications_t;
        vector<int> true_literals, false_literals;
        vector<set<Literal>> clause_unassigned_literals;

        vector<int> literal_active_clauses[2];
        vector<vector<int>> literal_clauses[2];

        std::unordered_map<int, int> coords_t;
    };

    class Solver {
    public:
        Interpretation solve(Formula &f, vector<Clause> *proof);
    };
}


#endif //CDCL_SAT_SOLVER_CDCL_SOLVER_H