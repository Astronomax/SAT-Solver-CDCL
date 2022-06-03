#ifndef CDCL_SAT_SOLVER_CDCL_SOLVER_H
#define CDCL_SAT_SOLVER_CDCL_SOLVER_H

#include "Entities.h"
#include <set>

using std::set;

namespace CDCL {
    struct SolverState {
        explicit SolverState(Formula f);

        void make_new_decision();

        void set_value(const Literal &l);

        void reset_value(int var);

        void add_clause(const Clause &c);

        void back_jump(Clause &c);

        Clause analyze_conflict(int conflict) const;

        int unit_propagate();

        bool all_variables_assigned() const;

        vector<int>& clauses_with_literal(const Literal &l);

        int decision_level;
        set<int> units;
        set<int> conflicts;
        set<int> unassigned;
        vector<int> values;
        vector<Clause> clauses;

        vector<int> level, level_time;
        vector<int> assignation_order, assignation_time;
        vector<set<int>> implications_t;
        vector<int> true_literals, false_literals;
        vector<set<Literal>> clause_unassigned_literals;

        vector<vector<int>> literal_clauses[2];
    };

    class Solver : public SATSolver {
    public:
        bool solve(const Formula &f) override;
    };
}


#endif //CDCL_SAT_SOLVER_CDCL_SOLVER_H
