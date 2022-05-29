#ifndef CDCL_SAT_SOLVER_CDCL_SOLVER_H
#define CDCL_SAT_SOLVER_CDCL_SOLVER_H

#include "Entities.h"
#include <map>
#include <set>

using std::map;
using std::set;
using std::vector;

namespace CDCL {
    const int U = -1;

    struct SolverState {
        std::pair<int, int> get_backtrack_level(Clause &conflict_clause);

        SolverState(Formula &f);

        void make_new_decision();

        void set_value(const Literal &l);

        void reset_value(int var);

        void add_clause(Clause &c);

        void back_jump(Clause &c);

        Clause analyze_conflict(int conflict);

        int unit_propagate();

        int all_variables_assigned() const;

        void remove_conflict(int conflict);

        int decision_level;

        set<int> unassigned;
        set<int> units;
        set<int> conflicts;

        vector<int> values;
        vector<Clause> clauses;
        vector<int> level, level_time;
        vector<int> assignation_order;
        vector<set<int>> implications, implications_t;
        vector<int> true_literals;
        vector<int> false_literals;
        vector<set<Literal>> clause_unassigned_literals;
        vector<vector<int>> literal_clauses;
        vector<int> is_pred;
    };

    struct Solver {
        bool solve(Formula f);
    };
}


#endif //CDCL_SAT_SOLVER_CDCL_SOLVER_H
